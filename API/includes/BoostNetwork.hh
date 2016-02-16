//
// BoostSocket.hh for BoostSocket in /home/lopez_t/Babel/API/Network
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Fri Oct 09 14:34:39 2015 Thibaut Lopez
// Last update Sat Nov  7 15:18:49 2015 Thibaut Lopez
//

#ifndef	BOOSTNETWORK_HH
#define	BOOSTNETWORK_HH

#include "INetwork.hh"
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

class	GlobalIOS
{
private:
  static GlobalIOS		 *_instance;
  boost::asio::io_service	 _ios;
  bool				 _isRunning;
  GlobalIOS();
  ~GlobalIOS();
public:
  static GlobalIOS		 *getInstance();
  boost::asio::io_service	 &getIOS();
  void				 run();
};

class	BoostConnection
  : public IConnection
{
private:
  boost::asio::ip::tcp::socket	 _socket;
  std::size_t			 _lenMax;
  std::size_t			 _id;
  char				 *_byteRead;
  IHandler			 *_hr;
  void				 _handleRead(const boost::system::error_code &err, std::size_t len);
  void				 _handleWrite(const boost::system::error_code &err, std::size_t len);

public:
  BoostConnection(boost::asio::io_service &ios, const std::size_t &lenMax, const std::size_t &id);
  virtual ~BoostConnection();

  void				 start(IHandler *handler, const std::size_t &firstReadLen);
  bool				 operator==(IConnection &cmp) const;
  bool				 operator!=(IConnection &cmp) const;
  void				 asyncRead(const std::size_t &len);
  void				 asyncReadSome(const std::size_t &len);
  void				 asyncWrite(const void *buff, const std::size_t &len);
  void				 *getRead() const;
  boost::asio::ip::tcp::socket	 &getSocket();
  std::size_t			 getId() const;
  bool				 isOpen() const;
};


class	BoostSocket
  : public ISocket
{
private:
  boost::asio::ip::tcp::acceptor _acceptor;
  std::size_t			 _firstReadLen;
  std::size_t			 _lenMax;
  std::size_t			 _backlog;
  std::list<std::string>	 _acceptedIP;
  IHandler			 *_hr;
  std::list<IConnection *>	 _connections;
  std::size_t			 _findId() const;
  void				 _startAccept();
  void				 _checkForDisconnected();
  void				 _handleAccept(BoostConnection *newConnection, const boost::system::error_code &err);

public:
  explicit BoostSocket(const std::size_t &firstReadLen, const std::size_t &lenMax);
  virtual ~BoostSocket();

  bool				 initServer(const uint16_t &port, IHandler *handler, const std::size_t &backlog = std::numeric_limits<std::size_t>::max());
  std::uint16_t			 getPort() const;
  const std::list<IConnection *> getClients() const;
  IConnection			 *getClient(const std::size_t &id) const;
  void				 setNextConnection(const std::string &ip);
  void				 eraseClient(const std::size_t &id);
  void				 loop();
};

class	BoostTimer
  : public ITimer
{
private:
  boost::asio::deadline_timer	 _timer;
  IWaiter			 *_waiter;
  void				 _handleWait(const int &key, const boost::system::error_code &err);

public:
  BoostTimer();
  virtual ~BoostTimer();

  void				 setWaiter(IWaiter *waiter);
  void				 setDeadLine(const std::size_t sec, const std::size_t usec, const int &key);
  void				 stop();
  void				 loop();
};

#endif
