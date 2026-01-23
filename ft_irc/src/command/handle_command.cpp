#include "../../includes/command/command.hpp"
#include <iostream>

void Command::cap() {

  if (_args[0] == "LS") {
    _server->sendMessage(_caller, "CAP * LS :\r\n");
  } else if (_args[0] == "END") {
    welcomeUser(_caller);
  } else {
    return;
  }
}

void Command::ping() {
  if (_args.empty()) {
    _server->sendMessage(_caller, Command::errNeedMoreParams());
    return;
  }

  std::string response = ":" + _server->getName() + " PONG " +
                         _server->getName() + " :" + _args[0] + "\r\n";
  _server->sendMessage(_caller, response);
}

void Command::pass() {
  if (_caller->getIsRegistered()) {
    _server->sendMessage(_caller, errAlreadyRegistered());
    return;
  }
  if (_args.empty()) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }
  if (_server->getPassword() != _args[0]) {
    _server->sendMessage(_caller, errPasswdMismatch());
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
  if (!_caller->getLoggedIn()) {
    return;
  }
  if (_args.empty()) {
    _server->sendMessage(_caller, errNoNicknameGiven());
    return;
  }
  if (/*check if the username is using valid format.*/ false) {
    _server->sendMessage(_caller, errErroneusNickname(_args[0]));
    return;
  }
  if (_server->isNickTaken(_args[0])) {
    _server->sendMessage(_caller, errNicknameInUse(_args[0]));
    return;
  }
  if (!_caller->getIsRegistered()) {
    _caller->setNick(_args[0]);
    welcomeUser(_caller);
  } else {
    // TODO : send this message to every channel/privmessage to make them
    // acknowledge the nick edit.
    _server->sendMessage(_caller,
                         ":" + _caller->getNick() + " NICK :" + _args[0]);
    _caller->setNick(_args[0]);
  }
}

void Command::privmsg() {
  if (_args.empty() || _args.size() != 1 || _trailing.empty())
    return;
  std::string target = _args[0];
  if (target.c_str()[0] == '#' || target.c_str()[0] == '&') {
    // the target is a channel.
    // TODO : check if the user is able to chat in this channel or not.
    target = target.erase(0, 1);
    Channel *channel = _server->getChannel(target);
    if (!channel)
      return;
    _server->MsgToServer(channel, _caller,
                         craftMessage(_caller, _args[0], _trailing));
  } else {
    // the target of the message is an other user.
    Client *client = _server->getClientFromNick(target);
    if (!client)
      return;
    _server->sendMessage(client, craftMessage(_caller, _args[0], _trailing));
  }
}

void Command::user() {
  if (!_caller->getLoggedIn())
    return; // There might be a proper err code
  if (_args.size() != 3 && _args.size() != 4)
    return;
  if (_trailing.empty())
    return;
  _caller->setUser(_args[0]);
  _caller->setReal(_trailing);
  welcomeUser(_caller);
  // TODO: implement proper logic
}
