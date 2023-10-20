#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../numericReplies.hpp"

void passCommand(Server &server, Client *client, Message &msg) {
  std::string connectionPassword;

  if (client->getAuthState() == AuthDone) {
    client->send(ERR_ALREADYREGISTRED(client->getNickname()));
    return;
  }
  if (msg.params.size() < 1) {
    client->send(ERR_NEEDMOREPARAMS("*", msg.command));
    return;
  }
  connectionPassword = msg.params.at(0);
  if (server.isConnectionPasswordValid(connectionPassword)) {
    client->setAuthState(AuthPass);
  } else {
    client->send(ERR_PASSWSMISMATCH("*"));
  }
}
