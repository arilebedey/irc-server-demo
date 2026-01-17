#include "../../includes/client/client.hpp"

void Client::appendBuffer(std::string buffer, size_t len) {
  _buffer.append(buffer, len);
}

bool Client::hasCompleteLine() {
  return _buffer.find("\r\n") != std::string::npos ||
         _buffer.find("\n") != std::string::npos;
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
