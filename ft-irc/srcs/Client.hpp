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

  static void setPassword(const std::string &password);

  int getFd(void) const;

  std::string &getBuffer(void);

  enum ReadEvent {
    ReadError = -1,
    ReadEof,
    ReadIn,
  };

  ReadEvent read(void);

  private:
  const int _fd;
  std::string _buffer;

  static const std::string _password;
};

#endif
