//
// Response.cpp for Response in /home/lopez_t/Babel/server
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Thu Oct 29 14:10:25 2015 Thibaut Lopez
// Last update Sat Nov  7 17:43:20 2015 Thibaut Lopez
//

#include "Response.hh"

Response::Response()
{
}

Response::~Response()
{
}

void		Response::add(IConnection &connection, const std::string &login) const
{
  int		header;

  std::cout << "Response add" << std::endl;
  header = 4;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(login.c_str(), login.size());
}

void		Response::rdelete(IConnection &connection, const std::string &login) const
{
  int		header;

  std::cout << "Response delete" << std::endl;
  header = 5;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(login.c_str(), login.size());
}

void		Response::status(IConnection &connection, const std::string &login, const std::string &status) const
{
  int		header;

  header = 6;
  connection.asyncWrite(&header, sizeof(int));
  header = status.size() + login.size() + 1;
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(login.c_str(), login.size());
  connection.asyncWrite(" ", 1);
  connection.asyncWrite(status.c_str(), status.size());
}

void		Response::join(IConnection &connection, const std::string &login) const
{
  int		header;

  std::cout << "Response join1" << std::endl;
  header = 7;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(login.c_str(), login.size());
}

void		Response::join(IConnection &connection, const std::string &chan, const std::string &login) const
{
  int		header;

  std::cout << "Response join2" << std::endl;
  header = 7;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size() + chan.size() + 1;
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(chan.c_str(), chan.size());
  connection.asyncWrite(" ", 1);
  connection.asyncWrite(login.c_str(), login.size());
}

void		Response::leave(IConnection &connection, const std::string &chan, const std::string &login) const
{
  int		header;

  std::cout << "Response leave" << std::endl;
  header = 8;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size() + chan.size() + 1;
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(chan.c_str(), chan.size());
  connection.asyncWrite(" ", 1);
  connection.asyncWrite(login.c_str(), login.size());
}

void		Response::call(IConnection &connection, const std::string &login) const
{
  int		header;

  std::cout << "Response call" << std::endl;
  header = 9;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(login.c_str(), login.size());
}

void		Response::cancel(IConnection &connection, const std::string &login) const
{
  int		header;

  std::cout << "Response cancel" << std::endl;
  header = 10;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(login.c_str(), login.size());
}

void		Response::send(IConnection &connection, const std::string &chan, const std::string &login, const std::string &message) const
{
  int		header;

  std::cout << "Response send" << std::endl;
  header = 13;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size() + chan.size() + message.size() + 2;
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(chan.c_str(), chan.size());
  connection.asyncWrite(" ", 1);
  connection.asyncWrite(login.c_str(), login.size());
  connection.asyncWrite(" ", 1);
  connection.asyncWrite(message.c_str(), message.size());
}

