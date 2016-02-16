#include "conversationManager.hh"

conversationManager::conversationManager()
{
  conversations = *(new std::vector<conversation*>);
  cid = 0;
}

conversationManager::~conversationManager()
{
}

void          conversationManager::addMsgToConv(std::string const &msg, std::string const &user, std::string const &conv)
{
  for (std::vector<conversation*>::iterator it = conversations.begin(); it != conversations.end(); ++it)
  {
    if (conv.compare((*it)->getName()) == 0)
    {
      (*it)->addMsg(user, msg);
      return;
    }
  }
}

void          conversationManager::createConversation(const std::string &name, const bool priv)
{
  this->conversations.push_back(new conversation(name, cid, priv));
  cid++;
}

void          conversationManager::deleteConversation(const int id)
{
  conversations.erase(conversations.begin() + id);
}

void          conversationManager::deleteConversation(const std::string &name)
{
  for (std::vector<conversation*>::iterator it = conversations.begin(); it != conversations.end(); it++)
  {
    if (name.compare((*it)->getName()) == 0)
    {
      conversations.erase(it);
      return;
    }
  }
}

void          conversationManager::printConversations() const
{
  std::cout << "There is " << conversations.size() << " active conversations : " << std::endl;

  for (std::vector<conversation*>::const_iterator it = conversations.cbegin(); it != conversations.cend(); it++)
  {
    std::cout << (*it)->getId() << " : " << (*it)->getName().c_str();
    if ((*it)->getPrivate() == true)
      std::cout << " (private)" << std::endl;
    else
      std::cout << " (public)" << std::endl;

    std::vector<std::string> ban = (*it)->getBanList();
    for (std::vector<std::string>::iterator b = ban.begin(); b != ban.end(); b++)
    {
      std::cout << "  - " << *b->c_str() << std::endl;
    }
  }
}

void          conversationManager::BanContact(const std::string &name, const std::string &contact)
{
  for (std::vector<conversation*>::iterator it = conversations.begin(); it != conversations.end(); it++)
  {
    if (name.compare((*it)->getName()) == 0)
    {
      (*it)->banContact(contact);
      return;
    }
  }
}

void          conversationManager::BanContact(const int id, const std::string &contact)
{
  conversations[id]->banContact(contact);
}

conversation  *conversationManager::getConversation(const int id) const
{
  return conversations[id];
}

conversation  *conversationManager::getConversation(const std::string &name) const
{
  for (std::vector<conversation*>::const_iterator it = conversations.cbegin(); it != conversations.cend(); it++)
  {
    if (name.compare((*it)->getName()))
      return *it;
  }
  return NULL;
}

std::vector<conversation*>  conversationManager::getConversations() const
{
  return conversations;
}

std::list<s_msg>   conversationManager::getMessages(const std::string &name) const
{
  for (std::vector<conversation*>::const_iterator it = conversations.cbegin(); it != conversations.cend(); it++)
  {
    if (name.compare((*it)->getName()) == 0)
      return (*it)->getMessages();
  }
  std::list<s_msg> tmp;
  return tmp;
}
