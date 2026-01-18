#include "../../includes/command/command.hpp"
#include <iostream>
#include <sstream>

void Command::parse(std::string &request) {
  std::istringstream iss(request);

  if (!(iss >> _name)) {
    std::cerr << "Error: client sent empty message" << std::endl;
  }
}
