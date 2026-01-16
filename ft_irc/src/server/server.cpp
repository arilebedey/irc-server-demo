#include "../../includes/server/server.hpp"
#include <iostream>

Server::Server() { _servSocketFd = -1; };

void Server::servInit() {
  this->_port = 4444;
  servSocket();

  std::cout << "Server #" << _servSocketFd << " connected" << std::endl;
  std::cout << "Waiting for clients\n";
}

void Server::servSocket() {
  struct sockaddr_in addr;
  struct pollfd _newPoll;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(this->_port);
  addr.sin_addr.s_addr = INADDR_ANY;

  _servSocketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_servSocketFd == -1)
    throw(std::runtime_error("faild to create socket"));

  _newPoll.fd = _servSocketFd;
  _newPoll.events = POLLIN;
  _newPoll.revents = 0;
  _fds.push_back(_newPoll);

  /* add socket options, bind?, listen */
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
