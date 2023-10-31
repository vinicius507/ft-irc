#include "../commands.hpp"

#include "../Channel.hpp"
#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"
#include "utils.hpp"

static bool handleChangeModes(Client *client, Channel *channel, char op, std::string modes, std::string modeParams) {
  bool didChange = false;
  bool isSetOp = op == '+';
  std::string::iterator it;

  for (it = modes.begin(); it != modes.end(); ++it) {
    if (*it == 't' && (isSetOp != channel->isTopicRestricted())) {
      channel->setRestrictTopic(isSetOp);
      didChange = true;
      continue;
    }
    if (*it == 'i' && (isSetOp != channel->isInviteOnly())) {
      channel->setInviteOnly(isSetOp);
      didChange = true;
      continue;
    }
    if (*it == 'o') {
      Client *target = channel->getClientByNickname(modeParams);
      if (target == NULL) {
        client->send(ERR_USERNOTINCHANNEL(client->getNickname(), modeParams, channel->getName()));
        continue;
      }
      if (isSetOp != channel->isOperator(target)) {
        if (isSetOp) {
          channel->addOperator(target);
        } else {
          channel->removeOperator(target);
        }
        didChange = true;
      }
    }
  }
  return (didChange);
}

void modeCommand(Server &server, Client *client, Message &msg) {
  Channel *channel;
  std::string::size_type idx, modeCount;
  std::string modes, modeParams, batch, changed;

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
  batch = std::string();
  changed = std::string();
  modes = msg.params.at(1);
  modeCount = 0;
  modeParams = std::string();
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
    std::string modeParam = (msg.params.size() > modeCount + 2) ? msg.params.at(2 + modeCount++) : "";
    batch = modes.substr(idx, endOfBatch);
    if (handleChangeModes(client, channel, op, batch, modeParam)) {
      changed += op + batch;
      modeParams += modeParam + " ";
    }
  }
  if (!changed.empty()) {
    channel->send(MSG_MODE(client->getNickname(), channel->getName(), changed + " " + modeParams));
  }
}
