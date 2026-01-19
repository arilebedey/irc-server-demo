#include "../../includes/client/client.hpp"
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

    struct pollfd pfd;
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    _fds.push_back(pfd);

    Client newClient;
    newClient.setFd(clientFd);
    _clients.push_back(newClient);

    std::cout << "Client fd " << clientFd << " connected" << std::endl;
    // Get the client from the vector (not the local variable)
    Client *client = getClientFromFd(clientFd);
    if (client)
      Server::sendMessage(client, "Welcome to the ft_irc server!\r\n");
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

  std::cout << "receiveFromClient n : " << n << std::endl;

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
    // TODO: parseCommand(line) and handleCommand(fd, cmd)

    // Broadcast the message to all clients to test sendMessage()
    // TODO : Delete this.
    for (long unsigned int i = 0; i < _clients.size(); i++) {
      if (_clients[i].getFd() != fd)
        Server::sendMessage(&_clients[i], req + "\r\n");
    }
  }
}

void Server::disconnectClient(int fd) {
  if (fd > 0) {
    close(fd);
    clearClients(fd);
    std::cout << "Client fd " << fd << " disconnected" << std::endl;
  }
}
