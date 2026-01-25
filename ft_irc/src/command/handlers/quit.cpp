#include "../../../includes/command/command.hpp"

void Command::quit() {
  if (!_caller->getIsRegistered()) {
    _server->disconnectClient(_caller->getFd());
    return;
  }

  std::string reason = _trailing.empty() ? "Client quit" : _trailing;
  _server->disconnectClient(_caller->getFd(), reason);
}
