#include "../../includes/client/client.hpp"

void Client::appendBuffer(const char *buffer, size_t len) {
  _buffer.append(buffer, len);
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
