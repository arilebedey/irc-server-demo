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

  bool validateChannelName(std::string channelName);
  bool validateModeParamCount(std::vector<std::string> _args);

  // Error messages.
  std::string errAlreadyRegistered();

  std::string errNeedMoreParams();
  std::string errPasswdMismatch();
  std::string errNoNicknameGiven();
  std::string errErroneusNickname(std::string nick);
  std::string errNicknameInUse(std::string nick);
  std::string infoSuccesConnexion();
  std::string errNotRegistered();
  std::string errNoSuchNick(std::string nick);

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
  std::string errUserNotInChannel(std::string nick, std::string channel);

public:
  Command(Server *server, Client *caller, const std::string &req);
  ~Command() {};

  void debug_print();

  void cap();
  void invite();
  void join();
  void mode();
  void nick();
  void part();
  void pass();
  void ping();
  void privmsg();
  void topic();
  void user();

  std::string getName() const { return _name; }
  std::vector<std::string> getArgs() const { return _args; }
  std::string getTrailing() const { return _trailing; }

  std::string craftMessage(Client *sender, std::string target,
                           std::string message);
  bool isValid() const { return !_name.empty(); }
  void welcomeUser(Client *caller);
};

#endif
