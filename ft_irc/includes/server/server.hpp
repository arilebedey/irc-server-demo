#ifndef SERVER_HPP
#define SERVER_HPP

#include "../client/client.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Server {
private:
  int _port;
  int _servSocketFd;
  bool _signal;
  std::string _password;
  std::vector<Client> _clients;
  std::vector<struct pollfd> _fds;

public:
  Server();
  ~Server();

  void servInit(int port, char *password);
  void servSocket();
  void servLoop();

  void acceptClient();
  void handleRead(int fd);
  void receiveFromClient(int fd);
  void acceptlient();
  //   void disconnectClient(int fd);

  static void signalHandler(int signum);

  void closeFds();
  void clearClients(int fd);
};

#endif
