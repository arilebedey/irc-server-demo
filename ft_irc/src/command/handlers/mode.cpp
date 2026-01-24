#include "../../../includes/command/command.hpp"
#include <cstdlib>
#include <iostream>

void Command::mode() {
  if (!_caller->getIsRegistered()) {
    _server->sendMessage(_caller, errNotRegistered());
    return;
  }

  if (_args.empty()) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }

  std::string channelName = _args[0];

  if (!validateChannelName(channelName)) {
    std::cout << channelName << std::endl;
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

  if (_args.size() == 1) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }

  std::string modes = _args[1];

  if (modes.empty() || (modes[0] != '+' && modes[0] != '-')) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }

  bool adding = (modes[0] == '+');

  if (!validateModeParamCount(_args)) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }

  int paramIdx = 0;

  for (size_t i = 1; i < modes.size(); ++i) {
    char mode = modes[i];

    if (mode == '+')
      adding = true;
    if (mode == '-')
      adding = false;

    if (mode == 'i')
      channel->setInviteOnly(adding);

    if (mode == 't')
      channel->setTopicCmdRestricted(adding);

    if (mode == 'k') {
      if (adding) {
        channel->setKey(_args[paramIdx + 2]);
        paramIdx++;
      } else
        channel->setKey("");
    }

    if (mode == 'o') {
      Client *target = _server->getClientFromNick(_args[paramIdx + 2]);
      if (!target) {
        _server->sendMessage(_caller, errNoSuchNick(_args[paramIdx + 2]));
        paramIdx++;
        continue;
      }
      if (!channel->isMember(target->getFd())) {
        _server->sendMessage(
            _caller, errUserNotInChannel(_args[paramIdx + 2], channelName));
        paramIdx++;
        continue;
      }
      if (adding)
        channel->addOperator(target->getFd());
      else
        channel->removeOperator(target->getFd());
      paramIdx++;
    }

    if (mode == 'l') {
      if (adding) {
        size_t limit = atoi(_args[paramIdx + 2].c_str());
        channel->setUserLimit(limit);
        paramIdx++;
      } else
        channel->setUserLimit(0);
    }
  }

  std::string prefix =
      ":" + _caller->getNick() + "!" + _caller->getUser() + "@127.0.0.1";
  std::string response = prefix + " MODE #" + channelName + " " + modes;
  for (size_t i = 2; i < _args.size(); i++)
    response += " " + _args[i];
  response += "\r\n";
  _server->broadcastToChannel(channel, response);
}

bool Command::validateModeParamCount(std::vector<std::string> _args) {
  size_t requiredParams = 0;
  bool adding = true;
  std::string modes = _args[1];

  for (size_t i = 0; i < modes.length(); i++) {
    if (modes[i] == '+') {
      adding = true;
    } else if (modes[i] == '-') {
      adding = false;
    } else if (modes[i] == 'k' && adding) {
      requiredParams++; // no param when removing key
    } else if (modes[i] == 'o') {
      requiredParams++; // operator add/del needs a nick
    } else if (modes[i] == 'l' && adding) {
      requiredParams++; // limit only has param on add
    }
  }

  if (_args.size() < 2 + requiredParams) {
    return false;
  }

  return true;
}
