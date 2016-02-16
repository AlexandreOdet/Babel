//
// INetwork.hh for INetwork in /home/lopez_t/Babel/API/interface
// 
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
// 
// Started on  Thu Oct  8 15:54:43 2015 Thibaut Lopez
// Last update Sat Nov  7 15:13:21 2015 Thibaut Lopez
//

#ifndef	INETWORK_HH
#define	INETWORK_HH

#include <iostream>
#include <cstdint>
#include <limits>
#include <list>
#ifndef _WIN32
#include <sys/time.h>
#endif

class	IConnection;

class	IHandler
{
public:
  virtual ~IHandler() {}

  virtual void			handleAccept(IConnection &) = 0;
  virtual void			handleRead(const std::size_t &, IConnection &) = 0;
  virtual void			handleDisconnection(IConnection &) = 0;
};

class	IConnection
{
public:
  virtual ~IConnection() {}

  virtual bool			operator==(IConnection &cmp) const = 0;
  virtual bool			operator!=(IConnection &cmp) const = 0;
  virtual void			asyncRead(const std::size_t &len) = 0;
  virtual void			asyncReadSome(const std::size_t &len) = 0;
  virtual void			asyncWrite(const void *buff, const std::size_t &len) = 0;
  virtual void			*getRead() const = 0;
  virtual std::size_t		getId() const = 0;
  virtual bool			isOpen() const = 0;
};

class	ISocket
{
public:
  virtual ~ISocket() {}

  virtual bool			initServer(const uint16_t &port, IHandler *handler, const std::size_t &backlog = std::numeric_limits<std::size_t>::max()) = 0;
  virtual std::uint16_t		getPort() const = 0;
  virtual const std::list<IConnection *>	getClients() const = 0;
  virtual IConnection		*getClient(const std::size_t &id) const = 0;
  virtual void			setNextConnection(const std::string &ip) = 0;
  virtual void			eraseClient(const std::size_t &id) = 0;
  virtual void			loop() = 0;
};

class	IWaiter
{
public:
  virtual ~IWaiter() {}

  virtual void			handleWait(const int &key = 0) = 0;
};

class	ITimer
{
public:
  virtual ~ITimer() {}

  virtual void			setWaiter(IWaiter *waiter) = 0;
  virtual void			setDeadLine(const std::size_t sec, const std::size_t usec, const int &key) = 0;
  virtual void			stop() = 0;
  virtual void			loop() = 0;
};

#endif
