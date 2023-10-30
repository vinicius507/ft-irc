#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

#include <string>

#define MSG_PONG "PONG ft-irc\r\n"
#define MSG_JOIN(prefix, channel) std::string(":" + prefix + " JOIN " + channel + "\r\n")
#define MSG_QUIT(prefix, message) std::string(":" + prefix + " QUIT :" + message + "\r\n")
#define MSG_PART(prefix, channel) std::string(":" + prefix + " PART " + channel + "\r\n")
#define MSG_PRIVMSG(prefix, target, message) std::string(":" + prefix + " PRIVMSG " + target + " :" + message + "\r\n")
#define MSG_NOTICE(prefix, target, message) std::string(":" + prefix + " NOTICE " + target + " :" + message + "\r\n")
#define MSG_KICK(prefix, channel, target, message)                                                                     \
  std::string(":" + prefix + " KICK " + channel + " " + target + " :" + message + "\r\n")
#define MSG_TOPIC(prefix, channel, topic) std::string(":" + prefix + " TOPIC " + channel + " :" + topic + "\r\n")
#define MSG_INVITE(prefix, channel, nickname) std::string(":" + prefix + " INVITE " + nickname + " " + channel + "\r\n")
#define MSG_MODE(prefix, target, modes) std::string(":" + prefix + " MODE " + target + " " + modes + "\r\n")

#define numericReply(numeric, target, message) std::string(":ft-irc ") + numeric + " " + target + " " + message + "\r\n"

#define RPL_WELCOME(target, user, host)                                                                                \
  numericReply("001", target, ":Welcome to the 42 IRC " + target + "!" + user + "@" + host)
#define RPL_YOURHOST(target) numericReply("002", target, ":Your host is ft-irc, running version 1.0")
#define RPL_CREATED(target) numericReply("003", target, ":This server was created sometime ago")
#define RPL_MYINFO(target) numericReply("004", target, "ft-irc 1.0 o o")
#define RPL_NOTOPIC(target, channel) numericReply("331", target, channel + " :No topic is set")
#define RPL_TOPIC(target, channel, topic) numericReply("332", target, channel + " :" + topic)
#define RPL_NAMREPLY(target, channel, nicknames) numericReply("353", target, "= " + channel + " :" + nicknames)
#define RPL_ENDOFNAMES(target, channel) numericReply("366", target, channel + " :End of NAMES list")
#define RPL_INVITING(channel, nickname) numericReply("341", channel, +" " + nickname)
#define RPL_CHANNELMODEIS(target, channel, modes) numericReply("324", target, channel + " +" + modes)

#define ERR_NOSUCHNICK(target, nickname) numericReply("401", target, nickname + " :No such nick")
#define ERR_NOSUCHCHANNEL(target, channel) numericReply("403", target, channel + " :No such channel")
#define ERR_NORECIPIENT(target, command) numericReply("411", target, command + " :No recipient given")
#define ERR_NOTEXTTOSEND(target) numericReply("412", target, ":No text to send")
#define ERR_UNKNOWNCOMMAND(target, command) numericReply("421", target, command + " :Unknown command")
#define ERR_NONICKNAMEGIVEN(target) numericReply("431", target, ":No nickname given")
#define ERR_ERRONEUSNICKNAME(target, nickname) numericReply("432", target, nickname + " :Erroneous nickname")
#define ERR_NICKNAMEINUSE(target, nickname) numericReply("433", target, nickname + " :Nickname is already in use")
#define ERR_NOTONCHANNEL(target, channel) numericReply("442", target, channel + " :You're not on that channel")
#define ERR_NOTREGISTERED(target) numericReply("451", target, ":You have not registered")
#define ERR_NEEDMOREPARAMS(target, command) numericReply("461", target, command + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(target) numericReply("462", target, ":You may not reregister")
#define ERR_PASSWSMISMATCH(target) numericReply("464", target, ":Password Incorrect")
#define ERR_INVITEONLYCHAN(target, channel) numericReply("473", target, channel + " :Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(target, channel) numericReply("475", target, channel + " :Cannot join channel (+k)")
#define ERR_CHANOPRIVSNEEDED(target, channel) numericReply("482", target, channel + " :You're not channel operator")
#define ERR_USERONCHANNEL(target, nickname, channel)                                                                   \
  numericReply("443", target, nickname + " " + channel + " :is already on channel")
#define ERR_UNKNOWNMODE(target, mode) numericReply("472", target, mode + " :is unknown mode char to me")

#endif
