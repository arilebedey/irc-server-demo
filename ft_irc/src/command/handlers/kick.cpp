#include "../../../includes/command/command.hpp"

void Command::kick() {
  if (!_caller->getIsRegistered())
    return;

  if (_args.size() < 2) {
    _server->sendMessage(_caller, Command::errNeedMoreParams());
    return;
  }

  std::string channelName = _args[0];
  std::string targetNick = _args[1];

  if (!validateChannelName(channelName)) {
    _server->sendMessage(_caller, Command::errBadChannelMask(channelName));
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

  Client *target = _server->getClientFromNick(targetNick);
  if (!target) {
    _server->sendMessage(_caller, errNoSuchNick(targetNick));
    return;
  }

  if (!channel->isMember(target->getFd())) {
    _server->sendMessage(_caller, errUserNotInChannel(targetNick, channelName));
    return;
  }

  std::string reason = (_args.size() > 2) ? _trailing : "";
  std::string prefix =
      ":" + _caller->getNick() + "!" + _caller->getUser() + "@127.0.0.1";
  std::string message = prefix + " KICK #" + channelName + " " + targetNick;
  if (!reason.empty())
    message += " :" + reason + "\r\n";
  else
    message += "\r\n";

  _server->broadcastToChannel(channel, message);
  channel->removeMember(target->getFd());
  _server->deleteChannelIfEmpty(channelName);
}
