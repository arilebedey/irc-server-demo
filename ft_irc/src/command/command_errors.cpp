#include "../../includes/command/command.hpp"

// 462
std::string Command::errAlreadyRegistered() {
  return ":" + _server->getName() + " 462 " + _caller->getNick() +
         " :You may not reregister\r\n";
}

// 461
std::string Command::errNeedMoreParams() {
  return ":" + _server->getName() + " 461 " + _caller->getNick() + " " +
         this->_name + " :Not enough parameters\r\n";
}

// 464
std::string Command::errPasswdMismatch() {
  return ":" + _server->getName() + " 464 * :Password incorrect\r\n";
}

// 431
std::string Command::errNoNicknameGiven() {
  return ":" + _server->getName() + " 431 " + _caller->getNick() +
         " :No nickname given\r\n";
}

// 432
std::string Command::errErroneusNickname(std::string nick) {
  return ":" + _server->getName() + " 432 " + nick + " :Erroneus nickname\r\n";
}

// 433
std::string Command::errNicknameInUse(std::string nick) {
  std::string currentNick =
      _caller->getNick().empty() ? "*" : _caller->getNick();
  return ":" + _server->getName() + " 433 " + currentNick + " " + nick +
         " :Nickname is already in use\r\n";
}

// RPL_WELCODE(001)
std::string Command::infoSuccesConnexion() {
  return ":" + _server->getName() + " 001 " + _caller->getNick() +
         " :Welcome to the Internet Relay Network " + _caller->getIdentity() +
         "\r\n";
}

// 476 ERR_BADCHANMASK
std::string Command::errBadChannelMask(std::string name) {
  return ":" + _server->getName() + " 476 " + _caller->getNick() + " " + name +
         " :Bad channel mask\r\n";
}

// 467 ERR_KEYSET
std::string Command::errKeySet(std::string channel) {
  return ":" + _server->getName() + " 467 " + _caller->getNick() + " #" +
         channel + " :Channel key already set\r\n";
}

// 471 ERR_CHANNELISFULL
std::string Command::errChannelIsFull(std::string channel) {
  return ":" + _server->getName() + " 471 " + _caller->getNick() + " #" +
         channel + " :Cannot join channel (+l)\r\n";
}

// 473 ERR_INVITEONLYCHAN
std::string Command::errInviteOnlyChan(std::string channel) {
  return ":" + _server->getName() + " 473 " + _caller->getNick() + " #" +
         channel + " :Cannot join channel (+i)\r\n";
}

// 475 ERR_BADCHANNELKEY
std::string Command::errBadChannelKey(std::string channel) {
  return ":" + _server->getName() + " 475 " + _caller->getNick() + " #" +
         channel + " :Cannot join channel (+k)\r\n";
}

// 482 ERR_CHANOPRIVSNEEDED
std::string Command::errChanOpPrivsNeeded(std::string channel) {
  return ":" + _server->getName() + " 482 " + _caller->getNick() + " #" +
         channel + " :You're not channel operator\r\n";
}

// 403 ERR_NOSUCHCHANNEL
std::string Command::errNoSuchChannel(std::string channel) {
  return ":" + _server->getName() + " 403 " + _caller->getNick() + " #" +
         channel + " :No such channel\r\n";
}

// 442 ERR_NOTONCHANNEL
std::string Command::errNotOnChannel(std::string channel) {
  return ":" + _server->getName() + " 442 " + _caller->getNick() + " #" +
         channel + " :You're not on that channel\r\n";
}

// 443 ERR_USERONCHANNEL
std::string Command::errUserOnChannel(std::string channel) {
  return ":" + _server->getName() + " 443 " + _caller->getNick() + " #" +
         channel + " :is already on channel\r\n";
}

// 441 ERR_USERNOTINCHANNEL
std::string Command::errUserNotInChannel(std::string nick,
                                         std::string channel) {
  return ":" + _server->getName() + " 441 " + _caller->getNick() + " " + nick +
         " #" + channel + " :They aren't on that channel\r\n";
}

// 401 ERR_NOSUCHNICK
std::string Command::errNoSuchNick(std::string nick) {
  return ":" + _server->getName() + " 401 " + _caller->getNick() + " " + nick +
         " :No such nick/channel\r\n";
}

// 451 ERR_NOTREGISTERED
std::string Command::errNotRegistered() {
  return ":" + _server->getName() + " 451 " + _caller->getNick() +
         " :You have not registered\r\n";
}
