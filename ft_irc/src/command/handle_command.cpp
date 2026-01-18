#include "../../includes/command/command.hpp"
#include <iostream>

void Command::cap() { _server->sendMessage(_caller, "CAP * LS :\r\n"); }

void Command::pass() {
  if (_caller->getIsRegistered()) {
    _server->sendMessage(_caller, ":" + _server->getName() + " 462 " +
                                      _caller->getNick() +
                                      " :You may not reregister\r\n");
    return;
  }
  if (_args.empty()) {
    _server->sendMessage(_caller, ":" + _server->getName() +
                                      " 461 * PASS :Not enough parameters\r\n");
    return;
  }
  if (_server->getPassword() != _args[0]) {
    _server->sendMessage(_caller, ":" + _server->getName() +
                                      " 464 * :Password incorrect\r\n");
    _server->disconnectClient(_caller->getFd());
    return;
  }
  _caller->setLoggedIn(true);
}
/*
    All error codes are given and explain here :
    https://datatracker.ietf.org/doc/html/rfc1459#section-6.1
*/
void Command::nick() {
  if (_args.empty()) {
    _server->sendMessage(_caller, ":" + _server->getName() +
                                      " 431 * :No nickname given");
    return;
  }

  // TODO: implement proper logic
  if (/*check if the username is using valid format.*/ false) {
    _server->sendMessage(_caller, ":" + _server->getName() + " 432 * " +
                                      _args[0] + " :Erroneus nickname\r\n");
    return;
  }

  if (/*check if the username is using valid format.*/ false) {
    _server->sendMessage(_caller, ":" + _server->getName() + " 433 * " +
                                      _args[0] +
                                      " :Nickname is already in use\r\n");
    return;
  }
  _caller->setNick(_args[0]);
}

void Command::user() {
  // TODO: implement proper logic
  
}