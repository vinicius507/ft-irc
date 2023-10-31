#include "../commands.hpp"

#include "../Channel.hpp"
#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"
#include "utils.hpp"

static bool handleTopicMode(Channel *channel, char op) {
  bool isSetOp = (op == '+');

  if (isSetOp == channel->isTopicRestricted()) {
    return (false);
  }
  channel->setRestrictTopic(isSetOp);
  return (true);
}

static bool handleInviteMode(Channel *channel, char op) {
  bool isSetOp = (op == '+');

  if (isSetOp == channel->isInviteOnly()) {
    return (false);
  }
  channel->setInviteOnly(isSetOp);
  return (true);
}

static bool handleChanOpMode(Channel *channel, Client *client, char op, const std::string &nickname) {
  Client *target;
  bool isSetOp = op == '+';

  target = channel->getClientByNickname(nickname);
  if (target == NULL) {
    client->send(ERR_USERNOTINCHANNEL(client->getNickname(), nickname, channel->getName()));
    return (false);
  }
  if (isSetOp) {
    channel->addOperator(client);
  } else {
    channel->removeOperator(client);
  }
  return (true);
}

static void setMode(Channel *channel, Client *client, Message &msg) {
  char op = '+';
  std::string ops("+-");
  std::string::iterator it;
  std::string finalModes, finalModeParams;
  std::string modes = msg.params.at(1);

  for (it = modes.begin(); it != modes.end(); ++it) {
    if (ops.find(*it) != std::string::npos) {
      op = *it;
      finalModes += op;
      continue;
    }
    while (it != modes.end() && ops.find(*it) == std::string::npos) {
      switch (*it) {
      case 't':
        if (handleTopicMode(channel, op)) {
          finalModes += 't';
        }
        break;
      case 'i':
        if (handleInviteMode(channel, op)) {
          finalModes += 'i';
        }
        break;
      case 'o':
        if (handleChanOpMode(channel, client, op, msg.params.at(2))) {
          finalModes += 'o';
          finalModeParams += msg.params.at(2);
        }
        break;
      }
      ++it;
    }
  }
  if (!finalModes.empty()) {
    channel->send(MSG_MODE(client->getNickname(), channel->getName(), finalModes + " " + finalModeParams));
  }
}

void modeCommand(Server &server, Client *client, Message &msg) {
  Channel *channel;
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
  setMode(channel, client, msg);
}
