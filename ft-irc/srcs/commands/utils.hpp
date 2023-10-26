#ifndef COMMANDUTILS_HPP
#define COMMANDUTILS_HPP

#include <string>
#include <vector>

std::vector<std::string> splitByComma(const std::string &data);

bool isValidChannelName(const std::string &channelName);

#endif