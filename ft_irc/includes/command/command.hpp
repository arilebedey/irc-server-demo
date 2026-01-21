#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../client/client.hpp"
#include "../server/server.hpp"
#include <iostream>
#include <string>
#include <vector>

class Server;

class Command {
private:
  std::string _name;
  std::vector<std::string> _args;
  std::string _trailing;

  Server *_server;
  Client *_caller;
  void parse(const std::string &req);

  void handle();

  // Error messages.
  std::string errAlreadyRegistered();

  std::string errNeedMoreParams();
  std::string errPasswdMismatch();
  std::string errNoNicknameGiven();
  std::string errErroneusNickname(std::string nick);
  std::string errNicknameInUse(std::string nick);
  std::string infoSuccesConnexion();

  // Channel-specific errors
  std::string errBadChannelMask(std::string name);
  std::string errChanOpPrivsNeeded(std::string channel);
  std::string errBadChannelKey(std::string channel);
  std::string errInviteOnlyChan(std::string channel);
  std::string errChannelIsFull(std::string channel);
  std::string errKeySet(std::string channel);
  std::string errNoSuchChannel(std::string channel);
  std::string errNotOnChannel(std::string channel);
  std::string errUserOnChannel(std::string channel);

public:
  Command(Server *server, Client *caller, const std::string &req);
  ~Command() {};

  void debug_print();

  void cap();
  void user();
  void nick();
  void pass();
  void join();
  void part();

  std::string getName() const { return _name; }
  std::vector<std::string> getArgs() const { return _args; }
  std::string getTrailing() const { return _trailing; }
  bool isValid() const { return !_name.empty(); }
};

#endif
