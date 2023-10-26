#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../serverReplies.hpp"
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

static void registerClient(Client *client, const std::string &username, const std::string &realname) {
  std::string hostname;

  hostname = getClientHostname(client);
  client->setUsername(username);
  client->setHostname(hostname);
  client->setRealname(realname);
  client->setAuthState(AuthDone);
}

static void greetClient(Client *client) {
  std::string msgs[4] = {
      RPL_WELCOME(client->getNickname(), client->getUsername(), client->getHostname()),
      RPL_YOURHOST(client->getNickname()),
      RPL_CREATED(client->getNickname()),
      RPL_MYINFO(client->getNickname()),
  };

  for (int i = 0; i < 4; i++) {
    client->send(msgs[i]);
  }
}

void userCommand(Server &server, Client *client, Message &msg) {
  (void)server;

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
  registerClient(client, msg.params.at(0), msg.trailingParam);
  greetClient(client);
}
