#include "../../../includes/command/command.hpp"

void Command::ping() {
  if (_args.empty()) {
    _server->sendMessage(_caller, Command::errNeedMoreParams());
    return;
  }

  std::string response = ":" + _server->getName() + " PONG " +
                         _server->getName() + " :" + _args[0] + "\r\n";
  _server->sendMessage(_caller, response);
}
