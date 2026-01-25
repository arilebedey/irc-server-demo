#include "../../includes/server/server.hpp"
#include "../../includes/client/client.hpp"
/*
   TODO :
  - Handle disconnection from users
  - Handle the writing of message server -> client
  - Creation of user instance so they have their own attribute/buffer
  - Login using the password stored in this->_password
*/

Server::Server() : _servSocketFd(-1), _serverName("ft_irc") {}

Server::~Server() { cleanup(); }

volatile sig_atomic_t Server::_sigReceived = 0;

void Server::setupSignals() {
  struct sigaction sig_action;

  sigemptyset(&sig_action.sa_mask);
  sig_action.sa_flags = 0;
  sig_action.sa_handler = Server::signalHandler;

  if (sigaction(SIGINT, &sig_action, NULL) == -1)
    throw(std::runtime_error("sigaction SIGINT failed"));
  if (sigaction(SIGQUIT, &sig_action, NULL) == -1)
    throw(std::runtime_error("sigaction SIGQUIT failed"));
  // NOTE: Handle SIGPIPE? See notes.md
}

void Server::servInit(int port, char *password) {
  this->_port = port;
  this->_password = std::string(password);

  setupSignals();
  servSocket();

  std::cout << "Server fd " << _servSocketFd << " connected" << std::endl;
  std::cout << "Waiting for clients\n";

  servLoop();
}

void Server::servLoop() {
  while (_sigReceived != 1) {
    int ready = poll(&_fds[0], _fds.size(), -1);

    if (ready == -1) {
      if (errno == EINTR)
        continue; // Signal interrupted poll, not error
      throw(std::runtime_error("poll failed"));
    }

    for (int i = _fds.size() - 1; i >= 0 && ready > 0; i--) {
      int fd = _fds[i].fd;
      short revents = _fds[i].revents;

      if (revents == 0)
        continue;

      ready--;

      if (revents & (POLLERR | POLLHUP | POLLNVAL)) {
        disconnectClient(fd);
        continue;
      }
      if (revents & POLLIN) {
        handleRead(fd);
      }
      if (revents & POLLOUT) {
        handleWrite(fd);
      }
    }
    // Flush all pending writes immediately after processing reads
    flushAllWrites();
  }
  Server::cleanup();
}

void Server::servSocket() {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(this->_port);
  addr.sin_addr.s_addr = INADDR_ANY;

  _servSocketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_servSocketFd == -1)
    throw(std::runtime_error("failed to create socket"));

  int reuse = 1;
  if (setsockopt(_servSocketFd, SOL_SOCKET, SO_REUSEADDR, &reuse,
                 sizeof(reuse)) == -1) {
    close(_servSocketFd);
    throw(std::runtime_error("failed to set SO_REUSEADDR"));
  }

  int nodelay = 1;
  if (setsockopt(_servSocketFd, IPPROTO_TCP, TCP_NODELAY, &nodelay,
                 sizeof(nodelay)) == -1) {
    close(_servSocketFd);
    throw(std::runtime_error("failed to set TCP_NODELAY"));
  }

  if (fcntl(_servSocketFd, F_SETFL, O_NONBLOCK) == -1) {
    close(_servSocketFd);
    throw(std::runtime_error("failed to set O_NONBLOCK"));
  }

  if (bind(_servSocketFd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    close(_servSocketFd);
    throw(std::runtime_error("failed to bind socket"));
  }

  if (listen(_servSocketFd, SOMAXCONN) == -1) {
    close(_servSocketFd);
    throw(std::runtime_error("failed to listen"));
  }

  struct pollfd servPoll;
  servPoll.fd = _servSocketFd;
  servPoll.events = POLLIN;
  servPoll.revents = 0;
  _fds.push_back(servPoll);
}

void Server::sendMessage(Client *client, std::string message) {
  if (!client)
    return;

  pollfd *client_pfd = getPollFdFromFd(client->getFd());

  if (!client_pfd)
    return;

  if (message.empty())
    return;

  if (client->getOutBuffer().empty()) {
    ssize_t sent_bytes =
        send(client->getFd(), message.c_str(), message.length(), 0);
    if (sent_bytes == -1) {
      // Error during send, buffer the whole message
      client->addMessage(message);
      client_pfd->events |= POLLOUT;
      return;
    } else if (static_cast<size_t>(sent_bytes) < message.length()) {
      // Partial send, buffer the remaining part
      client->addMessage(message.substr(sent_bytes));
      client_pfd->events |= POLLOUT;
      return;
    } else {
      return;
    }
  }
  client->addMessage(message);
  client_pfd->events |= POLLOUT;
}

void Server::broadcastToChannel(Channel *channel, std::string message) {
  sendToSet(channel->getMembers(), message);
}

void Server::msgToChannel(Channel *channel, Client *sender,
                          std::string message) {
  std::set<int> members = channel->getMembers();
  members.erase(sender->getFd());
  sendToSet(members, message);
}

void Server::sendToVisible(Client *sender, std::string message) {
  std::set<int> visible_members;

  for (ChannelMap::iterator it = _channels.begin(); it != _channels.end();
       ++it) {
    Channel &channel = it->second;
    if (channel.isMember(sender->getFd())) {
      std::set<int> channel_members = channel.getMembers();
      visible_members.insert(channel_members.begin(), channel_members.end());
    }
  }

  std::cout << "After searching, we found " << visible_members.size()
            << " visible members." << std::endl;

  sendToSet(visible_members, message);
}

void Server::sendToSet(std::set<int> receivers, std::string message) {
  for (std::set<int>::iterator it = receivers.begin(); it != receivers.end();
       ++it) {
    int fd = *it;
    Client *client = getClientFromFd(fd);

    if (client && client->getFd() == fd) {
      sendMessage(client, message);
      std::cout << "sending message to the client " << client->getNick()
                << " fd : " << fd << std::endl;
    }
  }
}
