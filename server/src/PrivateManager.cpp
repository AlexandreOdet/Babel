//
// PrivateManager.cpp for PrivateManager in /home/lopez_t/Babel/server
// 
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
// 
// Started on  Thu Nov  5 15:26:16 2015 Thibaut Lopez
// Last update Sat Nov  7 22:35:08 2015 Thibaut Lopez
//

#include "PrivateManager.hh"

PrivateManager::PrivateManager(IUserDB *db)
  : _db(db)
{
}

PrivateManager::~PrivateManager()
{
}

std::string		 PrivateManager::_getChanName(const std::string &login1, const std::string &login2)
{
  if (login1.compare(login2) > 0)
    return (login1 + "+" + login2);
  else if (login1.compare(login2) < 0)
    return (login2 + "+" + login1);
  else
    return ("");
}

Channel			 *PrivateManager::getPrivate(const std::string &login1, const std::string &login2)
{
  std::string		 chanName;

  chanName = this->_getChanName(login1, login2);
  if (chanName.size() == 0)
    return (NULL);
  return (static_cast<Channel *>(this->_db->select(PRIVATE_PREFIX(chanName))));
}

std::map<std::string, std::list<std::string> >	 PrivateManager::getPrivatesOf(const std::string &login)
{
  std::list<void *>	 tmp;
  std::map<std::string, std::list<std::string> >	ret;
  std::list<void *>::iterator	it;
  Channel		 *chan;
  std::string		 str;

  tmp = this->_db->selectStartWith(PRIVATE_PREFIX(std::string("")));
  it = tmp.begin();
  while (it != tmp.end())
    {
      chan = static_cast<Channel *>(*it);
      str = this->getName(chan, login);
      if (str.size() > 0)
	ret[str] = chan->getPeoples();
      ++it;
    }
  return (ret);
}

bool			 PrivateManager::addPrivate(const std::string &login1, const std::string &login2)
{
  std::string		 chanName;

  chanName = this->_getChanName(login1, login2);
  if (chanName.size() == 0)
    return (NULL);
  return (this->_db->insert(new Channel(chanName), true));
}

bool			 PrivateManager::removePrivate(const std::string &login1, const std::string &login2)
{
  std::string		 chanName;

  chanName = this->_getChanName(login1, login2);
  if (chanName.size() == 0)
    return (NULL);
  return (this->_db->del(PRIVATE_PREFIX(chanName)));
}

bool			 PrivateManager::removePrivate(Channel *chan)
{
  return (this->_db->del(PRIVATE_PREFIX(chan->getName())));
}

const std::string	 PrivateManager::getName(Channel *chan, const std::string &login) const
{
  std::string		 str;
  std::size_t		 pos;

  pos = chan->getName().find(login);
  str = "";
  if (pos != std::string::npos)
    {
      if (pos == 0 && chan->getName().at(login.size()) == '+')
	str = chan->getName().substr(login.size() + 1);
      else if (pos > 0 && chan->getName().at(pos - 1) == '+' && pos + login.size() == chan->getName().size())
	str = chan->getName().substr(0, pos - 1);
    }
  return (str);
}

bool			PrivateManager::isPrivate(Channel *chan) const
{
  if (this->_db->select(PRIVATE_PREFIX(chan->getName())) == NULL)
    return (false);
  else
    return (true);
}
