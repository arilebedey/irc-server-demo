#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include <string>

class Channel {
private:
  std::string _name;
  std::string _topic;
  std::string _key;
  size_t _userLimit;

  // set: uniqueness, int: client fd
  std::set<int> _members;
  std::set<int> _operators;
  std::set<int> _invited;

  bool _inviteOnly;
  bool _topicCmdRestricted;

public:
  Channel();
  Channel(const std::string &name);
  ~Channel();

  std::string getName() const { return _name; }
  std::string getTopic() const { return _topic; }
  std::string getKey() const { return _key; }
  size_t getUserLimit() const { return _userLimit; }

  bool isInviteOnly() const { return _inviteOnly; }
  bool isTopicCmdRestricted() const { return _topicCmdRestricted; }

  bool isMember(int clientFd) const;
  bool isOperator(int clientFd) const;
  bool isInvited(int clientFd) const;

  void setTopic(const std::string &topic) { _topic = topic; }
  void setKey(const std::string &key) { _key = key; }
  void setUserLimit(size_t limit) { _userLimit = limit; }
  void setInviteOnly(bool status) { _inviteOnly = status; }
  void setTopicCmdRestricted(bool status) { _topicCmdRestricted = status; }

  void addMember(int clientFd);
  void removeMember(int clientFd);
  void addOperator(int clientFd);
  void removeOperator(int clientFd);
  void addInvited(int clientFd);
  void removeInvited(int clientFd);

  /*
    getMemberCount needed for:
    - Checking if channel is empty → delete it after PART/KICK
    - Making first joiner an operator (auto-op the creator)
    - MODE command to enforce user limit
  */
  size_t getMemberCount() const { return _members.size(); }

  /*
    getMembers needed for:
    - Finding target user by nickname in KICK, INVITE, MODE
    - Iterating members to broadcast messages
    - Checking if a user is in the channel for various validations
  */
  std::set<int> getMembers() const { return _members; }

  bool checkKey(std::string providedKey) const;
  bool checkLimit() const;
  bool checkInvite(int clientFd) const;
};

#endif
