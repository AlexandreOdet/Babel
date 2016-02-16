//
// Status.cpp for Status in /home/lopez_t/Babel/server
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Fri Oct 30 16:10:16 2015 Thibaut Lopez
// Last update Fri Oct 30 18:49:08 2015 Thibaut Lopez
//

#include "Status.hh"

Status::Status()
{
  this->_convert[CONNECTED] = "connected";
  this->_convert[DISCONNECTED] = "disconnected";
  this->_convert[AWAY] = "away";
  this->_convert[UNKNOWN] = "unknown";
}

Status::~Status()
{
}

const eStatus		&Status::convert(const std::string &status) const
{
  std::map<eStatus, std::string>::const_iterator	it;

  it = this->_convert.begin();
  while (it != this->_convert.end())
    {
      if ((*it).second == status)
	return ((*it).first);
      ++it;
    }
  it = this->_convert.begin();
  while ((*it).first != UNKNOWN)
    ++it;
  return ((*it).first);
}

const std::string	&Status::convert(const eStatus &status) const
{
  return (this->_convert.at(status));
}
