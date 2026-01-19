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
  std::cout << "✓ testCommandSimple (NICK test)" << std::endl;
}

void testCommandWithArgs() {
  Command cmd("PRIVMSG #channel :hello world");
  assert(cmd.getName() == "PRIVMSG");
  assert(cmd.getArgs().size() == 1);
  assert(cmd.getArgs()[0] == "#channel");
  assert(cmd.getTrailing() == "hello world");
  std::cout << "✓ testCommandWithArgs (PRIVMSG #channel :hello world)"
            << std::endl;
}

void testCommandMultipleArgs() {
  Command cmd("MODE #channel +o user");
  assert(cmd.getName() == "MODE");
  assert(cmd.getArgs().size() == 3);
  assert(cmd.getArgs()[0] == "#channel");
  assert(cmd.getArgs()[1] == "+o");
  assert(cmd.getArgs()[2] == "user");
  std::cout << "✓ testCommandMultipleArgs (MODE #channel +o user)" << std::endl;
}

void testCommandModeWithTrailing() {
  Command cmd("KICK #channel user :reason here");
  assert(cmd.getName() == "KICK");
  assert(cmd.getArgs().size() == 2);
  assert(cmd.getArgs()[0] == "#channel");
  assert(cmd.getArgs()[1] == "user");
  assert(cmd.getTrailing() == "reason here");
  std::cout << "✓ testCommandModeWithTrailing (KICK #channel user :reason here)"
            << std::endl;
}

void testCommandEmpty() {
  Command cmd("");
  assert(!cmd.isValid());
  std::cout << "✓ testCommandEmpty" << std::endl;
}

void testMalformedGluedColon() {
  Command cmd("PRIVMSG #channel:hello world");
  assert(cmd.getName() == "PRIVMSG");
  assert(cmd.getArgs().size() == 2);
  assert(cmd.getArgs()[0] == "#channel:hello");
  assert(cmd.getArgs()[1] == "world");
  std::cout << "✓ testGluedColon (PRIVMSG #channel:hello world)" << std::endl;
}

/* Buffer Extraction Tests */

void testBufferExtraction() {
  Client client;

  client.appendBuffer("NICK test\r\n", 11);
  std::string req1 = client.extractLine();
  assert(req1 == "NICK test");

  client.appendBuffer("PRIVMSG #c :hi\r\n", 16);
  std::string req2 = client.extractLine();
  assert(req2 == "PRIVMSG #c :hi");
}

void testBufferPartial() {
  Client client;

  client.appendBuffer("NICK te", 7);
  std::string req1 = client.extractLine();
  assert(req1.empty());

  client.appendBuffer("st\r\n", 4);
  std::string req2 = client.extractLine();
  assert(req2 == "NICK test");
  std::cout << "✓ testBufferPartial" << std::endl;
}

void testCommandIntegration() {
  Client client;

  client.appendBuffer("MODE #channel +i\r\nUSER test\r\n", 31);

  std::string req1 = client.extractLine();
  Command cmd1(req1);
  assert(cmd1.getName() == "MODE");
  assert(cmd1.getArgs()[0] == "#channel");
  assert(cmd1.getArgs()[1] == "+i");

  std::string req2 = client.extractLine();
  Command cmd2(req2);
  assert(cmd2.getName() == "USER");
  assert(cmd2.getArgs()[0] == "test");

  std::cout
      << "✓ testCommandIntegration (MODE #channel +i\\r\\nUSER test\\r\\n)"
      << std::endl;
}

int main() {
  std::cout << "\n=== IRC Tests ===" << std::endl;

  try {
    std::cout << "\n=== Parser Tests ===\n" << std::endl;
    testCommandSimple();
    testCommandWithArgs();
    testCommandMultipleArgs();
    testCommandModeWithTrailing();
    testCommandEmpty();
    testMalformedGluedColon();

    std::cout << "\n=== appendBuffer & extractLine Tests ===\n" << std::endl;
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
