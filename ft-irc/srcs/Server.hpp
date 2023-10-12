#ifndef SERVER_HPP
#define SERVER_HPP

#include "ClientsManager.hpp"
#include "Message.hpp"
#include "PollManager.hpp"
#include "ServerSocket.hpp"

class Server {
  public:
  Server(void);

  Server(short port, const std::string &connectionPassword);

  Server(const Server &other);

  ~Server(void);

  Server &operator=(const Server &other);

  bool run(void);

  void handleClientData(int clientFd);

  void handleMessage(Client *client, Message &msg);

  bool isConnectionPasswordValid(const std::string &connectionPassword) const;

  static void gracefulShutdown(int signal);

  static bool _shouldExit;

  private:
  PollManager _pollFds;
  ServerSocket _socket;
  ClientsManager _clients;
  const std::string _connectionPassword;
};

#endif
