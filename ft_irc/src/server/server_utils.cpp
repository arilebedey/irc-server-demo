#include "../../includes/client/client.hpp"
#include "../../includes/server/server.hpp"

Client *Server::getClientFromFd(int fd) {
  ClientFdMap::iterator it = _clientFdMap.find(fd);
  if (it != _clientFdMap.end() && it->second < _clients.size()) {
    return &(_clients[it->second]);
  }
  return NULL;
}

Client *Server::getClientFromNick(std::string nick) {
  for (long unsigned int i = 0; i < _clients.size(); i++) {
    if (_clients[i].getNick() == nick)
      return &(_clients[i]);
  }
  return NULL;
}

pollfd *Server::getPollFdFromFd(int fd) {
  for (long unsigned int i = 1; i < _fds.size(); i++) {
    if (_fds[i].fd == fd)
      return (&_fds[i]);
  }
  return NULL;
}

bool Server::isNickTaken(std::string searched) {
  if (getClientFromNick(searched) != NULL)
    return true;
  return false;
}

void Server::clearClients(int fd) {
  _clientFdMap.erase(fd);
  for (int i = _fds.size() - 1; i >= 0; i--) {
    if (_fds[i].fd == fd) {
      _fds.erase(_fds.begin() + i);
      break;
    }
  }
  for (int i = _clients.size() - 1; i >= 0; i--) {
    if (_clients[i].getFd() == fd) {
      _clients.erase(_clients.begin() + i);
      // Rebuild map indices for clients after the erased one
      for (size_t j = i; j < _clients.size(); j++) {
        _clientFdMap[_clients[j].getFd()] = j;
      }
      break;
    }
  }
}

void Server::cleanup() {
  // i = 0 is server socket (skip)
  for (size_t i = 1; i < _fds.size(); i++) {
    if (_fds[i].fd > 0)
      close(_fds[i].fd);
  }

  if (_servSocketFd >= 0) {
    close(_servSocketFd);
    _servSocketFd = -1;
  }

  _fds.clear();
  _clients.clear();
  _clientFdMap.clear();
}
