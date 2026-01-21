#include "../../includes/command/command.hpp"
#include <iostream>

void Command::cap() {

  if (_args[0] == "LS") {
    _server->sendMessage(_caller, "CAP * LS :\r\n");
    return;
  } else if (_args[0] == "END") {
    if (_caller->getIsRegistered()) {
      std::cout << "He is already registered so welcoming him." << std::endl;
      std::string welcome_msg = infoSuccesConnexion();
      send(_caller->getFd(), welcome_msg.c_str(), welcome_msg.length(), 0);
      return;
    }
  } else {
    return;
  }
}

void Command::join() {
  if (_args.empty()) {
    _server->sendMessage(_caller, Command::errNeedMoreParams());
    return;
  }

  std::string fullChannelName = _args[0];

  if ((fullChannelName[0] != '#' || fullChannelName[0] != '&') &&
      (fullChannelName.length() < 2 || fullChannelName.length() > 200)) {
    std::cout << fullChannelName << std::endl;
    _server->sendMessage(_caller, Command::errBadChannelMask(fullChannelName));
    return;
  }

  for (size_t i = 0; i < fullChannelName.length(); i++) {
    char c = fullChannelName[i];
    if (c == 7 || c == ' ' || c == ',' || (c < 32 && c != 9) || c == 127) {
      _server->sendMessage(_caller,
                           Command::errBadChannelMask(fullChannelName));
      return;
    }
  }

  std::string channelName = fullChannelName.substr(1);

  std::string key = (_args.size() > 1) ? _args[1] : "";

  Channel *channel = _server->getOrCreateChannel(channelName);

  if (!channel->checkKey(_caller->getFd(), key)) {
    _server->sendMessage(_caller, errBadChannelKey(channelName));
    return;
  }

  if (!channel->checkInvite(_caller->getFd())) {
    _server->sendMessage(_caller, errInviteOnlyChan(channelName));
    return;
  }

  if (!channel->checkLimit(_caller->getFd())) {
    _server->sendMessage(_caller, errChannelIsFull(channelName));
    return;
  }

  if (channel->getMemberCount() == 0) {
    channel->addMember(_caller->getFd());
    channel->addOperator(_caller->getFd());
  } else {
    channel->addMember(_caller->getFd());
  }

  std::string prefix =
      ":" + _caller->getNick() + "!" + _caller->getUser() + "@127.0.0.1";
  std::string msg = prefix + "JOIN " + fullChannelName + "\r\n";
  // TODO: Broadcast `msg` to all
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
    std::string error = errPasswdMismatch();
    send(_caller->getFd(), error.c_str(), error.length(), 0);
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
  if (_caller->getLoggedIn())
    return;

  if (_args.empty()) {
    _server->sendMessage(_caller, errNoNicknameGiven());
    return;
  }

  // TODO: implement proper logic
  if (/*check if the username is using valid format.*/ false) {
    _server->sendMessage(_caller, errErroneusNickname(_args[0]));
    return;
  }

  if (/*check if the username is using valid format.*/ false) {
    _server->sendMessage(_caller, errNicknameInUse(_args[0]));
    return;
  }
  _caller->setNick(_args[0]);
}

/*
Behavior of Command::user() described here
-> https://datatracker.ietf.org/doc/html/rfc1459#section-6.1

Exemple : USER guest tolmoon tolsun :Ronnie Reagan
    - "guest" : username - mandatory
    - "tolmoon" : hostname - ignored, only used for server to server
    - "tolsun" : servername - ignored, same reason
    - "Ronine Reagan" : real identity

*/
void Command::user() {
  if (!_caller->getLoggedIn() || _caller->getNick().empty())
    return; // There might be a proper err code
  if (_args.size() != 3)
    return;
  if (_trailing.empty())
    return;
  _caller->setUser(_args[0]);
  _caller->setReal(_trailing);

  // TODO: implement proper logic
}
