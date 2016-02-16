#include "conversation.hh"

conversation::conversation(const std::string &_name, const int _id, const bool _priv)
 : name(_name), id(_id), priv(_priv)
{

}

conversation::~conversation()
{

}

void    conversation::addMsg(std::string const &user, std::string const &message)
{
  s_msg   tmp;

  tmp.user = user;
  tmp.msg = message;

  if (msgs.size() > 60)
    this->msgs.pop_front();
  this->msgs.push_back(tmp);
}

void    conversation::banContact(std::string const &contact)
{
  this->banList.push_back(contact);
}

std::list<s_msg>          conversation::getMessages() const
{
  return this->msgs;
}

std::string               conversation::getName() const
{
  return this->name;
}

int                       conversation::getId() const
{
  return this->id;
}

std::vector<std::string>  conversation::getBanList() const
{
  return this->banList;
}

bool                      conversation::getPrivate() const
{
  return priv;
}

void                      conversation::setName(std::string const &n)
{
  this->name = n;
}

void                      conversation::setId(int const i)
{
  this->id = i;
}

void                      conversation::setBanList(std::vector<std::string> const &bl)
{
  this->banList = bl;
}

void                      conversation::setPrivate(bool const p)
{
  this->priv = p;
}
