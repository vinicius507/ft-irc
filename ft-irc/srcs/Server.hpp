#ifndef SERVER_HPP
#define SERVER_HPP

#include "PollManager.hpp"
#include "ServerSocket.hpp"
#include <cstdlib>
#include <sys/poll.h>
#include <vector>

class Server {
public:
  Server(void);

  Server(short port);

  Server(const Server &other);

  ~Server(void);

  Server &operator=(const Server &other);

  bool run(void);

  void handleClientData(int fd);

private:
  PollManager _pollFds;
  ServerSocket _socket;
};

#endif
