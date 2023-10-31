#include "Channel.hpp"
#include "serverReplies.hpp"

#include <algorithm>

Channel::Channel(const std::string &name) : _name(name), _modes(CHANMODES_NONE) {}

Channel::Channel(const Channel &other) : _name(other._name), _modes(CHANMODES_NONE) {}

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
        this->removeOperator(client);
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

Client *Channel::getClient(const std::string &nickname) const {
  std::vector<Client *>::const_iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
    if ((*it)->getNickname() == nickname) {
      return (*it);
    }
  }
  return (NULL);
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
  if (this->isOperator(client)) {
    return;
  }
  this->_operators.push_back(client);
}

void Channel::removeOperator(Client *client) {
  std::vector<Client *>::iterator it, begin, end;

  if (client == NULL) {
    return;
  }
  begin = this->_operators.begin();
  end = this->_operators.end();
  it = std::find(begin, end, client);
  if (it != end) {
    this->_operators.erase(it);
  }
}

void Channel::addGuest(Client *client) {
  if (client == NULL) {
    return;
  }
  this->_guests.push_back(client);
}

void Channel::removeGuest(Client *client) {
  std::vector<Client *>::iterator it;

  if (client == NULL) {
    return;
  }
  it = std::find(this->_guests.begin(), this->_guests.end(), client);
  if (it != this->_guests.end()) {
    this->_guests.erase(it);
  }
}

bool Channel::isGuest(Client *client) const {
  std::vector<Client *>::const_iterator begin, end;

  if (client == NULL) {
    return (false);
  }
  begin = this->_guests.begin();
  end = this->_guests.end();
  if (std::find(begin, end, client) == end) {
    return (false);
  }
  return (true);
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
    if (this->isOperator(*it)) {
      nicknames += "@";
    }
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
  if (this->hasClient(client)) {
    return;
  }
  if (this->isGuest(client)) {
    this->addClient(client);
    this->removeGuest(client);
    return;
  }
  if (this->isInviteOnly()) {
    client->send(ERR_INVITEONLYCHAN(client->getNickname(), this->getName()));
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

void Channel::setRestrictTopic(bool restrictTopic) {
  int modes = this->_modes;

  if (restrictTopic) {
    modes |= CHANMODES_TOPIC;
  } else if ((modes & CHANMODES_TOPIC) != 0) {
    modes ^= CHANMODES_TOPIC;
  }
  this->_modes = static_cast<ChannelModeFlags>(modes);
}

bool Channel::isTopicRestricted(void) const { return ((this->_modes & CHANMODES_TOPIC) != 0); }

void Channel::setInviteOnly(bool inviteOnly) {
  int modes = this->_modes;

  if (inviteOnly) {
    modes |= CHANMODES_INVITE;
  } else if ((modes & CHANMODES_INVITE) != 0) {
    modes ^= CHANMODES_INVITE;
  }
  this->_modes = static_cast<ChannelModeFlags>(modes);
}

bool Channel::isInviteOnly(void) const { return ((this->_modes & CHANMODES_INVITE) != 0); }

std::string Channel::getModes(void) const {
  std::string modes, modeParams;

  if (this->isTopicRestricted()) {
    modes += "t";
  }
  if (this->isInviteOnly()) {
    modes += "i";
  }
  if (this->isKeyProtected()) {
    modes += "k";
    modeParams += this->_key;
  }
  if (modeParams.empty()) {
    return (modes);
  }
  return (std::string(modes + " " + modeParams));
}
