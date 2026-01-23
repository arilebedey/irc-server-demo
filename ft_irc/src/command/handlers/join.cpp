#include "../../../includes/command/command.hpp"
#include <iostream>

void Command::join() {
  if (!_caller->getIsRegistered())
    return;

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
