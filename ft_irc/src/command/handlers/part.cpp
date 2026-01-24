#include "../../../includes/command/command.hpp"

void Command::part() {
  if (_args.empty()) {
    _server->sendMessage(_caller, Command::errNeedMoreParams());
    return;
  }

  std::string channelName = _args[0];

  if (!validateChannelName(channelName)) {
    std::cout << channelName << std::endl;
    _server->sendMessage(_caller, Command::errBadChannelMask(channelName));
    return;
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
