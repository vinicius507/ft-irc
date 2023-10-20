#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>

class Channel {
  public:
  Channel(std::string name);

  Channel(const Channel &other);

  ~Channel(void);

  Channel &operator=(const Channel &other);

  void addClient(Client *client);

  void removeClient(Client *client);

  bool hasClient(Client *client) const;

  const std::string &getName(void) const;

  void send(const std::string &message) const;

  private:
  const std::string _name;
  std::vector<Client *> _clients;
};

#endif