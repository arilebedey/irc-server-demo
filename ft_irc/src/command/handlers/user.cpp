#include "../../../includes/command/command.hpp"

void Command::user() {
  if (!_caller->getLoggedIn())
    return; // There might be a proper err code
  if (_args.size() != 3 && _args.size() != 4)
    return;
  if (_trailing.empty())
    return;
  _caller->setUser(_args[0]);
  _caller->setReal(_trailing);
  welcomeUser(_caller);
}
