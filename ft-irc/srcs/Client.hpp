#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
  Client(void);

  Client(int fd);

  Client(const Client &client);

  ~Client(void);

  Client &operator=(const Client &client);

  int getFd(void) const;

private:
  const int _fd;
};

#endif
