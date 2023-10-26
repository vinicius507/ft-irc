#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"

void quitCommand(Server &server, Client *client, Message &message) {
  std::string quitMessage;

  if (client->getAuthState() < AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (message.params.empty()) {
    quitMessage = client->getNickname() + " has quit";
  } else {
    quitMessage = client->getNickname() + " has quit: " + message.params[0] + ".";
  }
  server.sendToVisible(MSG_QUIT(client->getNickname(), quitMessage));
  client->quitSent();
}