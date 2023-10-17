#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../numericReplies.hpp"

static bool isNicknameValid(const std::string &nickname) {
  char _char;

  if (nickname.empty() || nickname.size() > 9) {
    return (false);
  }
  _char = nickname.at(0);
  if (!std::isalpha(_char) && std::string(NICKNAME_SPECIAL_CHARS).find(_char) == std::string::npos) {
    return (false);
  }
  for (size_t i = 1; i < nickname.size(); i++) {
    _char = nickname.at(i);
    if (!std::isalnum(_char) && _char != '-' && std::string(NICKNAME_SPECIAL_CHARS).find(_char) == std::string::npos) {
      return (false);
    }
  }
  return (true);
}

void nickCommand(Server &server, Client *client, Message &msg) {
  std::string nickname;

  if (client->getAuthState() == AuthNone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (msg.params.size() < 1) {
    client->send(ERR_NONICKNAMEGIVEN(client->getNickname()));
    return;
  }
  nickname = msg.params.at(0);
  if (server.getClientByNickname(nickname) != NULL) {
    client->send(ERR_NICKNAMEINUSE(client->getNickname(), nickname));
    return;
  }
  if (isNicknameValid(nickname) == false) {
    client->send(ERR_ERRONEUSNICKNAME(client->getNickname(), nickname));
    return;
  }
  client->setNickname(nickname);
}
