#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "UserDB.hh"

#ifdef _WIN32
#define PRIVATE_PREFIX(X)	("PRV_" + (X))
#endif

#define foreach BOOST_FOREACH

// Example of base file:
// {
//   "login1":
//     {
//       "password" : "pass",
// 	"contact" : 
// 	[ "login2", "login3"]
//     },
//   "login2":
//     {
//       "password" : "pass",
// 	"contact" : 
// 	[ "login1", "login3"]
//     }
// }

UserDB::UserDB(const char *path)
{
  try
    {
      std::ifstream			t(path);
      std::stringstream			buffer;
      boost::property_tree::ptree	pt;
      UserInformation			*tmp;

      this->path = path;      
      buffer << t.rdbuf();
      boost::property_tree::read_json(buffer, pt);
      foreach(boost::property_tree::ptree::value_type &user_pair, pt) {
	// Now user_pair.first == login and user_pair.second is the subtree
	tmp = new UserInformation(user_pair.first,
				  user_pair.second.get<std::string>("password"));
	foreach(boost::property_tree::ptree::value_type &row_pair,
		      user_pair.second.get_child("contact"))
	  {
	    tmp->addContact(row_pair.second.get_value<std::string>());
	  }
	this->users.insert(std::pair<std::string, UserInformation *>(LOGIN_PREFIX(tmp->getLogin()), tmp));
      }
      t.close();
    }
  catch(std::exception const &e)
    {
		(void)e;
    }
}

UserDB::~UserDB()
{
  std::ofstream				t;
  std::string				tmp;
  bool					b;

  b = false;
  try
    {
      t.open(this->path.c_str(), std::ofstream::binary | std::ofstream::trunc);
    }
  catch(std::exception const &e)
    {
		(void)e;
      goto no_write;
    }
  t.write("{", 1);
  foreach(const entry::value_type &p, this->users)
    {
      if (p.first.find(LOGIN_PREFIX(std::string(""))) == 0)
	{
	  if (b == false)
	    b = true;
	  else
	    t.write(",", 1);
	  tmp = ((UserInformation*)(p.second))->stringify();
	  t.write(tmp.c_str(), tmp.length());
	  delete ((UserInformation*)(p.second));
	}
    }
  t.write("}", 1);
  t.close();
 no_write:;
}

void					*UserDB::select(const std::string &login) const
{
  entry::const_iterator			it;

  it = this->users.find(login);
  if (it != this->users.end())
    return (it->second);
  else
    return (NULL);
}

std::list<void *>			UserDB::selectStartWith(const std::string &key) const
{
  std::list<void *>			ret;
  entry::const_iterator			it;

  it = this->users.begin();
  while (it != this->users.end())
    {
      if ((*it).first.compare(0, key.size(), key) == 0)
	ret.push_back((*it).second);
      ++it;
    }
  return (ret);
}

bool					UserDB::insert(UserInformation *to_insert)
{
  entry::iterator			it;

  it = this->users.find(LOGIN_PREFIX(to_insert->getLogin()));
  if (it == this->users.end())
    {
      this->users[LOGIN_PREFIX(to_insert->getLogin())] = to_insert;
      return (true);
    }
  else
    return (false);
}

bool					UserDB::insert(Channel *to_insert, const bool &isPrivate)
{
  entry::iterator			it;
  std::string				name;

  name = (isPrivate) ? PRIVATE_PREFIX(to_insert->getName()) : CHANNEL_PREFIX(to_insert->getName());
  it = this->users.find(name);
  if (it == this->users.end())
    {
      this->users[name] = to_insert;
      return (true);
    }
  else
    return (false);
}

bool					UserDB::del(const std::string &login)
{
  entry::iterator			it;

  it = this->users.find(login);
  if (it != this->users.end())
    {
      if (it->first.find(LOGIN_PREFIX(std::string(""))) != std::string::npos)
	delete ((UserInformation*)(it->second));
      else
	delete ((Channel*)(it->second));
      this->users.erase(it);
      return (true);
    }
  else
    return (false);
}
