#ifndef CLIENTSMANAGER_HPP
#define CLIENTSMANAGER_HPP

#include "Client.hpp"
#include <map>

class ClientsManager {
  public:
  ClientsManager(void);

  ClientsManager(const ClientsManager &other);

  ~ClientsManager(void);

  ClientsManager &operator=(const ClientsManager &other);

  // Client &operator[](int clientFd);

  // const Client &operator[](int clientFd) const;

  void addClient(int clientFd);

  void disconnectClient(int clientFd);

  void disconnectAllClients(void);

  Client *getClient(int clientFd);

  private:
  std::map<int, Client *> _clients;
};

#endif
