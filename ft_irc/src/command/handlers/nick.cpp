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
    std::string prefix =
        ":" + _caller->getNick() + "!" + _caller->getUser() + "@127.0.0.1";
    std::string validation_message = prefix + " NICK " + _args[0] + "\r\n";
    _server->sendToVisible(_caller, validation_message);
    _caller->setNick(_args[0]);
  }
}
