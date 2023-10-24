#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

#include <string>

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

#define ERR_NOSUCHCHANNEL(target, channel) numericReply("403", target, channel + " :No such channel")
#define ERR_UNKNOWNCOMMAND(target, command) numericReply("421", target, command + " :Unknown command")
#define ERR_NONICKNAMEGIVEN(target) numericReply("431", target, ":No nickname given")
#define ERR_ERRONEUSNICKNAME(target, nickname) numericReply("432", target, nickname + " :Erroneous nickname")
#define ERR_NICKNAMEINUSE(target, nickname) numericReply("433", target, nickname + " :Nickname is already in use")
#define ERR_NOTREGISTERED(target) numericReply("451", target, ":You have not registered")
#define ERR_NEEDMOREPARAMS(target, command) numericReply("461", target, command + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(target) numericReply("462", target, ":You may not reregister")
#define ERR_PASSWSMISMATCH(target) numericReply("464", target, ":Password Incorrect")
#define ERR_BADCHANNELKEY(target, channel) numericReply("475", target, channel + " :Cannot join channel (+k)")

#endif
