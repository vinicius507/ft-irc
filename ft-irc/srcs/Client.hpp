#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
  Client(void);

  Client(const std::string nickname, const std::string username);

  Client(const Client &client);

  ~Client(void);

  Client &operator=(const Client &client);

  std::string getNickName(void);

  std::string getUserName(void);

private:
  std::string _nickname;
  std::string _username;
};

#endif
