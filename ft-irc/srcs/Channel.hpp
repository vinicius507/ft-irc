#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>

class Channel {
  public:
  Channel(const std::string &name);

  Channel(const Channel &other);

  ~Channel(void);

  Channel &operator=(const Channel &other);

  void addClient(Client *client);

  void removeClient(Client *client);

  bool hasClient(Client *client) const;

  const std::string &getName(void) const;

  void setKey(const std::string &key);

  const std::string &getTopic(void) const;

  void setTopic(const std::string &topic);

  bool hasTopic(void) const;

  bool isKeyProtected(void) const;

  bool isKeyValid(const std::string &key) const;

  void send(const std::string &message) const;

  void join(Client *client, const std::string &key);

  std::string getClientsNicknames(void) const;

  private:
  const std::string _name;
  const std::string _key;
  const std::string _topic;
  std::vector<Client *> _clients;
};

#endif