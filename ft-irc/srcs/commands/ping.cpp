#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"

void pingCommand(Server &server, Client *client, Message &msg) {
  client->send(MSG_PONG);
  (void)server;
  (void)msg;
}
