//
// BoostNetwork.cpp for BoostNetwork in /home/lopez_t/Babel/API/Network
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Fri Oct 09 14:34:39 2015 Thibaut Lopez
// Last update Sat Nov  7 23:33:44 2015 Thibaut Lopez
//

#include "BoostNetwork.hh"

GlobalIOS			*GlobalIOS::_instance = NULL;

GlobalIOS::GlobalIOS()
  : _isRunning(false)
{
}


GlobalIOS::~GlobalIOS()
{
}

GlobalIOS			*GlobalIOS::getInstance()
{
  if (_instance == NULL)
    _instance = new GlobalIOS();
  return (_instance);
}

boost::asio::io_service		&GlobalIOS::getIOS()
{
  return (this->_ios);
}

void				GlobalIOS::run()
{
  if (this->_isRunning == false)
    {
      this->_ios.run();
      this->_isRunning = true;
    }
}

BoostConnection::BoostConnection(boost::asio::io_service &ios, const std::size_t &lenMax, const std::size_t &id)
  : _socket(ios), _lenMax(lenMax), _id(id), _hr(NULL)
{
  this->_byteRead = new char[this->_lenMax];
}

BoostConnection::~BoostConnection()
{
  delete []this->_byteRead;
}

void				BoostConnection::_handleRead(const boost::system::error_code &err, std::size_t len)
{
  if (!err)
    {
      this->_hr->handleRead(len, *this);
    }
  else if (boost::asio::error::eof == err || boost::asio::error::connection_reset == err)
    {
      this->_socket.close();
      this->_hr->handleDisconnection(*this);
    }
}

void				BoostConnection::_handleWrite(const boost::system::error_code &err, std::size_t len)
{
  (void)len;
  if (boost::asio::error::eof == err || boost::asio::error::connection_reset == err)
    {
      this->_socket.close();
      this->_hr->handleDisconnection(*this);
    }
}

void				BoostConnection::start(IHandler *handler, const std::size_t &firstReadLen)
{
  std::cout << "New client, IP :" << this->_socket.remote_endpoint().address().to_string() << "." << std::endl;
  this->_hr = handler;
  this->_hr->handleAccept(*this);
  this->asyncRead(firstReadLen);
}

bool				BoostConnection::operator==(IConnection &cmp) const
{
  return (this->_id == cmp.getId());
}

bool				BoostConnection::operator!=(IConnection &cmp) const
{
  return (this->_id != cmp.getId());
}

