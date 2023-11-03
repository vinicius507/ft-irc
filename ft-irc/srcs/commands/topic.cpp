#include "../commands.hpp"

#include "../Client.hpp"
#include "../Message.hpp"
#include "../Server.hpp"
#include "../serverReplies.hpp"

void topicCommand(Server &server, Client *client, Message &msg) {
  Channel *channel;

  if (client->getAuthState() < AuthDone) {
    client->send(ERR_NOTREGISTERED(client->getNickname()));
    return;
  }
  if (msg.params.size() == 0) {
    client->send(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
    return;
  }

  channel = server.getChannel(msg.params[0]);
  if (channel == NULL) {
    client->send(ERR_NOSUCHCHANNEL(client->getNickname(), msg.params[0]));
    return;
  }

  if (channel->hasClient(client) == false) {
    client->send(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()));
    return;
  }

  if (msg.trailingParam.empty()) {
    const std::string topic = channel->getTopic();
    if (topic.empty()) {
      client->send(RPL_NOTOPIC(client->getNickname(), channel->getName()));
      return;
    }
    client->send(RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()));
    return;
  }

  if (channel->isTopicRestricted() && !server.isOper(client) && !channel->isOperator(client)) {
    client->send(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()));
    return;
  } else {
    channel->setTopic(msg.trailingParam);
    channel->send(MSG_TOPIC(client->getPrefix(), channel->getName(), msg.trailingParam));
    return;
  }
}
