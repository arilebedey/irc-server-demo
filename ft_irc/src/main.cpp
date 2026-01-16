#include "../includes/server/server.hpp"
#include <iostream>

int main() {
  Server ser;
  std::cout << "----- IRC SERVER -----" << std::endl;
  try {
    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, Server::signalHandler);
    ser.servInit();
  } catch (const std::exception &e) {
    ser.closeFds();
    std::cerr << e.what() << std::endl;
  }
  std::cout << "Server removed!" << std::endl;
}
