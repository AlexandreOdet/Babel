#include "Channel.hh"
#include <algorithm>

Channel::Channel(const std::string &channel_name)
  : name(channel_name)
{
}

Channel::~Channel()
{
}

bool				Channel::addPeople(const std::string &login)
{
  stringlist::const_iterator	it;

  it = std::find(this->people.begin(), this->people.end(), login);
  if (it != this->people.end())
    {
      //login already in the chat
      return (false);
    }
  it = std::find(this->banlist.begin(), this->banlist.end(), login);
  if (it != this->banlist.end())
    {
      //login is banned
      return (false);
    }
  this->people.push_back(login);
  return (true);
}

bool				Channel::banPeople(const std::string &login)
{
  stringlist::const_iterator	it;

  it = std::find(this->banlist.begin(), this->banlist.end(), login);
  if (it == this->banlist.end())
    {
      this->banlist.push_back(login);
      return (true);
    }
  return (false);
}

bool				Channel::removePeople(const std::string &login)
{
  stringlist::iterator		it;

  it = std::find(this->people.begin(), this->people.end(), login);
  if (it != this->people.end())
    {
      this->people.erase(it);
      return (true);
    }
  return (false);
}

bool				Channel::unbanPeople(const std::string &login)
{
  stringlist::iterator		it;

  it = std::find(this->banlist.begin(), this->banlist.end(), login);
  if (it != this->banlist.end())
    {
      this->banlist.erase(it);
      return (true);
    }
  return (false);
}

const stringlist		&Channel::getPeoples() const
{
  return (this->people);
}

const std::string		&Channel::getName() const
{
  return (this->name);
}

uint16_t			Channel::getProxyPort() const
{
  return (1000);
}
