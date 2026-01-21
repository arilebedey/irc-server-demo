#ifndef SERVER_HPP
#define SERVER_HPP

#include "../channel/channel.hpp"
#include "../client/client.hpp"

#include <csignal>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

typedef std::map<std::string, Channel> ChannelMap;

class Server {
private:
  int _port;
  int _servSocketFd;
  std::string _password;
  std::vector<Client> _clients;
  std::vector<struct pollfd> _fds;
  static volatile sig_atomic_t _sigReceived;
  std::string _serverName;
  ChannelMap _channels;

public:
  // server.cpp
  Server();
  ~Server();
  void setupSignals();
  void servInit(int port, char *password);
  void sendMessage(Client *client, std::string message);
  void servSocket();
  void servLoop();
  // Simple Getter
  std::string getPassword() const { return _password; };
  std::string getName() const { return _serverName; };
  // server_utils.cpp
  Client *getClientFromFd(int fd);
  pollfd *getPollFdFromFd(int fd);
  void cleanup();
  // void closeFds(); not defined
  void clearClients(int fd);

  // Channel-specific
  bool checkChannelExists(const std::string &name);
  Channel *getOrCreateChannel(const std::string &name);
  Channel *getChannel(const std::string &name);
  void deleteChannelIfEmpty(const std::string &name);

  // server_handler.cpp
  static void signalHandler(int signum);
  void acceptClient();
  void handleRead(int fd);
  void handleWrite(int fd);
  void receiveFromClient(int fd);
  void disconnectClient(int fd);
};

#endif
