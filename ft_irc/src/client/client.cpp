#include "../../includes/client/client.hpp"
#include <iostream>

void Client::appendBuffer(const char *buffer, size_t len) {
  _buffer.append(buffer, len);

  if (_buffer.length() > MAX_BUFFER_SIZE) {
    std::cerr << "Buffer overflow: message contains < 512 characters" << _fd
              << std::endl;
    _buffer.clear();
  }
}

std::string Client::extractRequest() {
  size_t pos = _buffer.find("\r\n");

  if (pos == std::string::npos)
    return "";

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
