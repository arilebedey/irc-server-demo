#include "../../includes/server/server.hpp"

Server::Server() { _servSocketFd = -1; };

void Server::ClearClients(int fd) {
  for (size_t i = 0; i < fds.size(); i++) {
    if (fds[i].fd == fd) {
      fds.erase(fds.begin() + i);
      break;
    }
  }
  for (size_t i = 0; i < clients.size(); i++) {
    if (clients[i].getFd() == fd) {
      clients.erase(clients.begin() + i);
      break;
    }
  }
}
