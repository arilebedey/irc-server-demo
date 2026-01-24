#include "../../../includes/command/command.hpp"
#include <iostream>

void Command::topic() {
  if (!_caller->getIsRegistered()) {
    _server->sendMessage(_caller, errNotRegistered());
    return;
  }

  if (_args.size() > 2) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }

  std::string channelName = _args[0];

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

  if (_trailing.empty()) {
    std::string topic = channel->getTopic();
    if (topic.empty()) {
      _server->sendMessage(_caller, ":" + _server->getName() + " 331 " +
                                        _caller->getNick() + " #" +
                                        channelName + " :No topic is set\r\n");
    } else {
      _server->sendMessage(_caller, ":" + _server->getName() + " 332 " +
                                        _caller->getNick() + " #" +
                                        channelName + " :" + topic + "\r\n");
    }
    return;
  }

  if (channel->isTopicCmdRestricted() &&
      !channel->isOperator(_caller->getFd())) {
    _server->sendMessage(_caller, errChanOpPrivsNeeded(channelName));
    return;
  }

  channel->setTopic(_trailing);

  std::string prefix =
      ":" + _caller->getNick() + "!" + _caller->getUser() + "@127.0.0.1";
  std::string message =
      prefix + " TOPIC #" + channelName + " :" + _trailing + "\r\n";
  _server->broadcastToChannel(channel, message);
}
