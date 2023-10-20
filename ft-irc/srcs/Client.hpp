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
  const std::string &getUsername(void) const;
  const std::string &getRealname(void) const;
  const std::string &getHostname(void) const;

  void setNickname(const std::string &nickname);
  void setUsername(const std::string &username);
  void setRealname(const std::string &realname);
  void setHostname(const std::string &hostname);

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

  const std::string _nickname;
  const std::string _username;
  const std::string _realname;
  const std::string _hostname;
};

#endif
