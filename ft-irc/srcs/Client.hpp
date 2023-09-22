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

  std::string &getBuffer(void);

  private:
  const int _fd;
  std::string _buffer;
};

#endif
