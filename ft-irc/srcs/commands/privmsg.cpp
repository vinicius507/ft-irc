#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"

void privmsgCommand(Server &server, Client *client, Message &msg) {
  std::string target;

  if (client->getAuthState() < AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (msg.params.empty() || msg.params.at(0).empty()) {
    client->send(ERR_NORECIPIENT(client->getNickname(), "PRIVMSG"));
    return;
  }
  if (msg.trailingParam.empty()) {
    client->send(ERR_NOTEXTTOSEND(client->getNickname()));
  }
  target = msg.params.at(0);
  if (target.at(0) == '#') {
    Channel *channel = server.getChannel(target);
    if (channel == NULL) {
      client->send(ERR_NOSUCHCHANNEL(client->getNickname(), target));
      return;
    }
    channel->send(MSG_PRIVMSG(client->getPrefix(), target, msg.trailingParam));
    return;
  }
  Client *recipient = server.getClientByNickname(target);
  if (recipient == NULL) {
    client->send(ERR_NOSUCHNICK(client->getNickname(), target));
    return;
  }
  recipient->send(MSG_PRIVMSG(client->getPrefix(), target, msg.trailingParam));
}