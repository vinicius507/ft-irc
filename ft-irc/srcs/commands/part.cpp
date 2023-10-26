#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../numericReplies.hpp"
#include "utils.hpp"

void partCommand(Server &server, Client *client, Message &message) {
  Channel *channel;
  std::string channelName;
  std::vector<std::string>::size_type i;
  std::vector<std::string> channelNames;

  if (client->getAuthState() < AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (message.params.empty()) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), message.command));
    return;
  }
  channelNames = splitByComma(message.params[0]);
  i = 0;
  while (i < channelNames.size()) {
    channelName = channelNames[i++];
    channel = server.getChannel(channelName);
    if (channel == NULL) {
      client->send(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
      continue;
    }
    if (channel->hasClient(client) == false) {
      client->send(ERR_NOTONCHANNEL(client->getNickname(), channelName));
      continue;
    }
    channel->send(":" + client->getNickname() + " PART " + channel->getName() + "\r\n");
    channel->removeClient(client);
  }
}
