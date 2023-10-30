#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>

#define CHANNELMODES_CHARS std::string("ti")

enum ChannelModeFlags {
  CHANMODES_NONE = 0,
  CHANMODES_TOPIC = 1 << 0,
  CHANMODES_INVITE = 1 << 1,
};

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

  unsigned int getClientsCount(void) const;

  void addOperator(Client *client);

  void addGuest(Client *client);

  bool isGuest(Client *client) const;

  void removeGuest(Client *client);

  bool isOperator(Client *client) const;

  void removeOperator(Client *client);

  void sendToVisible(Client *client, const std::string &message) const;

  void setRestrictTopic(bool restrictTopic);

  bool isTopicRestricted(void) const;

  void setInviteOnly(bool inviteOnly);

  bool isInviteOnly(void) const;

  std::string getModes(void) const;

  private:
  const std::string _name;
  const std::string _key;
  ChannelModeFlags _modes;
  const std::string _topic;
  std::vector<Client *> _clients;
  std::vector<Client *> _operators;
  std::vector<Client *> _guests;
};

#endif
