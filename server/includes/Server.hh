//
// Server.hh for Server in /home/lopez_t/Babel/src
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Fri Oct 16 11:58:58 2015 Thibaut Lopez
// Last update Sat Nov  7 18:26:45 2015 Thibaut Lopez
//

#ifndef	SERVER_HH
#define	SERVER_HH

#include "INetwork.hh"
#include "BoostNetwork.hh"
#include "Client.hh"
#include "UserDB.hh"
#include "ChannelManager.hh"
#include "PrivateManager.hh"
#include "Response.hh"
#include <stdlib.h>
#include <map>
#include <sstream>

class	Server
  : public IHandler, public IWaiter
{
private:
  ISocket			 *_srv;
  IUserDB			 *_udb;
  ChannelManager		 _cManager;
  PrivateManager		 _pManager;
  int				 _key;
  typedef	bool		 (Server::*s_func)(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  std::map<int, s_func>		 _commands;
  std::map<int, Client *>	 _clients;
  typedef	void		 (Server::*t_func)(Client *clt, IConnection *connection);
  std::map<eTimer, t_func>	 _timer;
  Response			 _rsp;
  Status			 _converter;
  bool				 _connect(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _register(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _add(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _delete(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _status(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _join(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _leav(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _call(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _cancel(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _listFriends(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _listChannels(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _send(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _pong(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _disconnect(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _quit(void *com, const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _findHeaderLen(const std::size_t &len, IConnection &connection, Client *clt);
  bool				 _parseCommand(const std::size_t &len, IConnection &connection, Client *clt);
  void				 _timerPing(Client *clt, IConnection *connection);
  void				 _timerPong(Client *clt, IConnection *connection);
  void				 _changeStatus(Client *clt, const eStatus &status) const;
  void				 _disconnection(Client *clt);

public:
  Server();
  virtual ~Server();

  void				 handleAccept(IConnection &connection);
  void				 handleRead(const std::size_t &len, IConnection &connection);
  void				 handleDisconnection(IConnection &connection);
  void				 handleWait(const int &key = 0);
  void				 init(const std::uint16_t &port);
  void				 run();
};

#endif
