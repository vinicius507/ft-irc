#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/poll.h>
#include <vector>

class Server {
public:
  Server(void);

  Server(short port);

  Server(const Server &other);

  ~Server(void);

  Server &operator=(const Server &other);

  void close(void);

  bool listen(void);

  bool run(void);

  void acceptNewClient(void);

  void handleClientData(int fd);

  void removeClient(int clientFd);

private:
  short _port;
  bool _listening;
  std::vector<struct pollfd> _pollFds;
};

#endif