void		Response::ping(IConnection &connection, const int &pongValue) const
{
  int		header;

  std::cout << "Je lance un ping: " << pongValue << std::endl;
  header = 18;
  connection.asyncWrite(&header, sizeof(int));
  header = sizeof(int);
  connection.asyncWrite(&header, sizeof(int));
  header = pongValue;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r100(IConnection &connection) const
{
  int		header;

  std::cout << "Response 100" << std::endl;
  header = 100;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r180(IConnection &connection) const
{
  int		header;

  std::cout << "Response 180" << std::endl;
  header = 180;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r200(IConnection &connection) const
{
  int		header;

  std::cout << "Response 200" << std::endl;
  header = 200;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r201(IConnection &connection, const UserInformation *id, const bool &isFriend) const
{
  int		header;

  if (id == NULL)
    return ;
  std::cout << "Response 201" << std::endl;
  std::string	status((Status()).convert(id->getStatus()));
  header = 201;
  connection.asyncWrite(&header, sizeof(int));
  header = id->getLogin().size() + status.size() + 3;
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(id->getLogin().c_str(), id->getLogin().size());
  connection.asyncWrite(" ", 1);
  connection.asyncWrite(status.c_str(), status.size());
  if (isFriend)
    connection.asyncWrite(" C", 2);
  else
    connection.asyncWrite(" P", 2);
}

void		Response::r202(IConnection &connection, const std::string &chan, const bool &isPrivate, const std::list<std::string> &peoples) const
{
  int		header;
  std::list<std::string>::const_iterator	it;

  std::cout << "Response 202" << std::endl;
  header = 202;
  connection.asyncWrite(&header, sizeof(int));
  header = chan.size() + 2;
  it = peoples.begin();
  while (it != peoples.end())
    {
      header += (*it).size() + 1;
      ++it;
    }
  connection.asyncWrite(&header, sizeof(int));
  if (isPrivate)
    connection.asyncWrite("P ", 2);
  else
    connection.asyncWrite("p ", 2);
  connection.asyncWrite(chan.c_str(), chan.size());
  it = peoples.begin();
  while (it != peoples.end())
    {
      connection.asyncWrite(" ", 1);
      connection.asyncWrite((*it).c_str(), (*it).size());
      ++it;
    }
}

void		Response::r203(IConnection &connection) const
{
  int		header;

  std::cout << "Response 203" << std::endl;
  header = 203;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r204(IConnection &connection, const uint16_t &port) const
{
  int		header;

  std::cout << "Response 204" << std::endl;
  header = 204;
  connection.asyncWrite(&header, sizeof(int));
  header = sizeof(uint16_t);
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(&port, sizeof(uint16_t));
}

void		Response::r301(IConnection &connection) const
{
  int		header;

  std::cout << "Response 301" << std::endl;
  header = 301;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r302(IConnection &connection) const
{
  int		header;

  std::cout << "Response 302" << std::endl;
  header = 302;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r303(IConnection &connection, const std::string &str) const
{
  int		header;

  std::cout << "Response 303" << std::endl;
  header = 303;
  connection.asyncWrite(&header, sizeof(int));
  header = str.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(str.c_str(), str.size());
}

void		Response::r304(IConnection &connection) const
{
  int		header;

  std::cout << "Response 304" << std::endl;
  header = 304;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r311(IConnection &connection, const std::string &login) const
{
  int		header;

  std::cout << "Response 311" << std::endl;
  header = 311;
  connection.asyncWrite(&header, sizeof(int));
  header = login.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(login.c_str(), login.size());
}

void		Response::r312(IConnection &connection) const
{
  int		header;

  std::cout << "Response 312" << std::endl;
  header = 312;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r313(IConnection &connection) const
{
  int		header;

  std::cout << "Response 313" << std::endl;
  header = 313;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r314(IConnection &connection) const
{
  int		header;

  std::cout << "Response 314" << std::endl;
  header = 314;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r321(IConnection &connection) const
{
  int		header;

  std::cout << "Response 321" << std::endl;
  header = 321;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r341(IConnection &connection) const
{
  int		header;

  std::cout << "Response 341" << std::endl;
  header = 341;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r342(IConnection &connection) const
{
  int		header;

  std::cout << "Response 342" << std::endl;
  header = 342;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r351(IConnection &connection, const std::string &friend_) const
{
  int		header;

  std::cout << "Response 351" << std::endl;
  header = 351;
  connection.asyncWrite(&header, sizeof(int));
  header = friend_.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(friend_.c_str(), friend_.size());
}

void		Response::r361(IConnection &connection) const
{
  int		header;

  std::cout << "Response 361" << std::endl;
  header = 361;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r362(IConnection &connection, const std::string &status) const
{
  int		header;

  std::cout << "Response 362" << std::endl;
  header = 362;
  connection.asyncWrite(&header, sizeof(int));
  header = status.size();
  connection.asyncWrite(&header, sizeof(int));
  connection.asyncWrite(status.c_str(), status.size());
}

void		Response::r371(IConnection &connection) const
{
  int		header;

  std::cout << "Response 371" << std::endl;
  header = 371;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r372(IConnection &connection) const
{
  int		header;

  std::cout << "Response 372" << std::endl;
  header = 372;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r373(IConnection &connection) const
{
  int		header;

  std::cout << "Response 373" << std::endl;
  header = 373;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r381(IConnection &connection) const
{
  int		header;

  std::cout << "Response 381" << std::endl;
  header = 381;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r391(IConnection &connection) const
{
  int		header;

  std::cout << "Response 391" << std::endl;
  header = 391;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r392(IConnection &connection) const
{
  int		header;

  std::cout << "Response 392" << std::endl;
  header = 392;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r401(IConnection &connection) const
{
  int		header;

  std::cout << "Response 401" << std::endl;
  header = 401;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r404(IConnection &connection) const
{
  int		header;

  std::cout << "Response 404" << std::endl;
  header = 404;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r408(IConnection &connection) const
{
  int		header;

  std::cout << "Response 408" << std::endl;
  header = 408;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r418(IConnection &connection) const
{
  int		header;

  std::cout << "Response 418" << std::endl;
  header = 418;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r486(IConnection &connection) const
{
  int		header;

  std::cout << "Response 486" << std::endl;
  header = 486;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}

void		Response::r500(IConnection &connection) const
{
  int		header;

  std::cout << "Response 500" << std::endl;
  header = 500;
  connection.asyncWrite(&header, sizeof(int));
  header = 0;
  connection.asyncWrite(&header, sizeof(int));
}
