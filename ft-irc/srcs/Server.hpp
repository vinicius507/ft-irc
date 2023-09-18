#ifndef SERVER_HPP
#define SERVER_HPP

#include "PollManager.hpp"
#include "ServerSocket.hpp"
#include <csignal>
#include <cstdlib>

class Server
{
  public:
    Server(void);
    Server(short port);
    Server(const Server &other);
    ~Server(void);
    Server &operator=(const Server &other);

    bool run(void);
    void handleClientData(int fd);

    struct sigaction mask;
    static void gracefulShutdown(int signal);
    static bool _shouldExit;

  private:
    PollManager _pollFds;
    ServerSocket _socket;
};

#endif
