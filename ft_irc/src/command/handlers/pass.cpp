#include "../../../includes/command/command.hpp"

void Command::pass() {
  if (_caller->getIsRegistered()) {
    _server->sendMessage(_caller, errAlreadyRegistered());
    return;
  }
  if (_args.empty()) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }
  if (_server->getPassword() != _args[0]) {
    _server->sendMessage(_caller, errPasswdMismatch());
    _server->disconnectClient(_caller->getFd());
    return;
  }
  _caller->setLoggedIn(true);
}
