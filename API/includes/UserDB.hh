#include <map>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "IUserDB.hh"

typedef std::map<std::string, void *> entry;

class UserDB : public IUserDB
{
public:
  UserDB(const char *path);
  virtual ~UserDB();
  virtual void			*select(const std::string &) const;
  virtual std::list<void *>	selectStartWith(const std::string &) const;
  virtual bool			insert(UserInformation *);
  virtual bool			insert(Channel *, const bool &);
  virtual bool			del(const std::string &);

private:
  entry				users;
  std::string			path;
};
