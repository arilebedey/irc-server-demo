#include "../client/client.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Server {
private:
  int _port;
  int _servSocketFd;
  static bool _signal;
  std::vector<Client> _clients;
  std::vector<struct pollfd> _fds;

public:
  Server();
  ~Server();

  void servInit(int port);
  void servSocket();
  void acceptNewClient();
  void receiveNewData(int fd);

  static void signalHandler(int signum);

  void closeFds();
  void clearClients(int fd);
};
