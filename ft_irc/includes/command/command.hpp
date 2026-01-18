#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

class Command {
private:
  std::string _name;
  std::vector<std::string> _args;
  std::string _trailing;

  void parse(const std::string &req);

public:
  Command();
  Command(const std::string &req);
  ~Command();

  std::string getName() const { return _name; }
  std::vector<std::string> getArgs() const { return _args; }
  std::string getTrailing() const { return _trailing; }
  bool isValid() const { return !_name.empty(); }
};

#endif
