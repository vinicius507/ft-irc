#include "../commands.hpp"

#include "../Channel.hpp"
#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"
#include "utils.hpp"

static bool handleChangeModes(Channel *channel, char op, std::string modes) {
  bool didChange = false;
  bool isSetOp = op == '+';
  std::string::size_type i = 0;

  while (i < modes.size()) {
    if (modes[i] == 't' && (isSetOp != channel->isTopicRestricted())) {
      channel->setRestrictTopic(isSetOp);
      didChange = true;
    }
    if (modes[i] == 'i' && (isSetOp != channel->isInviteOnly())) {
      channel->setInviteOnly(isSetOp);
      didChange = true;
    }
    ++i;
  }
  return (didChange);
}

void modeCommand(Server &server, Client *client, Message &msg) {
  Channel *channel;
  std::string modes, batch, changed;
  std::string::size_type idx;

  if (client->getAuthState() != AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (msg.params.empty()) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), msg.command));
    return;
  }
  channel = server.getChannel(msg.params.at(0));
  if (channel == NULL) {
    client->send(ERR_NOSUCHCHANNEL(client->getNickname(), msg.params.at(0)));
    return;
  }
  if (msg.params.size() < 2) {
    client->send(RPL_CHANNELMODEIS(client->getNickname(), channel->getName(), channel->getModes()));
    return;
  }
  idx = msg.params.at(1).find_first_not_of(CHANNELMODES_CHARS + "+-");
  if (idx != std::string::npos) {
    client->send(ERR_UNKNOWNMODE(client->getNickname(), msg.params.at(1).at(idx)));
    return;
  }
  char op = '+';
  batch = "";
  changed = "";
  modes = msg.params.at(1);
  for (idx = 0; idx < modes.size(); ++idx) {
    char c = modes[idx];
    if (c == '+') {
      op = '+';
      continue;
    }
    if (c == '-') {
      op = '-';
      continue;
    }
    std::string::size_type endOfBatch = modes.find_first_of("+-", idx);
    batch = modes.substr(idx, endOfBatch);
    if (handleChangeModes(channel, op, batch)) {
      changed += op + batch;
    }
  }
  if (!changed.empty()) {
    channel->send(MSG_MODE(client->getNickname(), channel->getName(), changed));
  }
}
