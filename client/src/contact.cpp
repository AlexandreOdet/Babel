#include "contact.hh"

contact::contact(const std::string &_name, const int _id)
 : name(_name), id(_id)
{

}

contact::~contact()
{

}

std::string               contact::getName() const
{
  return this->name;
}

int                       contact::getId() const
{
  return this->id;
}

void                      contact::setName(std::string const &n)
{
  this->name = n;
}

void                      contact::setId(int const i)
{
  this->id = i;
}
