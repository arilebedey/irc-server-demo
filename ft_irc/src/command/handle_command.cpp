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

void Command::join() {
  if (_args.empty()) {
    _server->sendMessage(_caller, Command::errNeedMoreParams());
    return;
  }

  std::string channelName = _args[0];

  if ((channelName[0] != '#' && channelName[0] != '&') ||
      (channelName.length() < 2 || channelName.length() > 200)) {
    std::cout << channelName << std::endl;
    _server->sendMessage(_caller, Command::errBadChannelMask(channelName));
    return;
  }

  for (size_t i = 0; i < channelName.length(); i++) {
    char c = channelName[i];
    if (c == 7 || c == ' ' || c == ',' || (c < 32 && c != 9) || c == 127) {
      _server->sendMessage(_caller, Command::errBadChannelMask(channelName));
      return;
    }
  }

  channelName = channelName.substr(1);

  std::string key = (_args.size() > 1) ? _args[1] : "";

  Channel *channel = _server->getOrCreateChannel(channelName);

  if (channel->isMember(_caller->getFd())) {
    _server->sendMessage(_caller, errUserOnChannel(channelName));
    return;
  }

  if (!channel->checkKey(key)) {
    _server->sendMessage(_caller, errBadChannelKey(channelName));
    return;
  }

  if (!channel->checkInvite(_caller->getFd())) {
    _server->sendMessage(_caller, errInviteOnlyChan(channelName));
    return;
  }

  if (!channel->checkLimit()) {
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
  std::string message = prefix + " JOIN #" + channelName + "\r\n";
  _server->broadcastToChannel(channel, message);
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
  welcomeUser(_caller);
}

void Command::user() {
  if (!_caller->getLoggedIn() || _caller->getNick().empty())
    return; // There might be a proper err code
  if (_args.size() != 3 && _args.size() != 4)
    return;
  if (_trailing.empty())
    return;
  std::cout << "Setting user info: " << _args[0] << ", " << _trailing
            << std::endl;
  _caller->setUser(_args[0]);
  _caller->setReal(_trailing);
  welcomeUser(_caller);
  // TODO: implement proper logic
}

void Command::part() {
  if (_args.empty()) {
    _server->sendMessage(_caller, Command::errNeedMoreParams());
    return;
  }

  std::string channelName = _args[0];

  if ((channelName[0] != '#' && channelName[0] != '&') ||
      (channelName.length() < 2 || channelName.length() > 200)) {
    std::cout << channelName << std::endl;
    _server->sendMessage(_caller, Command::errBadChannelMask(channelName));
    return;
  }

  for (size_t i = 0; i < channelName.length(); i++) {
    char c = channelName[i];
    if (c == 7 || c == ' ' || c == ',' || (c < 32 && c != 9) || c == 127) {
      _server->sendMessage(_caller, Command::errBadChannelMask(channelName));
      return;
    }
  }

  channelName = channelName.substr(1);

  Channel *channel = _server->getChannel(channelName);
  if (!channel) {
    _server->sendMessage(_caller, Command::errNoSuchChannel(channelName));
    return;
  }
  if (!channel->isMember(_caller->getFd())) {
    _server->sendMessage(_caller, Command::errNotOnChannel(channelName));
    return;
  }

  std::string prefix =
      ":" + _caller->getNick() + "!" + _caller->getUser() + "@127.0.0.1";
  std::string reason = (_args.size() > 1) ? _args[1] : "";
  std::string message = prefix + " PART #" + channelName;
  if (!reason.empty())
    message += " :" + reason + "\r\n";
  else
    message += "\r\n";
  _server->broadcastToChannel(channel, message);
  channel->removeMember(_caller->getFd());
  _server->deleteChannelIfEmpty(channelName);
}
