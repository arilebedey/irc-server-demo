#include "../client/client.hpp"
#include <vector>

class Server {
private:
  int _port;
  int _servSocketFd;
  static bool _signal;
  std::vector<Client> _clients;
  std::vector<struct pollfd> _fds;

public:
  void servInit();
  void servSocket();
  void acceptNewClient();
  void receiveNewData(int fd);

  static void signalHandler(int signum);

  void closeFds();
  void clearClients(int fd);
};
