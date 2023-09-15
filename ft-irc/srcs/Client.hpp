#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {

public:
  Client(void);
  Client(const std::string nickname, const std::string username);
  Client(const Client &client);
  Client &operator=(const Client &client);
  ~Client(void);

  std::string getNick(void);
  std::string getName(void);

private:
  std::string _nickname;
  std::string _usarname;
};

#endif
