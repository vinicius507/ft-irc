#ifndef SERVER_HPP
#define SERVER_HPP

#include "ServerSocket.hpp"
#include <sys/poll.h>
#include <vector>
#include <cstdlib>

class Server {
public:
  Server(void);

  Server(short port);

  Server(const Server &other);

  ~Server(void);

  Server &operator=(const Server &other);

  bool run(void);

  void handleClientData(int fd);

  void removeClient(int clientFd);

private:
  std::vector<struct pollfd> _pollFds;
  ServerSocket _socket;
};

#endif
