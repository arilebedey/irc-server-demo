#include "../../../includes/command/command.hpp"

void Command::nick() {
  if (!_caller->getLoggedIn()) {
    return;
  }
  if (_args.empty()) {
    _server->sendMessage(_caller, errNoNicknameGiven());
    return;
  }
  if (/*check if the username is using valid format.*/ false) {
    _server->sendMessage(_caller, errErroneusNickname(_args[0]));
    return;
  }
  if (_server->isNickTaken(_args[0])) {
    _server->sendMessage(_caller, errNicknameInUse(_args[0]));
    return;
  }
  if (!_caller->getIsRegistered()) {
    _caller->setNick(_args[0]);
    welcomeUser(_caller);
  } else {
    // TODO : send this message to every channel/privmessage to make them
    // acknowledge the nick edit.
    _server->sendMessage(_caller,
                         ":" + _caller->getNick() + " NICK :" + _args[0]);
    _caller->setNick(_args[0]);
  }
}
