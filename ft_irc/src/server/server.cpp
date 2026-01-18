#include "../../includes/server/server.hpp"
#include "../../includes/client/client.hpp"

/*
   TODO :
  - Handle disconnection from users
  - Handle the writing of message server -> client
  - Creation of user instance so they have their own attribute/buffer
  - Login using the password stored in this->_password
*/

Server::Server() : _servSocketFd(-1) {}

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
      } else if (revents & POLLIN) {
        handleRead(fd);
      } else if (revents & POLLOUT) {
        handleWrite(fd);
      }
    }
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

  client->addMessage(message);
  client_pfd->events |= POLLOUT;
}