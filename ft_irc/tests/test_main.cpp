#include "../includes/client/client.hpp"
#include "../includes/command/command.hpp"
#include <cassert>
#include <iostream>

void testCommandSimple() {
  Command cmd("NICK test");
  assert(cmd.getName() == "NICK");
  assert(cmd.getArgs().size() == 1);
  assert(cmd.getArgs()[0] == "test");
  assert(cmd.getTrailing().empty());
  std::cout << "✓ Simple command (NICK test)" << std::endl;
}

void testCommandWithArgs() {
  Command cmd("PRIVMSG #channel :hello world");
  assert(cmd.getName() == "PRIVMSG");
  assert(cmd.getArgs().size() == 1);
  assert(cmd.getArgs()[0] == "#channel");
  assert(cmd.getTrailing() == "hello world");
  std::cout << "✓ Command with trailing (PRIVMSG #channel :hello world)"
            << std::endl;
}

void testCommandMultipleArgs() {
  Command cmd("MODE #channel +o user");
  assert(cmd.getName() == "MODE");
  assert(cmd.getArgs().size() == 2);
  assert(cmd.getArgs()[0] == "#channel");
  assert(cmd.getArgs()[1] == "+o");
  std::cout << "✓ Multiple args (MODE #channel +o user)" << std::endl;
}

void testCommandModeWithTrailing() {
  Command cmd("KICK #channel user :reason here");
  assert(cmd.getName() == "KICK");
  assert(cmd.getArgs().size() == 2);
  assert(cmd.getArgs()[0] == "#channel");
  assert(cmd.getArgs()[1] == "user");
  assert(cmd.getTrailing() == "reason here");
  std::cout << "✓ MODE with trailing (KICK #channel user :reason here)"
            << std::endl;
}

void testCommandEmpty() {
  Command cmd("");
  assert(!cmd.isValid());
  std::cout << "✓ Empty command rejected" << std::endl;
}

void testBufferExtraction() {
  Client client;
  client.setFd(5);

  client.appendBuffer("NICK test\r\n", 11);
  std::string req1 = client.extractRequest();
  assert(req1 == "NICK test");

  client.appendBuffer("PRIVMSG #c :hi\r\n", 16);
  std::string req2 = client.extractRequest();
  assert(req2 == "PRIVMSG #c :hi");

  std::string empty = client.extractRequest();
  assert(empty.empty());
  std::cout << "✓ Buffer extraction with \\r\\n" << std::endl;
}

void testBufferPartial() {
  Client client;
  client.setFd(5);

  client.appendBuffer("NICK te", 7);
  std::string req1 = client.extractRequest();
  assert(req1.empty());

  client.appendBuffer("st\r\n", 4);
  std::string req2 = client.extractRequest();
  assert(req2 == "NICK test");
  std::cout << "✓ Partial buffer handling" << std::endl;
}

void testCommandIntegration() {
  Client client;
  client.setFd(5);

  client.appendBuffer("MODE #channel +i\r\nUSER test\r\n", 31);

  std::string req1 = client.extractRequest();
  Command cmd1(req1);
  assert(cmd1.getName() == "MODE");
  assert(cmd1.getArgs()[0] == "#channel");
  assert(cmd1.getArgs()[1] == "+i");

  std::string req2 = client.extractRequest();
  Command cmd2(req2);
  assert(cmd2.getName() == "USER");
  assert(cmd2.getArgs()[0] == "test");

  std::cout << "✓ Integration: buffer → command parsing" << std::endl;
}

int main() {
  std::cout << "\n=== IRC Parser Tests ===\n" << std::endl;

  try {
    testCommandSimple();
    testCommandWithArgs();
    testCommandMultipleArgs();
    testCommandModeWithTrailing();
    testCommandEmpty();
    testBufferExtraction();
    testBufferPartial();
    testCommandIntegration();

    std::cout << "\n✅ All tests passed!\n" << std::endl;
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "\n❌ Test failed: " << e.what() << std::endl;
    return 1;
  }
}
