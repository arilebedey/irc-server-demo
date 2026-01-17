#include "../../includes/server/server.hpp"
#include "../../includes/client/client.hpp"
#include <errno.h>
#include <iostream>
#include <stdlib.h>

/*
        Next TODO :
        - Handle disconnection from users
        - Handle the writing of message server -> client
        - Creation of user instance so they have their own attribute/buffer
        - Login using the password stored in this->_password
*/

Server::Server() : _servSocketFd(-1), _signal(1) {}

Server::~Server() {}

void Server::servInit(int port, char *password) {
  this->_port = port;
  this->_password = std::string(password);
  this->_signal = 1;
  servSocket();
  servLoop();

  std::cout << "Server #" << _servSocketFd << " connected" << std::endl;
  std::cout << "Waiting for clients\n";
}

void Server::servLoop() {
  while (this->_signal) {
    int ready = poll(&_fds[0], _fds.size(), -1);

    if (ready == -1)
      throw(std::runtime_error("poll failed"));

    for (size_t i = 0; i < _fds.size() && ready > 0; i++) {
      if (_fds[i].revents == 0)
        continue;

      --ready;

      if (_fds[i].revents & POLLIN)
        handleRead(_fds[i].fd);

      //   if (_fds[i].revents & POLLOUT)
      //     handleWrite(_fds[i].fd);

      //   if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
      //     disconnectClient(_fds[i].fd);
    }
  }
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
      // Do something else for cleanup?
      throw(std::runtime_error("failed to set O_NONBLOCK"));
    }

    struct pollfd pfd;
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    Client newClient;

    _fds.push_back(pfd);
  }
}

void Server::handleRead(int fd) {
  if (fd == this->_servSocketFd) {
    acceptClient();
    return;
  }
  receiveFromClient(fd);
}

// TODO -> switch the temporary buffer `clientBuffer` for one
// client directly that will be stored in the class
void Server::receiveFromClient(int fd) {
  char buffer[512];
  std::string clientBuffer;
  ssize_t n = recv(fd, buffer, sizeof(buffer), 0);

  if (n <= 0) {
    // disconnectClient(fd);
    std::cerr << "Error: nothing has been read" << std::endl;
    return;
  }

  clientBuffer.append(buffer, n);
  std::cout << clientBuffer << std::endl;
}

// TODO -> the clean exit logic need to be coded in both Destructor and
// signalHandler to make sure that the program exit always properly
void Server::signalHandler(int signum) {
  if (signum == SIGINT || signum == SIGQUIT)
    exit(0);
}

/* Erasing at interator downshifts the vector, so to avoid skipping over
   element reverse interation is used
*/
void Server::clearClients(int fd) {
  for (int i = _fds.size() - 1; i >= 0; i--) {
    if (_fds[i].fd == fd) {
      _fds.erase(_fds.begin() + i);
      break;
    }
  }
  for (int i = _clients.size() - 1; i >= 0; i--) {
    if (_clients[i].getFd() == fd) {
      _clients.erase(_clients.begin() + i);
      break;
    }
  }
}
