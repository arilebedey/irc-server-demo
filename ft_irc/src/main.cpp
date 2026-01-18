#include "../includes/server/server.hpp"
#include <iostream>
#include <stdlib.h>

int main(int ac, char **av) {
  Server server;
  std::cout << "----- IRC SERVER -----" << std::endl;

  if (ac != 3)
    return (std::cerr << "Error: wrong number of args" << std::endl, 1);

  try {
    server.servInit(atoi(av[1]), av[2]);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  std::cout << "Server removed!" << std::endl;
  return 0;
}
