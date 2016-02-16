#ifndef EXECCMD_H_
#define EXECCMD_H_

#include <iostream>
#include <sstream>
#include <QString>
#include <map>
#include <list>

class Client;

class execCmd
{
private:
  Client              *client;
  typedef void        (execCmd::*_cmd)(std::string const &);
  typedef void        (execCmd::*ret)(int const, std::string const &);

  std::map<int, _cmd>      cmdMap;
  std::map<int, ret>      retMap;
  std::list<int>          waitingReturn;
  std::list<std::string>  waitingAdd;
  std::list<std::string>  waitingJoin;
  std::map<int, std::string> errCode;

public:
  execCmd (Client *c);
  ~execCmd ();

  void    findMessage(int const, std::string const &);
  void    addReturn(int const);
  void    addWaitAdd(std::string const &);
  void    addWaitJoin(std::string const &);

private:
  void    update(std::string const &);
  void    add(std::string const &);
  void    del(std::string const &);
  void    status(std::string const &);
  void    join(std::string const &);
  void    leave(std::string const &);
  void    call(std::string const &);
  void    send(std::string const &);
  void    ping(std::string const &);
  void    ack(std::string const &);
  void    cancel(std::string const &);
  void    bye(std::string const &);

  void    retLogin(int const, std::string const &);
  void    addContact(int const, std::string const &);
  void    deleteContact(int const, std::string const &);
  void    addConversation(int const, std::string const &);
  void    doNothing(int const, std::string const &);
  void    printReturn(int const, std::string const &);
  void    addToConv(int const, std::string const &);
};

#endif
