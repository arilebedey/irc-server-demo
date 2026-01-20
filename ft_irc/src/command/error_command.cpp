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
  return ":" + _server->getName() + " 433 " + nick +
         " :Nickname is already in use\r\n";
}

// RPL_WELCODE(001)
std::string Command::infoSuccesConnexion() {
        return ":" + _server->getName() + " 001 " + _caller->getNick() +
                       " :Welcome to the Internet Relay Network " +
                       _caller->getNick() + "!" + _caller->getUser() +
                       "@127.0.0.1";
}
