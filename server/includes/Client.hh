//
// Client.hh for Client in /home/lopez_t/Babel/server
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Sun Oct 25 11:02:13 2015 Thibaut Lopez
// Last update Sat Nov  7 18:25:01 2015 Thibaut Lopez
//

#ifndef	CLIENT_HH
#define	CLIENT_HH

#include <iostream>
#include <string>
#include "BoostNetwork.hh"
#include "UserInformation.hh"
#include "Channel.hh"

enum	eClientState
  {
    READ_HEADER,
    READ_COMMAND
  };

enum	eTimer
  {
    PING = 0,
    PONG = 1,
    CALL = 2
  };

class	Client
{
private:
  bool				_isHandShaken;
  int				_pongValue;
  int				_key;
  ITimer			*_ping;
  ITimer			*_pong;
  eClientState			_state;
  UserInformation		*_identity;
  std::list<std::string>	_requests;
  std::list<Channel *>		_channels;
  std::list<std::string>	_calls;

public:
  Client(IWaiter *waiter, const int &key);
  virtual ~Client();

  void				startWFPing();
  void				cancelWFPing();
  void				startWFPong();
  void				cancelWFPong();
  const bool			&isHandShaken() const;
  void				isHandShaken(const bool &isHandShaken);
  const int			&pongValue() const;
  void				pongValue(const int &pongValue);
  const eClientState		&state() const;
  void				state(const eClientState &state);
  UserInformation		*identity() const;
  void				identity(UserInformation *identity);
  bool				addRequest(const std::string &chan);
  bool				delRequest(const std::string &chan);
  const std::list<std::string>	&getRequests() const;
  bool				addChannel(Channel *chan);
  bool				delChannel(Channel *chan);
  bool				findChannel(Channel *chan);
  const std::list<Channel *>	&getChannels() const;
  bool				addCall(const std::string &login);
  bool				delCall(const std::string &login);
};

#endif
