#ifndef IUSERDB_HH_
#define IUSERDB_HH_
#define LOGIN_PREFIX(X)		("LOG_" + (X))
#define CHANNEL_PREFIX(X)	("CHN_" + (X))
#define PRIVATE_PREFIX(X)	("PRV_" + (X))

#include <string>
#include <list>
#include "UserInformation.hh"
#include "Channel.hh"

class IUserDB
{
public:
  virtual ~IUserDB(){}
  virtual void			*select(const std::string &) const = 0;
  virtual std::list<void *>	selectStartWith(const std::string &) const = 0;
  virtual bool                  insert(UserInformation *) = 0;
  virtual bool                  insert(Channel *, const bool &) = 0;
  virtual bool                  del(const std::string &) = 0;
};

#endif
