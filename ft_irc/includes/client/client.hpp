#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
private:
  int _fd;
  std::string _addr;
  std::string _buffer;
  std::string _outBuffer;
  static const size_t MAX_BUFFER_SIZE = 512;

  bool _loggedIn;
  std::string _nick;
  std::string _user;
  std::string _real;

public:
  Client() : _loggedIn(false){};
  ~Client(){};

  std::string getOutBuffer() const { return _outBuffer; };
  std::string getInBuffer() const { return _buffer; };

  void clearOutBuffer(size_t bytes);
  void addMessage(std::string message);

  int getFd() { return _fd; };
  bool getLoggedIn() { return _loggedIn; };
  std::string getNick() { return _nick; };
  std::string getReal() { return _real; };
  std::string getUser() { return _user; };
  bool getIsRegistered() {
    return _loggedIn && !_nick.empty() && !_user.empty();
  };

  void setLoggedIn(bool status) { _loggedIn = status; };
  void setFd(int fd) { _fd = fd; };
  void setNick(std::string nick) { _nick = nick; };
  void setUser(std::string user) { _user = user; };
  void setReal(std::string real) { _real = real; };
  void setAddr(std::string addr) { _addr = addr; };

  void appendBuffer(const char *buffer, size_t len);
  std::string extractLine();
};

#endif
