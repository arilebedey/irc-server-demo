#include "../../../includes/command/command.hpp"
#include <iostream>

void Command::invite() {
  if (!_caller->getIsRegistered()) {
    _server->sendMessage(_caller, errNotRegistered());
    return;
  }

  if (_args.size() > 2) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }

  std::string nick = _args[0];
  std::string channelName = _args[1];

  if (!validateChannelName(channelName)) {
    _server->sendMessage(_caller, errBadChannelMask(channelName));
    return;
  }

  channelName = channelName.substr(1);

  Channel *channel = _server->getChannel(channelName);
  if (!channel) {
    _server->sendMessage(_caller, errNoSuchChannel(channelName));
    return;
  }
  if (!channel->isMember(_caller->getFd())) {
    _server->sendMessage(_caller, errNotOnChannel(channelName));
    return;
  }
  if (!channel->isOperator(_caller->getFd())) {
    _server->sendMessage(_caller, errChanOpPrivsNeeded(channelName));
    return;
  }

  Client *target = _server->getClientFromNick(nick);
  if (!target) {
    _server->sendMessage(_caller, errNoSuchNick(nick));
    return;
  }

  if (channel->isMember(target->getFd())) {
    _server->sendMessage(_caller, errUserOnChannel(channelName));
    return;
  }

  channel->addInvited(target->getFd());

  std::string prefix =
      ":" + _caller->getNick() + "!" + _caller->getUser() + "@127.0.0.1";
  std::string response =
      prefix + " INVITE " + nick + " #" + channelName + "\r\n";
  _server->sendMessage(target, response);
}
