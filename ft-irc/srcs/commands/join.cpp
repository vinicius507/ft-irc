#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../numericReplies.hpp"

void joinCommand(Server &server, Client *client, Message &msg) {
  std::vector<std::string> channels;

  if (client->getAuthState() < AuthDone) {
    return; // ERR_NOTREGISTERED?
  }
  if (msg.params.empty()) {
    return; // ERR_NEEDMOREPARAMS?
  }
}