void				BoostConnection::asyncRead(const std::size_t &len)
{
  if (len > this->_lenMax || !this->_socket.is_open())
    return ;
  boost::asio::async_read(this->_socket, boost::asio::buffer(this->_byteRead, len), boost::bind(&BoostConnection::_handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void				BoostConnection::asyncReadSome(const std::size_t &len)
{
  if (len > this->_lenMax || !this->_socket.is_open())
    return ;
  this->_socket.async_read_some(boost::asio::buffer(this->_byteRead, len), boost::bind(&BoostConnection::_handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void				BoostConnection::asyncWrite(const void *buff, const std::size_t &len)
{
  if (!this->_socket.is_open())
    return ;
  boost::asio::async_write(this->_socket, boost::asio::buffer(buff, len), boost::bind(&BoostConnection::_handleWrite, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void				*BoostConnection::getRead() const
{
  return (this->_byteRead);
}

boost::asio::ip::tcp::socket	&BoostConnection::getSocket()
{
  return (this->_socket);
}

std::size_t			BoostConnection::getId() const
{
  return (this->_id);
}

bool				BoostConnection::isOpen() const
{
  return (this->_socket.is_open());
}

BoostSocket::BoostSocket(const std::size_t &firstReadLen, const std::size_t &lenMax)
  : _acceptor((GlobalIOS::getInstance())->getIOS()), _firstReadLen(firstReadLen), _lenMax(lenMax)
{
}

BoostSocket::~BoostSocket()
{
  std::list<IConnection *>::iterator	it;

  it = this->_connections.begin();
  while (it != this->_connections.end())
    {
      delete (*it);
      it = this->_connections.erase(it);
    }
  this->_acceptor.close();
}

std::size_t			BoostSocket::_findId() const
{
  std::size_t			id;
  std::list<IConnection *>::const_iterator	it;

  id = 0;
  it = this->_connections.begin();
  while (it != this->_connections.end())
    {
      if ((*it)->getId() == id)
	{
	  id++;
	  it = this->_connections.begin();
	}
      else
	++it;
    }
  return (id);
}

void				BoostSocket::_startAccept()
{
  BoostConnection		*newConnection;

  newConnection = new BoostConnection((GlobalIOS::getInstance())->getIOS(), this->_lenMax, this->_findId());
  this->_acceptor.async_accept(newConnection->getSocket(), boost::bind(&BoostSocket::_handleAccept, this, newConnection, boost::asio::placeholders::error));
  this->_connections.push_back(newConnection);
}

void				BoostSocket::_checkForDisconnected()
{
  std::list<IConnection *>::iterator	it;

  it = this->_connections.begin();
  while (it != this->_connections.end())
    {
      if ((*it)->isOpen() == false)
	{
	  delete (*it);
	  it = this->_connections.erase(it);
	}
      else
	++it;
    }
}

void				BoostSocket::_handleAccept(BoostConnection *newConnection, const boost::system::error_code &err)
{
  std::string			ip;
  std::list<std::string>::iterator	found;

  this->_checkForDisconnected();
  if (!err)
    {
      ip = (newConnection->getSocket()).remote_endpoint().address().to_string();
      if (this->_backlog > 0 &&
	  (this->_acceptedIP.size() == 0 ||
	   *(found = std::find(this->_acceptedIP.begin(), this->_acceptedIP.end(), ip)) == ip))
	{
	  this->_backlog--;
	  if (this->_acceptedIP.size() != 0)
	    this->_acceptedIP.erase(found);
	  newConnection->start(this->_hr, this->_firstReadLen);
	}
      else
	{
	  this->_connections.erase(std::find(this->_connections.begin(), this->_connections.end(), newConnection));
	  delete newConnection;
	}
    }
  this->_startAccept();
}

bool				BoostSocket::initServer(const uint16_t &port, IHandler *handler, const std::size_t &backlog)
{
  std::cout << "Trying launching a server, using port " << port << "..." << std::endl;
  try
    {
      this->_acceptor.open(boost::asio::ip::tcp::v4());
      this->_acceptor.set_option(boost::asio::ip::tcp::socket::reuse_address(true));
      this->_acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
      this->_acceptor.listen();
      this->_hr = handler;
      this->_backlog = backlog;
      std::cout << "Server successfuly launched." << std::endl;
      this->_startAccept();
    }
  catch (boost::system::system_error &err)
    {
      std::cerr << "Error : " << err.what() << std::endl;
      return (false);
    }
  return (true);
}

std::uint16_t			BoostSocket::getPort() const
{
  return (this->_acceptor.local_endpoint().port());
}

const std::list<IConnection *>	BoostSocket::getClients() const
{
  return (this->_connections);
}

IConnection			*BoostSocket::getClient(const std::size_t &id) const
{
  std::list<IConnection *>::const_iterator	it;

  it = this->_connections.begin();
  while (it != this->_connections.end())
    {
      if ((*it)->getId() == id)
	return (*it);
      ++it;
    }
  return (NULL);
}

void				BoostSocket::setNextConnection(const std::string &ip)
{
  std::list<std::string>::iterator	it;
  boost::system::error_code	ec;
  boost::asio::ip::address::from_string(ip, ec);

  if (ec)
    return ;
  it = std::find(this->_acceptedIP.begin(), this->_acceptedIP.end(), ip);
  if (*it == ip)
    return ;
  if (this->_backlog - this->_acceptedIP.size() == 0)
    this->_backlog++;
  this->_acceptedIP.push_back(ip);
}

void				BoostSocket::eraseClient(const std::size_t &id)
{
  std::list<IConnection *>::iterator	it;

  it = this->_connections.begin();
  while (it != this->_connections.end())
    {
      if ((*it)->getId() == id)
	{
	  delete (*it);
	  it = this->_connections.erase(it);
	}
      else
	++it;
    }
}

void				BoostSocket::loop()
{
  (GlobalIOS::getInstance())->run();
}

BoostTimer::BoostTimer()
  : _timer((GlobalIOS::getInstance())->getIOS()), _waiter(NULL)
{
}

BoostTimer::~BoostTimer()
{
}

void				BoostTimer::_handleWait(const int &key, const boost::system::error_code &err)
{
  if (!err)
    {
      this->_waiter->handleWait(key);
    }
}

void				BoostTimer::setWaiter(IWaiter *waiter)
{
  this->_waiter = waiter;
}

void				BoostTimer::setDeadLine(const std::size_t sec, const std::size_t usec, const int &key)
{
  this->_timer.expires_from_now(boost::posix_time::seconds(sec) + boost::posix_time::milliseconds(usec));
  this->_timer.async_wait(boost::bind(&BoostTimer::_handleWait, this, key, boost::asio::placeholders::error));
}

void				BoostTimer::stop()
{
  this->_timer.cancel();
}

void				BoostTimer::loop()
{
  (GlobalIOS::getInstance())->run();
}
