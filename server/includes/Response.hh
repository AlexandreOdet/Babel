//
// Response.hh for Response in /home/lopez_t/Babel/server
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Thu Oct 29 14:10:25 2015 Thibaut Lopez
// Last update Sat Nov  7 17:42:31 2015 Thibaut Lopez
//

#ifndef	RESPONSE_HH
#define	RESPONSE_HH

#include "INetwork.hh"
#include "UserInformation.hh"
#include "Channel.hh"

class	Response
{
public:
  Response();
  virtual ~Response();

  void	add(IConnection &connection, const std::string &login) const;
  void	rdelete(IConnection &connection, const std::string &login) const;
  void	status(IConnection &connection, const std::string &login, const std::string &status) const;
  void	join(IConnection &connection, const std::string &login) const;
  void	join(IConnection &connection, const std::string &chan, const std::string &login) const;
  void	leave(IConnection &connection, const std::string &chan, const std::string &login) const;
  void	call(IConnection &connection, const std::string &login) const;
  void	cancel(IConnection &connection, const std::string &login) const;
  void	send(IConnection &connection, const std::string &chan, const std::string &login, const std::string &message) const;
  void	ping(IConnection &connection, const int &pongValue) const;
  void	r100(IConnection &connection) const;
  void	r180(IConnection &connection) const;
  void	r200(IConnection &connection) const;
  void	r201(IConnection &connection, const UserInformation *id, const bool &isFriend) const;
  void	r202(IConnection &connection, const std::string &chan, const bool &isPrivate, const std::list<std::string> &peoples) const;
  void	r203(IConnection &connection) const;
  void	r204(IConnection &connection, const uint16_t &port) const;
  void	r301(IConnection &connection) const;
  void	r302(IConnection &connection) const;
  void	r303(IConnection &connection, const std::string &str) const;
  void	r304(IConnection &connection) const;
  void	r311(IConnection &connection, const std::string &login) const;
  void	r312(IConnection &connection) const;
  void	r313(IConnection &connection) const;
  void	r314(IConnection &connection) const;
  void	r321(IConnection &connection) const;
  void	r341(IConnection &connection) const;
  void	r342(IConnection &connection) const;
  void	r351(IConnection &connection, const std::string &friend_) const;
  void	r361(IConnection &connection) const;
  void	r362(IConnection &connection, const std::string &status) const;
  void	r371(IConnection &connection) const;
  void	r372(IConnection &connection) const;
  void	r373(IConnection &connection) const;
  void	r381(IConnection &connection) const;
  void	r391(IConnection &connection) const;
  void	r392(IConnection &connection) const;
  void	r401(IConnection &connection) const;
  void	r404(IConnection &connection) const;
  void	r408(IConnection &connection) const;
  void	r418(IConnection &connection) const;
  void	r486(IConnection &connection) const;
  void	r500(IConnection &connection) const;
};

#endif
