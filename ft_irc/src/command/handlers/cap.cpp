#include "../../../includes/command/command.hpp"

void Command::cap() {

  if (_args[0] == "LS") {
    _server->sendMessage(_caller, "CAP * LS :\r\n");
  } else if (_args[0] == "END") {
    welcomeUser(_caller);
  } else {
    return;
  }
}
