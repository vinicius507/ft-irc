#ifndef POLLMANAGER_HPP
#define POLLMANAGER_HPP

#include <sys/poll.h>
#include <vector>

enum EPollStatus {
  PERROR = -1,
  PTIMEOUT = 0,
  PEVENTS = 1,
};

class PollManager {
  public:
  PollManager(void);

  PollManager(short port);

  PollManager(const PollManager &other);

  ~PollManager(void);

  PollManager &operator=(const PollManager &other);

  void disconnectClients(void);

  bool hasNewClientOnQueue(void) const;

  std::vector<int> getFdsReadyForReading(void) const;

  int poll(void);

  void addFd(int fd);

  void removeFd(int fd);

  private:
  std::vector<struct pollfd> _pollFds;
};

#endif
