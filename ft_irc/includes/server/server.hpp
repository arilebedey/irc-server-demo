#ifndef SERVER_HPP
#define SERVER_HPP

#include "../client/client.hpp"
#include <csignal>
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
  std::string _password;
  std::vector<Client> _clients;
  std::vector<struct pollfd> _fds;
  static volatile sig_atomic_t _sigReceived;

public:
  Server();
  ~Server();

  void servInit(int port, char *password);
  void servSocket();
  void servLoop();

  void setupSignals();
  static void signalHandler(int signum);

  Client *getClientFromFd(int fd);

  void acceptClient();
  void handleRead(int fd);
  void handleWrite(int fd);
  void receiveFromClient(int fd);
  void disconnectClient(int fd);

  void cleanup();
  void closeFds();
  void clearClients(int fd);
};

#endif
