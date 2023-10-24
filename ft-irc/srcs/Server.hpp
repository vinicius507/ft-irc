#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
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

  Client *getClientByNickname(const std::string &nickname);

  static void gracefulShutdown(int signal);

  Channel *getChannel(std::string &channelName);

  void createChannel(const std::string &channelName, const std::string &key, Client *client);

  static bool _shouldExit;

  private:
  PollManager _pollFds;
  ServerSocket _socket;
  ClientsManager _clients;
  std::map<std::string, Channel *> _channels;
  const std::string _connectionPassword;
};

#endif
