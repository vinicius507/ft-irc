#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"

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

static std::string getNicknameWithFallback(const Client *client) {
  std::string nickname = client->getNickname();

  if (nickname.empty()) {
    return ("*");
  }
  return (nickname);
}

void nickCommand(Server &server, Client *client, Message &msg) {
  std::string nickname;

  if (client->getAuthState() == AuthNone) {
    client->send(ERR_NOTREGISTERED("*"));
    return;
  }
  if (msg.params.size() < 1) {
    client->send(ERR_NONICKNAMEGIVEN(getNicknameWithFallback(client)));
    return;
  }
  nickname = msg.params.at(0);
  if (server.getClientByNickname(nickname) != NULL) {
    client->send(ERR_NICKNAMEINUSE(getNicknameWithFallback(client), nickname));
    return;
  }
  if (!isNicknameValid(nickname)) {
    client->send(ERR_ERRONEUSNICKNAME(getNicknameWithFallback(client), nickname));
    return;
  }
  client->setNickname(nickname);
  if (client->getAuthState() == AuthPass) {
    client->setAuthState(AuthNick);
  }
}
