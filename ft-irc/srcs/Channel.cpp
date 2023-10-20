#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name) {}

Channel::Channel(const Channel &other) : _name(other._name) {}

Channel::~Channel() {}

Channel &Channel::operator=(const Channel &other) {
  if (this != &other) {
    _name = other._name;
  }
  return *this;
}

void Channel::addClient(Client *client) {
  if (client != NULL) {
    this->_clients.push_back(client);
  }
}

void Channel::removeClient(Client *client) {
  std::vector<Client *>::iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (*it == client) {
      this->_clients.erase(it);
      break;
    }
  }
}

bool Channel::hasClient(Client *client) {
  std::vector<Client *>::iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (*it == client) {
      return (true);
    }
  }
  return (false);
}

const std::string &Channel::getName() const { return this->_name; }
