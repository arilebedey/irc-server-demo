#include "../../includes/client/client.hpp"
#include <iostream>

void Client::appendBuffer(const char *buffer, size_t len) {
    _buffer.append(buffer, len);
}

bool Client::hasCompleteLine() {
  return (_buffer.find("\r\n") != std::string::npos ||
          _buffer.find("\n") != std::string::npos);
}

std::string Client::extractLine() {
  size_t pos = _buffer.find("\r\n");
  size_t erase_len = 2;

  if (pos == std::string::npos) {
    pos = _buffer.find("\n");
    erase_len = 1;
  }
  if (pos == std::string::npos)
    return "";

  std::string line = _buffer.substr(0, pos);
  _buffer.erase(0, pos + erase_len);

  return line;
}

void Client::clearOutBuffer(size_t bytes) {
  if (bytes >= _outBuffer.length())
    _outBuffer.clear();
  else
    _outBuffer.erase(0, bytes);
}
