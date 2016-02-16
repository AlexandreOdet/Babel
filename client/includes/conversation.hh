#ifndef conversation_hh
#define conversation_hh

#include "conversation.hh"
#include <iostream>
#include <vector>
#include <list>

struct s_msg
{
  std::string user;
  std::string msg;
};

class conversation
{
private:
  std::list<s_msg>            msgs;
  std::vector<std::string>  banList;
  std::string   name;
  int           id;
  bool          priv;

public:
  conversation(const std::string &_name, const int _id, const bool _priv);
  ~conversation();

  void                      addMsg(std::string const &, std::string const &);
  void                      banContact(std::string const &);

  std::list<s_msg>          getMessages() const;
  std::string               getName() const;
  int                       getId() const;
  std::vector<std::string>  getBanList() const;
  bool                      getPrivate() const;

  void                      setName(std::string const &);
  void                      setId(int const);
  void                      setBanList(std::vector<std::string> const &);
  void                      setPrivate(bool const);
};

#endif
