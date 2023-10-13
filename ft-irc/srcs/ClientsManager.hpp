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

  Client *operator[](int clientFd);

  const Client *operator[](int clientFd) const;

  typedef std::map<int, Client *>::iterator iterator;

  void addClient(int clientFd);

  void disconnectClient(int clientFd);

  void disconnectAllClients(void);

  iterator begin(void);

  iterator end(void);

  private:
  std::map<int, Client *> _clients;
};

#endif
