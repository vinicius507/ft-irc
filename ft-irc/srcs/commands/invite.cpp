#include "../commands.hpp"

#include "../Channel.hpp"
#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"
#include "utils.hpp"

void inviteCommand(Server &server, Client *client, Message &msg) {
  Client *invited;
  Channel *channel;
  std::string channelName;

  if (client->getAuthState() < AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (msg.params.size() < 2) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), msg.command));
    return;
  }
  channelName = msg.params[1];
  channel = server.getChannel(channelName);
  if (channel == NULL) {
    client->send(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
    return;
  }
  if (channel->hasClient(client) == false) {
    client->send(ERR_NOTONCHANNEL(client->getNickname(), channelName));
    return;
  }
  if (channel->isInviteOnly() && server.isOper(client) == false && channel->isOperator(client) == false) {
    client->send(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
    return;
  }
  invited = server.getClientByNickname(msg.params[0]);
  if (invited == NULL) {
    client->send(ERR_NOSUCHNICK(client->getNickname(), msg.params[0]));
    return;
  }
  if (channel->hasClient(invited)) {
    client->send(ERR_USERONCHANNEL(client->getNickname(), invited->getNickname(), channelName));
    return;
  }
  channel->addGuest(invited);
  client->send(RPL_INVITING(channelName, client->getNickname()));
  invited->send(MSG_INVITE(client->getNickname(), invited->getNickname(), channelName));
}
