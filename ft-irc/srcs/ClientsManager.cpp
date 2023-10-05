#include "ClientsManager.hpp"

ClientsManager::ClientsManager(void) {}

ClientsManager::ClientsManager(const ClientsManager &other) : _clients(other._clients) {}

ClientsManager::~ClientsManager(void) { this->disconnectAllClients(); }

ClientsManager &ClientsManager::operator=(const ClientsManager &other) {
  if (this != &other) {
    this->disconnectAllClients();
    this->_clients = other._clients;
  }
  return (*this);
}

Client *ClientsManager::operator[](int clientFd) { return (this->_clients[clientFd]); }

const Client *ClientsManager::operator[](int clientFd) const { return (this->_clients.at(clientFd)); }

void ClientsManager::addClient(int clientFd) {
  Client *newClient = new Client(clientFd);
  this->_clients[clientFd] = newClient;
}

void ClientsManager::disconnectClient(int clientFd) {
  std::map<int, Client *>::iterator it = this->_clients.find(clientFd);

  if (it != this->_clients.end()) {
    delete it->second;
    this->_clients.erase(it);
  }
}

void ClientsManager::disconnectAllClients(void) { this->_clients.clear(); }
