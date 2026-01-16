#include "../includes/server/server.hpp"
#include <iostream>
#include <stdlib.h>

int main(int ac, char **av) {
  Server ser;
  std::cout << "----- IRC SERVER -----" << std::endl;

	if (ac != 3)
		return (std::cerr << "ERROR" << std::endl, 1);

  try {
    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, Server::signalHandler);
    ser.servInit(atoi(av[1]), av[2]);
  } catch (const std::exception &e) {
    // ser.closeFds();
    std::cerr << e.what() << std::endl;
  }
  std::cout << "Server removed!" << std::endl;
}
