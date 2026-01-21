#include "../../includes/channel/channel.hpp"
#include <iostream>

Channel::Channel()
    : _userLimit(0), _inviteOnly(false), _topicCmdRestricted(false) {}

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

void Channel::removeMember(int clientFd) {
  _members.erase(clientFd);
  if (isOperator(clientFd))
    _operators.erase(clientFd);
}

void Channel::addOperator(int clientFd) {
  if (isMember(clientFd))
    _operators.insert(clientFd);
}

void Channel::removeOperator(int clientFd) { _operators.erase(clientFd); }

void Channel::addInvited(int clientFd) { _invited.insert(clientFd); }

void Channel::removeInvited(int clientFd) { _invited.erase(clientFd); }

bool Channel::checkKey(std::string providedKey) const {
  if (!_key.empty() && providedKey != _key)
    return false;
  return true;
}

bool Channel::checkInvite(int clientFd) const {
  if (isInviteOnly() && !isInvited(clientFd))
    return false;
  return true;
}

bool Channel::checkLimit() const {
  if (_userLimit > 0 && _members.size() >= _userLimit)
    return false;
  return true;
}
