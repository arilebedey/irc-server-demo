#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

class Command {
private:
  std::string _name;
  std::vector<std::string> _args;
  std::string _trailing;

  void parse(std::string &request);

public:
};

#endif
