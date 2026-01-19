#include "../../includes/channel/channel.hpp"

Channel::Channel(const std::string &name)
    : _name(name), _userLimit(0), _inviteOnly(false),
      _topicCmdRestricted(false) {}

Channel::~Channel() {}

bool Channel::isMember(int clientFd) const {
  return _members.find(clientFd) != _members.end();
}

bool Channel::isOperator(int clientFd) const {
  return _operators.find(clientFd) != _operators.end();
}

bool Channel::isInvited(int clientFd) const {
  return _invited.find(clientFd) != _invited.end();
}

void Channel::addMember(int clientFd) {
  _members.insert(clientFd);
  if (isInvited(clientFd))
    _invited.erase(clientFd);
}
