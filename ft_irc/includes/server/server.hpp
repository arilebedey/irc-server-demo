#ifndef SERVER_HPP
#define SERVER_HPP

#include "../client/client.hpp"
#include <csignal>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
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
  // server.cpp
  Server();
  ~Server();
  void setupSignals();
  void servInit(int port, char *password);
  void sendMessage(Client *client, std::string message);
  void servSocket();
  void servLoop();

  // server_utils.cpp
  Client *getClientFromFd(int fd);
  pollfd *getPollFdFromFd(int fd);
  void cleanup();
  // void closeFds(); not defined
  void clearClients(int fd);

  // server_handler.cpp
  static void signalHandler(int signum);
  void acceptClient();
  void handleRead(int fd);
  void handleWrite(int fd);
  void receiveFromClient(int fd);
  void disconnectClient(int fd);
};

#endif
