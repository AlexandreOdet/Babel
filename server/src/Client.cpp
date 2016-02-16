//
// Client.cpp for Client in /home/lopez_t/Babel/server
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Sun Oct 25 11:02:13 2015 Thibaut Lopez
// Last update Sat Nov  7 18:25:27 2015 Thibaut Lopez
//

#include "Client.hh"

Client::Client(IWaiter *waiter, const int &key)
  : _isHandShaken(false), _pongValue(-1), _key(key), _ping(new BoostTimer()), _pong(new BoostTimer()), _state(READ_HEADER), _identity(NULL)
{
  this->_ping->setWaiter(waiter);
  this->_pong->setWaiter(waiter);
}

Client::~Client()
{
  delete (this->_ping);
  delete (this->_pong);
}

void				Client::startWFPing()
{
  this->_ping->setDeadLine(10, 0, (this->_key << 2) + PING);
}

void				Client::cancelWFPing()
{
  this->_ping->stop();
}

void				Client::startWFPong()
{
  this->_pong->setDeadLine(5, 0, (this->_key << 2) + PONG);
}

void				Client::cancelWFPong()
{
  this->_pong->stop();
}

const bool			&Client::isHandShaken() const
{
  return (this->_isHandShaken);
}

void				Client::isHandShaken(const bool &isHandShaken)
{
  this->_isHandShaken = isHandShaken;
}

const int			&Client::pongValue() const
{
  return (this->_pongValue);
}

void				Client::pongValue(const int &pongValue)
{
  this->_pongValue = pongValue;
}

const eClientState		&Client::state() const
{
  return (this->_state);
}

void				Client::state(const eClientState &state)
{
  this->_state = state;
}

UserInformation			*Client::identity() const
{
  return (this->_identity);
}

void				Client::identity(UserInformation *identity)
{
  this->_identity = identity;
}

bool				Client::addRequest(const std::string &chan)
{
  if (std::find(this->_requests.begin(), this->_requests.end(), chan) == this->_requests.end())
    {
      this->_requests.push_back(chan);
      return (true);
    }
  else
    return (false);
}

bool				Client::delRequest(const std::string &chan)
{
  std::list<std::string>::iterator	it;

  it = std::find(this->_requests.begin(), this->_requests.end(), chan);
  if (it != this->_requests.end())
    {
      this->_requests.erase(it);
      return (true);
    }
  else
    return (false);
}

const std::list<std::string>	&Client::getRequests() const
{
  return (this->_requests);
}

bool				Client::addChannel(Channel *chan)
{
  if (std::find(this->_channels.begin(), this->_channels.end(), chan) == this->_channels.end())
    {
      this->_channels.push_back(chan);
      return (true);
    }
  else
    return (false);
}

bool				Client::delChannel(Channel *chan)
{
  std::list<Channel *>::iterator	it;

  it = std::find(this->_channels.begin(), this->_channels.end(), chan);
  if (it != this->_channels.end())
    {
      this->_channels.erase(it);
      return (true);
    }
  else
    return (false);
}

bool				Client::findChannel(Channel *chan)
{
  std::list<Channel *>::iterator	it;

  it = std::find(this->_channels.begin(), this->_channels.end(), chan);
  if (it != this->_channels.end())
    return (true);
  else
    return (false);
}

const std::list<Channel *>	&Client::getChannels() const
{
  return (this->_channels);
}

bool				Client::addCall(const std::string &chan)
{
  if (std::find(this->_calls.begin(), this->_calls.end(), chan) == this->_calls.end())
    {
      this->_calls.push_back(chan);
      return (true);
    }
  else
    return (false);
}

bool				Client::delCall(const std::string &chan)
{
  std::list<std::string>::iterator	it;

  it = std::find(this->_calls.begin(), this->_calls.end(), chan);
  if (it != this->_calls.end())
    {
      this->_calls.erase(it);
      return (true);
    }
  else
    return (false);
}

