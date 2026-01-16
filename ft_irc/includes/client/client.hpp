#include <string>

class Client {
private:
  int _fd;
  std::string _addr;

public:
  Client() {};

  int getFd() { return _fd; }
  void setFd(int fd) { _fd = fd; }
  void setAddr(std::string addr) { _addr = addr; }
};
