#include "../../../includes/command/command.hpp"
#include <iostream>

void Command::mode() {
  if (!_caller->getIsRegistered())
    return;

  if (_args.empty()) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }
}
