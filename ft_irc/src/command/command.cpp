#include "../../includes/command/command.hpp"
#include <iostream>
#include <sstream>

Command::Command(const std::string &req) { parse(req); }

void Command::parse(const std::string &req) {
  if (req.find("\n") != std::string::npos) {
    std::cerr << "Invalid character in command: newline" << std::endl;
    _name = "";
    return;
  }

  std::string token;
  std::istringstream iss(req);

  if (!(iss >> token) || token.empty()) {
    std::cerr << "Error: client sent empty message" << std::endl;
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

// Test for
//
// KICK #channel user1 :
// PRIVMSG #channel:hello world
