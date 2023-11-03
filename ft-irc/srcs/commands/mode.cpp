#include "../commands.hpp"

#include "../Channel.hpp"
#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"
#include "utils.hpp"
#include <algorithm>

static bool requiresModeParam(char mode) { return (std::string("okl").find(mode) != std::string::npos); }

static bool hasRequiredModeParams(const std::vector<std::string> msgParams) {
  const std::string &modes = msgParams.at(1);
  std::vector<std::string> modeParams(msgParams.begin() + 2, msgParams.end());
  std::size_t countRequiresModeParam = std::count_if(modes.begin(), modes.end(), requiresModeParam);

  if (countRequiresModeParam != modeParams.size()) {
    return (false);
  }
  return (true);
}

static bool handleTopicMode(Channel *channel, bool isSetOp) {
  if (isSetOp == channel->isTopicRestricted()) {
    return (false);
  }
  channel->setRestrictTopic(isSetOp);
  return (true);
}

static bool handleInviteMode(Channel *channel, bool isSetOp) {
  if (isSetOp == channel->isInviteOnly()) {
    return (false);
  }
  channel->setInviteOnly(isSetOp);
  return (true);
}

static bool handleChanOpMode(Channel *channel, Client *client, bool isSetMode, std::string nickname) {
  Client *target;

  target = channel->getClient(nickname);
  if (target == NULL) {
    client->send(ERR_USERNOTINCHANNEL(client->getNickname(), channel->getName(), nickname));
    return (false);
  }
  if (isSetMode) {
    channel->addOperator(target);
  } else {
    channel->removeOperator(target);
  }
  return (true);
}

static void handleKeyMode(Channel *channel, bool isSetOp, std::string key) {
  if (isSetOp) {
    channel->setKey(key);
  } else {
    channel->setKey("");
  }
}

static std::map<char, void (*)(Channel *, bool, std::string)> getModeHandlersWithParam() {
  std::map<char, void (*)(Channel *, bool, std::string)> modeHandlersWithParam;

  modeHandlersWithParam['k'] = handleKeyMode;
  return (modeHandlersWithParam);
}

static std::map<char, bool (*)(Channel *, bool)> getModeFlagHandlers() {
  std::map<char, bool (*)(Channel *, bool)> modeFlagHandlers;

  modeFlagHandlers['t'] = handleTopicMode;
  modeFlagHandlers['i'] = handleInviteMode;
  return (modeFlagHandlers);
}

static void setMode(Channel *channel, Client *client, Message &msg) {
  bool isSetOp;
  std::string modes = msg.params.at(1);
  std::string finalMode, finalModeParam;
  std::vector<std::string> modeParams(msg.params.begin() + 2, msg.params.end());
  std::map<char, bool (*)(Channel *, bool)> modeFlagHandlers = getModeFlagHandlers();
  std::map<char, void (*)(Channel *, bool, std::string)> modeHandlersWithParam = getModeHandlersWithParam();

  isSetOp = true;
  std::string::iterator i, j;
  for (i = modes.begin(); i != modes.end(); ++i) {
    if (std::string("+-").find(*i) != std::string::npos) {
      isSetOp = (*i == '+');
      continue;
    }
    std::string batchChanges = "";
    for (j = i; j != modes.end() && std::string("+-").find(*j) == std::string::npos; ++j) {
      if (*j == 'o') {
        if (handleChanOpMode(channel, client, isSetOp, modeParams.at(0))) {
          batchChanges += "o";
          finalModeParam += modeParams.at(0);
        }
        modeParams.erase(modeParams.begin());
      }
      if (modeHandlersWithParam.find(*j) != modeHandlersWithParam.end()) {
        batchChanges += *j;
        finalModeParam += isSetOp ? modeParams.at(0) : "*";
        modeHandlersWithParam[*j](channel, isSetOp, modeParams.at(0));
        modeParams.erase(modeParams.begin());
      }
      if (modeFlagHandlers.find(*j) != modeFlagHandlers.end() && modeFlagHandlers[*j](channel, isSetOp)) {
        batchChanges += *j;
      }
    }
    if (i != j) {
      if (!batchChanges.empty()) {
        finalMode += isSetOp ? "+" : "-";
        finalMode += batchChanges;
      }
      i = j - 1;
    }
  }
  if (!finalMode.empty()) {
    channel->send(MSG_MODE(client->getPrefix(), channel->getName(), finalMode + " " + finalModeParam));
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
  if (server.isOper(client) == false && channel->isOperator(client) == false) {
    client->send(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()));
    return;
  }
  idx = msg.params.at(1).find_first_not_of(CHANNELMODES_CHARS + "+-");
  if (idx != std::string::npos) {
    client->send(ERR_UNKNOWNMODE(client->getNickname(), msg.params.at(1).at(idx)));
    return;
  }
  if (!hasRequiredModeParams(msg.params)) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), msg.command));
    return;
  }
  setMode(channel, client, msg);
}
