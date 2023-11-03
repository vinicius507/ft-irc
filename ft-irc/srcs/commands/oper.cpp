
#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"

// /oper <user> <password>

void operCommand(Server &server, Client *client, Message &msg) {
  if (client->getAuthState() != AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }

  if (msg.params.size() < 2) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), msg.command));
    return;
  }

  if (server.isOper(client)) {
    return;
  }

  if (!server.verifyOper(msg.params.at(0), msg.params.at(1))) {
    client->send(ERR_PASSWSMISMATCH(client->getNickname()));
    return;
  }

  if (server.verifyOperHost(client->getHostname())) {
    client->send(ERR_NOOPERHOST(client->getNickname()));
    return;
  }

  server.addOper(client);
  client->send(RPL_YOUREOPER(client->getNickname()));
}
