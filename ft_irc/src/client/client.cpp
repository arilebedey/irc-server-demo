#include "../../includes/client/client.hpp"
#include <iostream>

void Client::appendBuffer(const char *buffer, size_t len) {
  _buffer.append(buffer, len);
}

std::string Client::extractLine() {
  size_t pos = _buffer.find("\r\n");

  if (pos == std::string::npos) {
    if (_buffer.length() >= MAX_BUFFER_SIZE) {
      return "ERROR";
    }
    return "";
  }

  if (pos + 2 > MAX_BUFFER_SIZE) {
    return "ERROR";
  }

  std::string req = _buffer.substr(0, pos);
  _buffer.erase(0, pos + 2);

  return req;
}

void Client::clearOutBuffer(size_t bytes) {
  if (bytes >= _outBuffer.length())
    _outBuffer.clear();
  else
    _outBuffer.erase(0, bytes);
}

void Client::addMessage(std::string message) { _outBuffer += message; }

std::string Client::getIdentity() {
  if (_nick.empty() || _user.empty())
    return "Unknown!Unknown@127.0.0.1";
  return _nick + "!" + _user + "@127.0.0.1";
}
