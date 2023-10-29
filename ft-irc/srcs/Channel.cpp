#include "Channel.hpp"
#include "serverReplies.hpp"

Channel::Channel(const std::string &name) : _name(name), _key(""), _operators(0) {}

Channel::Channel(const Channel &other) : _name(other._name) {}

Channel::~Channel() {}

Channel &Channel::operator=(const Channel &other) {
  if (this != &other) {
    const_cast<std::string &>(this->_name) = other._name;
    const_cast<std::string &>(this->_key) = other._key;
    const_cast<std::string &>(this->_topic) = other._topic;
    this->_clients = other._clients;
  }
  return *this;
}

void Channel::addClient(Client *client) {
  if (client == NULL) {
    return;
  }
  this->_clients.push_back(client);
  this->send(MSG_JOIN(client->getPrefix(), this->getName()));
  if (this->hasTopic()) {
    client->send(RPL_TOPIC(client->getNickname(), this->getName(), this->getTopic()));
  } else {
    client->send(RPL_NOTOPIC(client->getNickname(), this->getName()));
  }
  client->send(RPL_NAMREPLY(client->getNickname(), this->getName(), this->getClientsNicknames()));
  client->send(RPL_ENDOFNAMES(client->getNickname(), this->getName()));
}

void Channel::removeClient(Client *client) {
  std::vector<Client *>::iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (*it == client) {
      this->_clients.erase(it);
      if (this->isOperator(client)) {
        this->_operators.erase(it);
      }
      break;
    }
  }
}

bool Channel::hasClient(Client *client) const {
  std::vector<Client *>::const_iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (*it == client) {
      return (true);
    }
  }
  return (false);
}

const std::string &Channel::getName(void) const { return (this->_name); }

const std::string &Channel::getTopic(void) const { return (this->_topic); }

void Channel::setKey(const std::string &key) { const_cast<std::string &>(this->_key) = key; }

void Channel::setTopic(const std::string &topic) { const_cast<std::string &>(this->_topic) = topic; }

bool Channel::hasTopic() const { return (this->_topic.empty() == false); }

bool Channel::isKeyProtected() const { return (this->_key.empty() == false); }

bool Channel::isKeyValid(const std::string &key) const { return (this->_key == key); }

void Channel::addOperator(Client *client) {
  if (client == NULL) {
    return;
  }
  this->_operators.push_back(client);
}

bool Channel::isOperator(Client *client) const {
  std::vector<Client *>::const_iterator it;

  for (it = this->_operators.begin(); it != this->_operators.end(); it++) {
    if (*it == client) {
      return (true);
    }
  }
  return (false);
}

std::string Channel::getClientsNicknames(void) const {
  std::vector<Client *>::const_iterator it;
  std::string nicknames;

  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    nicknames += (*it)->getNickname() + " ";
  }
  return (nicknames);
}

void Channel::send(const std::string &message) const {
  std::vector<Client *>::const_iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    (*it)->send(message);
  }
}

void Channel::join(Client *client, const std::string &key) {
  if (client == NULL) {
    return;
  }
  if (this->isKeyProtected() && !this->isKeyValid(key)) {
    client->send(ERR_BADCHANNELKEY(client->getNickname(), this->getName()));
    return;
  }
  this->addClient(client);
}

unsigned int Channel::getClientsCount(void) const { return (this->_clients.size()); }

void Channel::sendToVisible(Client *client, const std::string &message) const {
  std::vector<Client *>::const_iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
    if (*it != client) {
      (*it)->send(message);
    }
  }
}
