#include "ChannelManager.hh"

ChannelManager::ChannelManager(IUserDB *db)
{
  this->db = db;
}

ChannelManager::~ChannelManager()
{

}

Channel			*ChannelManager::getChannel(const std::string &channame)
{
  return (static_cast<Channel *>(this->db->select(CHANNEL_PREFIX(channame))));
}

std::list<Channel *>	ChannelManager::getChannels() const
{
  std::list<void *>	tmp;
  std::list<Channel *>	ret;
  std::list<void *>::iterator	it;

  tmp = this->db->selectStartWith(CHANNEL_PREFIX(std::string("")));
  it = tmp.begin();
  while (it != tmp.end())
    {
      ret.push_back(static_cast<Channel *>(*it));
      ++it;
    }
  return (ret);
}

bool			ChannelManager::addChannel(Channel *to_add)
{
  return (this->db->insert(to_add, false));
}

bool			ChannelManager::removeChannel(Channel *to_remove)
{
  return (this->db->del(CHANNEL_PREFIX(to_remove->getName())));
}
