#include <iostream>
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "UserInformation.hh"

UserInformation::UserInformation(const std::string &login, const std::string &password)
  : status(DISCONNECTED)
{
  this->login = login;
  this->password = password;
}

UserInformation::~UserInformation()
{
}

bool				UserInformation::operator==(UserInformation &cmp) const
{
  return (this->login == cmp.getLogin() && this->password == cmp.getPassword());
}

bool				UserInformation::operator!=(UserInformation &cmp) const
{
  return (this->login != cmp.getLogin() || this->password != cmp.getPassword());
}

bool				UserInformation::addContact(const std::string &login)
{
  std::vector<std::string>::iterator	it;

  it = std::find(this->contacts.begin(), this->contacts.end(), login);
  if (it == this->contacts.end())
    {
      this->contacts.push_back(login);
      return (true);
    }
  else
    return (false);
}

bool				UserInformation::delContact(const std::string &login)
{
  std::vector<std::string>::iterator	it;

  it = std::find(this->contacts.begin(), this->contacts.end(), login);
  if (it != this->contacts.end())
    {
      this->contacts.erase(it);
      return (true);
    }
  else
    return (false);
}

const std::vector<std::string>	&UserInformation::getContacts() const
{
  return (this->contacts);
}

const std::string		&UserInformation::getPassword() const
{
  return (this->password);
}

void				UserInformation::setPassword(const std::string &password_)
{
  this->password = password_;
}

const std::string		&UserInformation::getLogin() const
{
  return (this->login);
}

void				UserInformation::setLogin(const std::string &login_)
{
  this->login = login_;
}

const eStatus			&UserInformation::getStatus() const
{
  return (this->status);
}

void				UserInformation::setStatus(const eStatus &status_)
{
  this->status = status_;
}

bool				UserInformation::addPending(const std::string &login)
{
  std::vector<std::string>::iterator	it;

  it = std::find(this->pendings.begin(), this->pendings.end(), login);
  if (it == this->pendings.end())
    {
      this->pendings.push_back(login);
      return (true);
    }
  else
    return (false);
}

bool				UserInformation::delPending(const std::string &login)
{
  std::vector<std::string>::iterator	it;

  it = std::find(this->pendings.begin(), this->pendings.end(), login);
  if (it != this->pendings.end())
    {
      this->pendings.erase(it);
      return (true);
    }
  else
    return (false);
}

const std::vector<std::string>	&UserInformation::getPendings() const
{
  return (this->pendings);
}

std::string			UserInformation::stringify() const
{
  boost::property_tree::ptree	pt;
  boost::property_tree::ptree	subpt;
  boost::property_tree::ptree	row;
  boost::property_tree::ptree	array;
  std::ostringstream		buff;
  std::string			r;

  subpt.put("password", this->password);
  BOOST_FOREACH(const std::string &str, this->contacts)
    {
      row.put("", str.c_str());
      array.push_back(std::make_pair("", row));
    }
  subpt.add_child("contact", array);
  pt.add_child(this->login, subpt);
  boost::property_tree::write_json(buff, pt, false);
  r = buff.str();
  r.erase(0, 1);
  r.erase(r.length() - 2, 1);
  return (r);
}
