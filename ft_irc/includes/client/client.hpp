#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
private:
  int _fd;
  std::string _addr;
  std::string _buffer;

public:
  Client() {};
  ~Client() {};

  int getFd() { return _fd; }
  void setFd(int fd) { _fd = fd; }
  void setAddr(std::string addr) { _addr = addr; }

  void appendBuffer(std::string buffer, size_t len);
  std::string extractLine();
};

#endif
