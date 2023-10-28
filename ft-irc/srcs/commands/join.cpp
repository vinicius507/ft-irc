#include "../commands.hpp"

#include "../Channel.hpp"
#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"
#include "utils.hpp"

void joinCommand(Server &server, Client *client, Message &msg) {
  Channel *channel;
  bool hasKeysParam = false;
  std::string channelName, key;
  std::vector<std::string>::size_type i;
  std::vector<std::string> channelNames, keys;

  if (client->getAuthState() < AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (msg.params.empty()) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), msg.command));
    return;
  }
  channelNames = splitByComma(msg.params[0]);
  hasKeysParam = (msg.params.size() > 1);
  if (hasKeysParam) {
    keys = splitByComma(msg.params[1]);
  }
  i = 0;
  while (i < channelNames.size()) {
    channelName = channelNames[i];
    key = "";
    if (hasKeysParam && i < keys.size()) {
      key = keys[i];
    }
    if (isValidChannelName(channelName) == false) {
      client->send(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
      ++i;
      continue;
    }
    channel = server.getChannel(channelName);
    if (channel == NULL) {
      channel = server.createChannel(channelName, key, client);
      channel->addOperator(client);
      
    } else {
      channel->join(client, key);
    }
    ++i;
  }
}