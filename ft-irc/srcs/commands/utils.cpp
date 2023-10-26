#include "utils.hpp"
#include <sstream>

std::vector<std::string> splitByComma(const std::string &data) {
  std::string token;
  std::vector<std::string> tokens;
  std::istringstream iss(data);

  while (std::getline(iss, token, ',')) {
    tokens.push_back(token);
  }
  return (tokens);
}

bool isValidChannelName(const std::string &channelName) {
  std::string::const_iterator it;

  if (channelName.size() < 2) {
    return (false);
  }
  if (channelName.at(0) != '#') {
    return (false);
  }
  for (it = channelName.begin() + 1; it != channelName.end(); ++it) {
    if (!std::isalnum(*it) && *it != '-' && *it != '_') {
      return (false);
    }
  }
  return (true);
}
