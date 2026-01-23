#include "../../includes/command/command.hpp"
#include <iostream>
#include <sstream>

Command::Command(Server *server, Client *caller, const std::string &req)
    : _server(server), _caller(caller) {
  parse(req);
  handle();
}

void Command::parse(const std::string &req) {
  if (req.find("\n") != std::string::npos) {
    std::cerr << "Invalid character in command: newline" << std::endl;
    _name = "";
    return;
  }

  std::string token;
  std::istringstream iss(req);

  if (!(iss >> token) || token.empty()) {
    return;
  }

  _name = token;

  size_t colonPos = req.find(":");
  if (colonPos != std::string::npos) {
    while (iss >> token && token[0] != ':') // args are until ':'
      _args.push_back(token);
    _trailing = req.substr(colonPos + 1);
  } else {
    while (iss >> token) // no trailing, just args
      _args.push_back(token);
  }
}

void Command::handle() {
  if (_name == "PASS")
    pass();
  else if (_name == "CAP")
    cap();
  else if (_name == "USER")
    user();
  else if (_name == "NICK")
    nick();
  else if (_name == "JOIN")
    join();
  else if (_name == "PART")
    part();
  else if (_name == "PING")
    ping();
  else if (_name == "PRIVMSG")
    privmsg();
  else if (_name == "MODE")
    mode();
}

void Command::debug_print() {
  std::cout << "=== DEBUG ==="
            << "\n";
  std::cout << "Command -> " << this->_name << "\n";
  std::cout << "Arguments ->";
  for (unsigned long int i = 0; i < _args.size(); i++) {
    std::cout << " " << _args[i];
  }
  std::cout << "\n";
  std::cout << "Trailings ->" << this->_trailing << "\n";
  std::cout << "==== END ====" << std::endl;
}
