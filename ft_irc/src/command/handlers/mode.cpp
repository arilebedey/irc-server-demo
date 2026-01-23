#include "../../../includes/command/command.hpp"

void Command::mode() {
  if (!_caller->getIsRegistered())
    return;

  if (_args.empty()) {
    _server->sendMessage(_caller, errNeedMoreParams());
    return;
  }
}
