#include "../commands.hpp"

#include "../Channel.hpp"
#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"
#include "utils.hpp"

void kickCommand(Server &server, Client *client, Message &message) {
  Channel *channel;
  Client *target;
  std::string channelName, targetNickname, reason;
  std::vector<std::string>::size_type i;
  std::vector<std::string> channelNames, targetNicknames;

  if (client->getAuthState() < AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (message.params.size() < 2) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), message.command));
    return;
  }
  channelNames = splitByComma(message.params[0]);
  targetNicknames = splitByComma(message.params[1]);
  if (message.params.size() > 2) {
    reason = message.params[2];
  }
  i = 0;
  while (i < channelNames.size()) {
    channelName = channelNames[i];
    targetNickname = targetNicknames[i];
    channel = server.getChannel(channelName);
    if (channel == NULL) {
      client->send(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
      ++i;
      continue;
    }
    if (channel->hasClient(client) == false) {
      client->send(ERR_NOTONCHANNEL(client->getNickname(), channelName));
      ++i;
      continue;
    }
    if (channel->isOperator(client) == false) {
      client->send(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
      ++i;
      continue;
    }
    target = server.getClientByNickname(targetNickname);
    if (channel->hasClient(target) == false) {
      client->send(ERR_NOTONCHANNEL(targetNickname, channelName));
      ++i;
      continue;
    }
    channel->send(MSG_KICK(client->getPrefix(), channel->getName(), targetNickname, reason));
    channel->removeClient(target);
    i++;
  }
}