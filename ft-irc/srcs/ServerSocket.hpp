#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

class ServerSocket {
public:
  ServerSocket(void);

  ServerSocket(short port);

  ServerSocket(const ServerSocket &other);

  ~ServerSocket(void);

  ServerSocket &operator=(const ServerSocket &other);

  int getFd(void) const;

  int acceptClient(void) const;

  bool bindAndListen(void);

  void closeSocket(void);

private:
  int _socketFd;
  short _port;
};

#endif
