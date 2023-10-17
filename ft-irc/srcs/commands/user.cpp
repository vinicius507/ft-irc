#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../numericReplies.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

static std::string getClientHostname(Client *client) {
  socklen_t sockaddr_len;
  struct sockaddr_in sockaddr;
  std::string hostname;

  sockaddr_len = sizeof(sockaddr);
  getpeername(client->getFd(), reinterpret_cast<struct sockaddr *>(&sockaddr), &sockaddr_len);

  hostname = inet_ntoa(sockaddr.sin_addr);
  return (hostname);
}

void userCommand(Server &server, Client *client, Message &msg) {
  (void)server;
  std::string username, hostname, realname;

  if (client->getAuthState() < AuthNick) {
    return;
  }
  if (client->getAuthState() == AuthDone) {
    client->send(ERR_ALREADYREGISTRED(client->getNickname()));
    return;
  }
  if (msg.params.size() < 3 || msg.trailingParam.empty()) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), msg.command));
    return;
  }
  username = msg.params.at(0);
  realname = msg.trailingParam;
  hostname = getClientHostname(client);
  client->setUsername(username);
  client->setRealname(realname);
  client->setHostname(hostname);
  client->setAuthState(AuthDone);
  client->send(RPL_WELCOME(
      client->getNickname(),
      std::string(client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname())));
}
