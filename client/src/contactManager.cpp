#include "contactManager.hh"
#include <iostream>

contactManager::contactManager()
{
  contacts = *(new std::vector<contact*>);
  cid = 1;
}

contactManager::~contactManager()
{
}

void          contactManager::createcontact(const std::string &name)
{
  this->contacts.push_back(new contact(name, cid));
  cid++;
}

void          contactManager::deletecontact(const int id)
{
  contacts.erase(contacts.begin() + id);
}

void          contactManager::deletecontact(const std::string &name)
{
  for (std::vector<contact*>::iterator it = contacts.begin(); it != contacts.end(); it++)
  {
    if (name.compare((*it)->getName()) == 0)
    {
      contacts.erase(it);
      return;
    }
  }
}

void          contactManager::printcontacts() const
{
  std::cout << "There are " << contacts.size() << " contacts : " << std::endl;

  for (std::vector<contact*>::const_iterator it = contacts.cbegin(); it != contacts.cend(); it++)
  {
	  std::string tmp((*it)->getId() + " : " + (*it)->getName());
    std::cout << tmp.c_str() << std::endl;
  }
}

contact  *contactManager::getcontact(const int id) const
{
  return contacts[id];
}

contact  *contactManager::getcontact(const std::string &name) const
{
  for (std::vector<contact*>::const_iterator it = contacts.cbegin(); it != contacts.cend(); it++)
  {
    if (name.compare((*it)->getName()))
      return *it;
  }
  return NULL;
}

std::vector<contact*> contactManager::getcontacts() const
{
  return contacts;
}
