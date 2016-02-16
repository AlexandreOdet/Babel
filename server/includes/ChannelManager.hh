#ifndef CHANNELMANAGER_HH_
#define CHANNELMANAGER_HH_

#include "Channel.hh"
#include "IUserDB.hh"

class ChannelManager
{
public:
  ChannelManager(IUserDB*);
  ~ChannelManager();
  Channel		*getChannel(const std::string &);
  std::list<Channel *>	getChannels() const;
  bool			addChannel(Channel *);
  bool			removeChannel(Channel *);

private:
  IUserDB		*db;
};

#endif //CHANNELMANAGER_HH_
