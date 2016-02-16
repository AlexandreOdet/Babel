//
// PrivateManager.hh for PrivateManager in /home/lopez_t/Babel/server
// 
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
// 
// Started on  Thu Nov  5 15:26:31 2015 Thibaut Lopez
// Last update Sat Nov  7 22:25:30 2015 Thibaut Lopez
//

#ifndef PRIVATEMANAGER_HH_
#define PRIVATEMANAGER_HH_

#include "Channel.hh"
#include "IUserDB.hh"

class PrivateManager
{
public:
  explicit PrivateManager(IUserDB *db);
  ~PrivateManager();
  Channel	*getPrivate(const std::string &login1, const std::string &login2);
  std::map<std::string, std::list<std::string> >	getPrivatesOf(const std::string &login);
  bool			addPrivate(const std::string &login1, const std::string &login2);
  bool			removePrivate(const std::string &login1, const std::string &login2);
  bool			removePrivate(Channel *chan);
  const std::string	getName(Channel *chan, const std::string &login) const;
  bool			isPrivate(Channel *chan) const;

private:
  IUserDB		*_db;
  std::string		_getChanName(const std::string &login1, const std::string &login2);
};

#endif //PRIVATEMANAGER_HH_
