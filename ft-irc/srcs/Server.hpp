#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "ClientsManager.hpp"
#include "Message.hpp"
#include "PollManager.hpp"
#include "ServerSocket.hpp"
#include <vector>

#define OPER_PASS "password"
#define OPER_NAME "name"
#define OPER_HOST "locahost"

class Server {
  public:
  Server(void);

  Server(short port, const std::string &connectionPassword);

  Server(const Server &other);

  ~Server(void);

  Server &operator=(const Server &other);

  bool run(void);

  void handleNewClient(int clientFd);

  void handleClientData(int clientFd);

  void disconnectClient(Client *client);

  void handleMessage(Client *client, Message &msg);

  bool isConnectionPasswordValid(const std::string &connectionPassword) const;

  Client *getClientByNickname(const std::string &nickname);

  static void gracefulShutdown(int signal);

  Channel *getChannel(std::string &channelName);

  std::vector<Channel *> getChannelsWithClient(Client *client) const;

  Channel *createChannel(const std::string &channelName, const std::string &key, Client *client);

  void removeChannel(Channel *channel);

  void sendToVisible(Client *client, const std::string &message);

  bool verifyOper(const std::string &name, const std::string &password) const;

  bool verifyOperHost(const std::string &host) const;
  
  void addOper(Client *client);

  void removeOper(Client *client);

  bool isOper(Client *client) const;

  static bool _shouldExit;

  private:
  PollManager _pollFds;
  ServerSocket _socket;
  ClientsManager _clients;
  std::map<std::string, Channel *> _channels;
  const std::string _connectionPassword;
  std::vector<Client *> _operators;
};

#endif
