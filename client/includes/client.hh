#ifndef client_hh
#define client_hh

#include <cstdint>
#include <exception>
#include "QNetwork.hh"
#include "conversationManager.hh"
#include "contactManager.hh"
#include "mainwidget.h"
#include "login.h"
#include "execCmd.hh"

typedef int     (*FuncPtr)();

class Client
{
private:
  contact               *user;
  ICNetwork             *net;
  Mainwidget            *mainWin;
  login                 *loginWin;
  execCmd               *exec;
  conversationManager   conversations;
  contactManager        contacts;
  bool                  connected;

public:
  Client(const char *ip = "127.0.0.1", const char *port = "6667");
  ~Client();

  void    startLogin();
  void    startMain(std::string const &);
  void    sendToCmd(int const, std::string const &) const;
  void    writeToNet(int const cmd, std::string const &args) const;
  bool    isDigits(const std::string &) const;
  void    setConnect(bool const);
  std::vector<unsigned int> charToInt(const unsigned char *) const;

  bool                getConnect() const;
  std::string         getName() const;
  login               *getLoginWin() const;
  Mainwidget          *getMainWin() const;

  conversationManager getConvManager() const;
  contactManager      getContacts() const;
  void                setConvManager(conversationManager &);
  void                setContacts(contactManager &);
};

#endif
