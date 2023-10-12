#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

enum AuthState {
  AuthNone,
  AuthPass,
  AuthNick,
  AuthDone,
};

class Client {
  public:
  Client(void);

  Client(int fd);

  Client(const Client &client);

  ~Client(void);

  Client &operator=(const Client &client);

  int getFd(void) const;

  AuthState getAuthState(void) const;

  void setAuthState(AuthState state);

  std::string &getBuffer(void);

  const std::string &getNickname(void) const;

  void setNickname(const std::string &nickname);

  enum ReadEvent {
    ReadError = -1,
    ReadEof,
    ReadIn,
  };

  ReadEvent read(void);

  void send(const std::string &msg);

  private:
  const int _fd;
  std::string _buffer;
  AuthState _authState;
  std::string _nickname;
};

#endif
