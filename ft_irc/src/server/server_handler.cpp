#include "../../includes/client/client.hpp"
#include "../../includes/command/command.hpp"
#include "../../includes/server/server.hpp"

void Server::handleWrite(int fd) {
  Client *client = getClientFromFd(fd);
  if (!client) {
    disconnectClient(fd);
    return;
  }

  const std::string msg = client->getOutBuffer();
  if (msg.empty())
    return;

  ssize_t n = send(fd, msg.c_str(), msg.length(), 0);
  std::cout << "[SENDing to " << fd << "]: " << msg << std::endl;

  if (n == -1) {
    disconnectClient(fd);
    return;
  }

  client->clearOutBuffer(n);
  if (!client->getOutBuffer().empty())
    return;

  pollfd *client_pfd = getPollFdFromFd(fd);
  if (!client_pfd)
    return;

  client_pfd->events &= ~POLLOUT;
}

void Server::acceptClient() {
  while (true) {
    int clientFd = accept(this->_servSocketFd, NULL, NULL);
    if (clientFd == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;
      throw(std::runtime_error("accept failed"));
    }

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
      close(clientFd);
      throw(std::runtime_error("failed to set O_NONBLOCK"));
    }

    int nodelay = 1;
    if (setsockopt(clientFd, IPPROTO_TCP, TCP_NODELAY, &nodelay,
                   sizeof(nodelay)) == -1) {
      close(clientFd);
      throw(std::runtime_error("failed to set TCP_NODELAY on client"));
    }

    struct pollfd pfd;
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    _fds.push_back(pfd);

    Client newClient;
    newClient.setFd(clientFd);
    _clients.push_back(newClient);
    _clientFdMap[clientFd] = _clients.size() - 1;

    std::cout << "Client fd " << clientFd << " connected" << std::endl;
  }
}

void Server::signalHandler(int signum) {
  (void)signum;
  Server::_sigReceived = 1;
}

void Server::handleRead(int fd) {
  // If it's the server socket
  if (fd == this->_servSocketFd) {
    acceptClient();
    return;
  }
  // If it's the client socket
  receiveFromClient(fd);
}

void Server::receiveFromClient(int fd) {
  char buffer[512];
  ssize_t n = recv(fd, buffer, sizeof(buffer), 0);

  if (n <= 0) {
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
      return; // safety: rare race condition in poll(), no actual data ready
    disconnectClient(fd);
    return;
  }

  Client *client = getClientFromFd(fd);
  if (!client) {
    std::cerr << "Warning: client fd " << fd << " not found" << std::endl;
    clearClients(fd);
    close(fd);
    return;
  }

  client->appendBuffer(buffer, n);

  std::string req;
  while ((req = client->extractLine()) != "") {
    if (req == "ERROR") {
      std::cout << "Client fd " << fd << " sent erroneous request" << std::endl;
      disconnectClient(fd);
      return;
    }
    std::cout << "Client fd " << fd << ": " << req << std::endl;
    Command cmd(this, client, req);
    // cmd.debug_print();

    if (getClientFromFd(fd) == NULL) {
      return;
    }
  }
}

void Server::disconnectClient(int fd, const std::string &reason) {
  Client *client = getClientFromFd(fd);

  if (client) {
    std::vector<std::string> channelsToDelete;

    for (ChannelMap::iterator it = _channels.begin(); it != _channels.end();
         ++it) {
      Channel &channel = it->second;
      if (channel.isMember(fd)) {
        channel.removeMember(fd);

        if (channel.getMemberCount() == 0) {
          channelsToDelete.push_back(it->first);
        }
      }
    }

    for (size_t i = 0; i < channelsToDelete.size(); ++i) {
      _channels.erase(channelsToDelete[i]);
    }

    std::string prefix =
        ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1";
    std::string message = prefix + " QUIT :" + reason + "\r\n";
    sendToVisible(client, message);
  }

  if (fd > 0)
    close(fd);
  clearClients(fd);
  std::cout << "Client fd " << fd << " disconnected" << std::endl;
}

void Server::flushAllWrites() {
  for (size_t i = 1; i < _fds.size(); i++) {
    if (_fds[i].events & POLLOUT) {
      Client *client = getClientFromFd(_fds[i].fd);
      if (client && !client->getOutBuffer().empty()) {
        const std::string &msg = client->getOutBuffer();
        ssize_t n = send(_fds[i].fd, msg.c_str(), msg.length(), 0);
        if (n > 0) {
          client->clearOutBuffer(n);
          if (client->getOutBuffer().empty()) {
            _fds[i].events &= ~POLLOUT;
          }
        }
      }
    }
  }
}
