//
// Server.cpp for Server in /home/lopez_t/Babel/src
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Fri Oct 16 11:58:58 2015 Thibaut Lopez
// Last update Sun Nov  8 21:28:31 2015 Thibaut Lopez
//

#include "Server.hh"
#include <vector>

Server::Server()
  : _udb(new UserDB(".users.json")), _cManager(this->_udb), _pManager(this->_udb)
{
  this->_srv = new BoostSocket(sizeof(int) * 2, 512);
  this->_commands[1] = &Server::_connect;
  this->_commands[2] = &Server::_register;
  this->_commands[4] = &Server::_add;
  this->_commands[5] = &Server::_delete;
  this->_commands[6] = &Server::_status;
  this->_commands[7] = &Server::_join;
  this->_commands[8] = &Server::_leav;
  this->_commands[9] = &Server::_call;
  this->_commands[10] = &Server::_cancel;
  this->_commands[11] = &Server::_listFriends;
  this->_commands[12] = &Server::_listChannels;
  this->_commands[13] = &Server::_send;
  this->_commands[19] = &Server::_pong;
  this->_commands[20] = &Server::_disconnect;
  this->_commands[21] = &Server::_quit;
  this->_timer[PING] = &Server::_timerPing;
  this->_timer[PONG] = &Server::_timerPong;
}

Server::~Server()
{
  delete this->_srv;
  delete this->_udb;
}

bool				Server::_connect(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  std::string			name, pass;
  std::map<int, Client *>::iterator	it;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id != NULL)
    {
      this->_rsp.r311(connection, id->getLogin());
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  std::istringstream		ss(str);

  ss >> name >> pass;
  if (ss.eof())
    {
      id = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(name)));
      if (id == NULL)
	{
	  this->_rsp.r313(connection);
	  return (true);
	}
      if (id->getPassword() != pass)
	{
	  this->_rsp.r312(connection);
	  return (true);
	}
      it = this->_clients.begin();
      while (it != this->_clients.end())
	{
	  if ((*it).second->identity() != NULL && *((*it).second->identity()) == *id)
	    {
	      this->_rsp.r314(connection);
	      return (true);
	    }
	  ++it;
	}
      clt->identity(id);
      this->_changeStatus(clt, CONNECTED);
      this->_rsp.r200(connection);
      this->_listFriends(NULL, 0, connection, clt);
      this->_listChannels(NULL, 0, connection, clt);
    }
  else
    this->_rsp.r302(connection);
  return (true);
}

bool				Server::_register(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  std::string			name, pass;

  (void)clt;
  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  std::istringstream		ss(str);
  ss >> name >> pass;
  if (ss.eof())
    {
      id = new UserInformation(name, pass);
      if (this->_udb->insert(id) == true)
	this->_rsp.r200(connection);
      else
	{
	  this->_rsp.r321(connection);
	  delete id;
	}
    }
  else
    this->_rsp.r302(connection);
  return (true);
}

bool				Server::_add(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  UserInformation		*toAdd;
  std::map<int, Client *>::iterator	it;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  toAdd = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(str)));
  if (toAdd == NULL)
    {
      this->_rsp.r303(connection, str);
      return (true);
    }
  if (std::find(id->getContacts().begin(), id->getContacts().end(), str) != id->getContacts().end())
    {
      this->_rsp.r341(connection);
      return (true);
    }
  if (toAdd->getStatus() != DISCONNECTED)
    {
      it = this->_clients.begin();
      while (it != this->_clients.end() && (*it).second->identity() != toAdd)
	++it;
    }
  else
    it = this->_clients.end();
  if (toAdd->addPending(id->getLogin()))
    {
      if (std::find(id->getPendings().begin(), id->getPendings().end(), str) != id->getPendings().end())
	{
	  id->delPending(str);
	  toAdd->delPending(id->getLogin());
	  id->addContact(str);
	  toAdd->addContact(id->getLogin());
	  this->_listFriends(NULL, 0, connection, clt);
	  if (it != this->_clients.end())
	    this->_listFriends(NULL, 0, *(this->_srv->getClient((*it).first)), (*it).second);
	}
      else
	{
	  this->_rsp.r100(connection);
	  if (it != this->_clients.end())
	    this->_rsp.add(*(this->_srv->getClient((*it).first)), id->getLogin());
	}
    }
  else
    this->_rsp.r342(connection);
  return (true);
}

bool				Server::_delete(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  UserInformation		*toDel;
  std::map<int, Client *>::iterator	it;
  Channel			*chan;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  toDel = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(str)));
  if (toDel == NULL)
    {
      this->_rsp.r303(connection, str);
      return (true);
    }
  if (!id->delContact(str))
    {
      if (!id->delPending(str))
	this->_rsp.r303(connection, str);
      else
	this->_rsp.r200(connection);
    }
  else
    {
      this->_rsp.r200(connection);
      if (toDel->delContact(id->getLogin()) && toDel->getStatus() != DISCONNECTED)
	{
	  it = this->_clients.begin();
	  while (it != this->_clients.end() && (*it).second->identity() != toDel)
	    ++it;
	  if (it != this->_clients.end())
	    this->_rsp.rdelete(*(this->_srv->getClient((*it).first)), id->getLogin());
	  chan = this->_pManager.getPrivate(str, id->getLogin());
	  if (chan != NULL)
	    {
	      clt->delChannel(chan);
	      (*it).second->delChannel(chan);
	      this->_pManager.removePrivate(chan);
	      this->_rsp.leave(connection, toDel->getLogin(), toDel->getLogin());
	      this->_rsp.leave(*(this->_srv->getClient((*it).first)), id->getLogin(), id->getLogin());
	    }
	}
    }

  return (true);
}

bool				Server::_status(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  eStatus			status;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  status = this->_converter.convert(str);
  if (status == UNKNOWN)
    {
      this->_rsp.r303(connection, str);
      return (true);
    }
  if (status == DISCONNECTED)
    {
      this->_rsp.r361(connection);
      return (true);
    }
  if (status == clt->identity()->getStatus())
    {
      this->_rsp.r362(connection, str);
      return (true);
    }
  this->_changeStatus(clt, status);
  this->_rsp.r200(connection);
  return (true);
}

bool				Server::_join(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  Channel			*chan;
  UserInformation		*ui;
  std::map<int, Client *>::iterator	it;
  std::list<std::string>	peoples;
  std::list<std::string>::iterator	it2;
  std::list<std::string>	requests;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  chan = this->_cManager.getChannel(str);
  if (chan == NULL)
    chan = this->_pManager.getPrivate(str, id->getLogin());
  if (chan != NULL)
    {
      if (chan->addPeople(id->getLogin()))
	{
	  peoples = chan->getPeoples();
	  it2 = peoples.begin();
	  while (it2 != peoples.end())
	    {
	      if ((*it2) != id->getLogin())
		{
		  ui = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(*it2)));
		  if (ui != NULL)
		    {
		      it = this->_clients.begin();
		      while (it != this->_clients.end() && (*it).second->identity() != ui)
			++it;
		      if (it != this->_clients.end())
			this->_rsp.join(*(this->_srv->getClient((*it).first)), chan->getName(), id->getLogin());
		    }
		}
	      ++it2;
	    }
	  clt->addChannel(chan);
	  this->_rsp.r200(connection);
	}
      else
	this->_rsp.r371(connection);
      return (true);
    }
  ui = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(str)));
  if (ui == NULL)
    {
      this->_rsp.r303(connection, str);
      return (true);
    }
  if (ui->getStatus() == DISCONNECTED)
    {
      this->_rsp.r372(connection);
      return (true);
    }
  it = this->_clients.begin();
  while (it != this->_clients.end() && (*it).second->identity() != ui)
    ++it;
  if (it == this->_clients.end())
    {
      ui->setStatus(DISCONNECTED);
      this->_rsp.r372(connection);
      return (true);
    }
  if ((*it).second->addRequest(id->getLogin()))
    {
      requests = clt->getRequests();
      if (std::find(requests.begin(), requests.end(), str) != requests.end())
	{
	  clt->delRequest(str);
	  (*it).second->delRequest(id->getLogin());
	  this->_pManager.addPrivate(str, id->getLogin());
	  chan = this->_pManager.getPrivate(str, id->getLogin());
	  chan->addPeople(str);
	  chan->addPeople(id->getLogin());
	  clt->addChannel(chan);
	  (*it).second->addChannel(chan);
	  this->_listChannels(NULL, 0, connection, clt);
	  this->_listChannels(NULL, 0, *(this->_srv->getClient((*it).first)), (*it).second);
	}
      else
	{
	  this->_rsp.r100(connection);
	  this->_rsp.join(*(this->_srv->getClient((*it).first)), id->getLogin());
	}
    }
  else
    this->_rsp.r373(connection);
  return (true);
}

bool				Server::_leav(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  Channel			*chan;
  UserInformation		*ui;
  std::map<int, Client *>::iterator	it;
  std::list<std::string>	peoples;
  std::list<std::string>::iterator	it2;
  bool				isPrivate = false;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  chan = this->_cManager.getChannel(str);
  if (chan == NULL)
    {
      chan = this->_pManager.getPrivate(str, id->getLogin());
      isPrivate = true;
    }
  if (chan != NULL)
    {
      if (chan->removePeople(id->getLogin()))
	{
	  peoples = chan->getPeoples();
	  it2 = peoples.begin();
	  while (it2 != peoples.end())
	    {
	      ui = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(*it2)));
	      if (ui != NULL)
		{
		  it = this->_clients.begin();
		  while (it != this->_clients.end() && (*it).second->identity() != ui)
		    ++it;
		  if (it != this->_clients.end())
		    {
		      if (isPrivate)
			{
			  this->_rsp.leave(*(this->_srv->getClient((*it).first)), id->getLogin(), id->getLogin());
			  (*it).second->delChannel(chan);
			  (*it).second->delCall(id->getLogin());
			  clt->delCall(str);
			}
		      else
			this->_rsp.leave(*(this->_srv->getClient((*it).first)), chan->getName(), id->getLogin());
		    }
		}
	      ++it2;
	    }
	  clt->delChannel(chan);
	  if (isPrivate)
	    this->_pManager.removePrivate(str, id->getLogin());
	  this->_rsp.r200(connection);
	}
      else
	this->_rsp.r381(connection);
      return (true);
    }
  else
    {
      ui = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(str)));
      if (ui != NULL)
	{
	  it = this->_clients.begin();
	  while (it != this->_clients.end() && (*it).second->identity() != ui)
	    ++it;
	  if (it != this->_clients.end() && (*it).second->delRequest(id->getLogin()))
	    this->_rsp.r200(connection);
	  else
	    this->_rsp.r303(connection, str);
	}
      else
	this->_rsp.r303(connection, str);
    }
  return (true);
}

bool				Server::_call(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  Channel			*chan;
  UserInformation		*ui;
  std::map<int, Client *>::iterator	it;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  chan = this->_cManager.getChannel(str);
  if (chan != NULL && clt->findChannel(chan))
    {
      this->_rsp.r204(connection, chan->getProxyPort());
      return (true);
    }
  chan = this->_pManager.getPrivate(str, id->getLogin());
  if (chan != NULL)
    {
      ui = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(str)));
      if (ui != NULL)
	{
	  it = this->_clients.begin();
	  while (it != this->_clients.end() && (*it).second->identity() != ui)
	    ++it;
	  if (it != this->_clients.end())
	    {
	      if ((*it).second->addCall(id->getLogin()))
		{
		  if (clt->delCall(str))
		    {
		      (*it).second->delCall(id->getLogin());
		      this->_rsp.r204(connection, chan->getProxyPort());
		      this->_rsp.r204(*(this->_srv->getClient((*it).first)), chan->getProxyPort());
		    }
		  else
		    {
		      this->_rsp.r180(connection);
		      this->_rsp.call(*(this->_srv->getClient((*it).first)), id->getLogin());
		    }
		}
	      else
		this->_rsp.r392(connection);
	    }
	}
    }
  else
    this->_rsp.r391(connection);
  return (true);
}

bool				Server::_cancel(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  UserInformation		*ui;
  std::map<int, Client *>::iterator	it;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  ui = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(str)));
  if (ui == NULL)
    {
      this->_rsp.r303(connection, str);
      return (true);
    }
  it = this->_clients.begin();
  while (it != this->_clients.end() && (*it).second->identity() != ui)
    ++it;
  if (it != this->_clients.end())
    {
      if (clt->delCall(str))
	{
	  this->_rsp.r200(connection);
	  this->_rsp.cancel(*(this->_srv->getClient((*it).first)), id->getLogin());
	}
      else
	this->_rsp.r391(connection);
    }
  return (true);
}

bool				Server::_listFriends(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  std::string			friend_;
  std::list<std::string>	friends;
  std::vector<std::string>	list;
  std::vector<std::string>::iterator	it;

  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  if (com != NULL)
    {
      std::string		str(static_cast<char *>(com), len);
      std::istringstream	ss(str);

      while (!ss.eof())
	{
	  ss >> friend_;
	  friends.push_back(friend_);
	}
    }
  list = id->getContacts();
  it = list.begin();
  while (it != list.end())
    {
      if (friends.empty() || std::find(friends.begin(), friends.end(), *it) != friends.end())
	this->_rsp.r201(connection, static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(*it))), true);
      ++it;
    }
  list = id->getPendings();
  it = list.begin();
  while (it != list.end())
    {
      if (friends.empty() || std::find(friends.begin(), friends.end(), *it) != friends.end())
	this->_rsp.r201(connection, static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(*it))), false);
      ++it;
    }
  this->_rsp.r203(connection);
  return (true);
}

bool				Server::_listChannels(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  std::string			channel;
  std::list<std::string>	channels;
  std::list<Channel *>		chan;
  std::list<Channel *>::iterator	it;
  std::map<std::string, std::list<std::string> >	privates;
  std::map<std::string, std::list<std::string> >::iterator	it2;

  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  if (com != NULL)
    {
      std::string		str(static_cast<char *>(com), len);
      std::istringstream	ss(str);

      while (!ss.eof())
	{
	  ss >> channel;
	  channels.push_back(channel);
	}
    }
  chan = this->_cManager.getChannels();
  it = chan.begin();
  while (it != chan.end())
    {
      if (channels.empty() || std::find(channels.begin(), channels.end(), (*it)->getName()) != channels.end())
	this->_rsp.r202(connection, (*it)->getName(), false, (*it)->getPeoples());
      ++it;
    }
  privates = this->_pManager.getPrivatesOf(id->getLogin());
  it2 = privates.begin();
  while (it2 != privates.end())
    {
      this->_rsp.r202(connection, (*it2).first, true, (*it2).second);
      ++it2;
    }
  this->_rsp.r203(connection);
  return (true);
}

bool				Server::_send(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  UserInformation		*id;
  Channel			*chan;
  UserInformation		*ui;
  std::map<int, Client *>::iterator	it;
  std::list<std::string>	peoples;
  std::list<std::string>::iterator	it2;
  std::string			chanName, message;
  std::size_t			pos;
  bool				isPrivate = false;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  id = clt->identity();
  if (id == NULL)
    {
      this->_rsp.r401(connection);
      return (true);
    }
  std::string			str(static_cast<char *>(com), len);
  pos = str.find_first_of(' ');
  if (pos == 0 || pos == std::string::npos)
    {
      this->_rsp.r302(connection);
      return (true);
    }
  chanName = str.substr(0, pos);
  while (pos != str.size() && str.at(pos) == ' ')
    pos++;
  if (pos == str.size())
    {
      this->_rsp.r302(connection);
      return (true);
    }
  message = str.substr(pos, std::string::npos);
  chan = this->_cManager.getChannel(chanName);
  if (chan == NULL)
    {
      isPrivate = true;
      chan = this->_pManager.getPrivate(chanName, id->getLogin());
    }
  if (chan != NULL)
    {
      if (!clt->findChannel(chan))
	{
	  this->_rsp.r381(connection);
	  return (true);
	}
      peoples = chan->getPeoples();
      it2 = peoples.begin();
      while (it2 != peoples.end())
	{
	  if (*it2 != id->getLogin())
	    {
	      ui = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(*it2)));
	      if (ui != NULL)
		{
		  it = this->_clients.begin();
		  while (it != this->_clients.end() && (*it).second->identity() != ui)
		    ++it;
		  if (it != this->_clients.end())
		    {
		      if (isPrivate)
			this->_rsp.send(*(this->_srv->getClient((*it).first)), id->getLogin(), id->getLogin(), message);
		      else
			this->_rsp.send(*(this->_srv->getClient((*it).first)), chanName, id->getLogin(), message);
		    }
		}
	      this->_rsp.r200(connection);
	    }
	  ++it2;
	}
    }
  else
    this->_rsp.r303(connection, chanName);
  return (true);
}

bool				Server::_pong(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  int				*pongValue;

  if (com == NULL || len == 0)
    {
      this->_rsp.r301(connection);
      return (true);
    }
  if (clt->isHandShaken() == true)
    return (true);
  pongValue = static_cast<int *>(com);
  if (clt->pongValue() == *pongValue)
    clt->isHandShaken(true);
  else
    {
      this->_timerPong(clt, &connection);
      return (false);
    }
  return (true);
}

bool				Server::_disconnect(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  if (com != NULL || len != 0)
    {
      this->_rsp.r302(connection);
      return (true);
    }
  this->_disconnection(clt);
  this->_rsp.r200(connection);
  return (true);
}

bool				Server::_quit(void *com, const std::size_t &len, IConnection &connection, Client *clt)
{
  (void)com;
  (void)len;
  this->_disconnection(clt);
  this->_timerPong(clt, &connection);
  return (false);
}

bool				Server::_findHeaderLen(const std::size_t &len, IConnection &connection, Client *clt)
{
  int				*buff;
  bool				exist = true;

  (void)len;
  buff = static_cast<int *>(connection.getRead());
  this->_key = buff[0];
  if (buff[1] > 0)
    {
      connection.asyncRead(buff[1]);
      clt->state(READ_COMMAND);
    }
  else
    {
      if (clt->isHandShaken() || this->_key == 19)
	{
	  try
	    {
	      exist = (this->*_commands.at(this->_key))(NULL, 0, connection, clt);
	    }
	  catch (std::out_of_range &err)
	    {
	      (void)err;
	    }
	  if (exist)
	    connection.asyncRead(sizeof(int) * 2);
	}
      else
	this->_rsp.r304(connection);
    }
  if (exist)
    clt->startWFPing();
  return (exist);
}

bool				Server::_parseCommand(const std::size_t &len, IConnection &connection, Client *clt)
{
  void				*com;
  bool				exist = true;

  (void)len;
  com = connection.getRead();
  if (clt->isHandShaken() || this->_key == 19)
    {
      try
	{
	  exist = (this->*_commands.at(this->_key))(com, len, connection, clt);
	}
      catch (std::out_of_range &err)
	{
	  (void)err;
	}
      if (exist)
	{
	  connection.asyncRead(sizeof(int) * 2);
	  clt->state(READ_HEADER);
	}
    }
  else
    this->_rsp.r304(connection);
  return (exist);
}

void				Server::_timerPing(Client *clt, IConnection *connection)
{
  clt->pongValue(rand());
  this->_rsp.ping(*connection, clt->pongValue());
  clt->pongValue(clt->pongValue());
  clt->startWFPong();
}

void				Server::_timerPong(Client *clt, IConnection *connection)
{
  std::cout << "Goodbye, client n°" << connection->getId() << std::endl;
  this->_clients.erase(connection->getId());
  delete clt;
  this->_srv->eraseClient(connection->getId());
}

void				Server::_changeStatus(Client *clt, const eStatus &status) const
{
  std::vector<std::string>	friends;
  std::vector<std::string>::const_iterator	it;
  std::map<int, Client *>::const_iterator	it2;
  IConnection			*friendConnection;

  clt->identity()->setStatus(status);
  friends = clt->identity()->getContacts();
  it = friends.begin();
  while (it != friends.end())
    {
      it2 = this->_clients.begin();
      while (it2 != this->_clients.end() && ((*it2).second->identity() == NULL || (*it2).second->identity()->getLogin() != (*it)))
	++it2;
      if (it2 != this->_clients.end())
	{
	  friendConnection = this->_srv->getClient((*it2).first);
	  this->_rsp.status(*friendConnection, clt->identity()->getLogin(), this->_converter.convert(status));
	}
      ++it;
    }
}

void				Server::_disconnection(Client *clt)
{
  std::list<Channel *>		channels;
  std::list<Channel *>::const_iterator	it;
  UserInformation		*id;
  UserInformation		*ui;
  std::map<int, Client *>::const_iterator	it2;
  std::list<std::string>	peoples;
  std::list<std::string>::const_iterator	it3;
  bool				isPrivate;

  id = clt->identity();
  if (id != NULL)
    {
      channels = clt->getChannels();
      it = channels.begin();
      while (it != channels.end())
	{
	  isPrivate = this->_pManager.isPrivate(*it);
	  if ((*it)->removePeople(id->getLogin()))
	    {
	      peoples = (*it)->getPeoples();
	      it3 = peoples.begin();
	      while (it3 != peoples.end())
		{
		  ui = static_cast<UserInformation *>(this->_udb->select(LOGIN_PREFIX(*it3)));
		  if (ui != NULL)
		    {
		      it2 = this->_clients.begin();
		      while (it2 != this->_clients.end() && (*it2).second->identity() != ui)
			++it2;
		      if (it2 != this->_clients.end())
			{
			  if (isPrivate)
			    {
			      this->_rsp.leave(*(this->_srv->getClient((*it2).first)), id->getLogin(), id->getLogin());
			      (*it2).second->delChannel(*it);
			    }
			  else
			    this->_rsp.leave(*(this->_srv->getClient((*it2).first)), (*it)->getName(), id->getLogin());
			}
		    }
		  ++it3;
		}
	      clt->delChannel(*it);
	      if (isPrivate)
		this->_pManager.removePrivate(*it);
	      ++it;
	    }
	}
      this->_changeStatus(clt, DISCONNECTED);
    }
  clt->identity(NULL);
}

void				Server::handleAccept(IConnection &connection)
{
  Client			*clt;

  try
    {
      clt = this->_clients.at(connection.getId());
      this->_clients.erase(connection.getId());
      delete clt;
    }
  catch (std::out_of_range &err)
    {
      (void)err;
    }
  clt = new Client(this, connection.getId());
  this->_clients[connection.getId()] = clt;
  this->_timerPing(clt, &connection);
  std::cout << "Hello, client n°" << connection.getId() << std::endl;
}

void				Server::handleRead(const std::size_t &len, IConnection &connection)
{
  Client			*clt;
  bool				exist;

  try
    {
      clt = this->_clients.at(connection.getId());
    }
  catch (std::out_of_range &err)
    {
      (void)err;
      return ;
    }
  clt->cancelWFPing();
  clt->cancelWFPong();
  if (clt->state() == READ_HEADER)
    exist = this->_findHeaderLen(len, connection, clt);
  else
    exist = this->_parseCommand(len, connection, clt);
  if (exist)
    clt->startWFPing();
}

void				Server::handleDisconnection(IConnection &connection)
{
  Client			*clt;

  try
    {
      clt = this->_clients.at(connection.getId());
    }
  catch (std::out_of_range &err)
    {
      (void)err;
      return ;
    }
  this->_disconnection(clt);
  std::cout << "Goodbye, client n°" << connection.getId() << std::endl;
  try
    {
      Client			*tmp;

      tmp = this->_clients.at(connection.getId());
      this->_clients.erase(connection.getId());
      delete tmp;
    }
  catch (std::out_of_range &err)
    {
      (void)err;
    }
}

void				Server::handleWait(const int &key)
{
  int				id;
  eTimer			type;
  Client			*clt;
  IConnection			*connection;

  id = key >> 2;
  type = static_cast<eTimer>(key << (sizeof(int) - 2) >> 2);
  try
    {
      clt = this->_clients.at(id);
    }
  catch (std::out_of_range &err)
    {
      (void)err;
      return ;
    }
  connection = this->_srv->getClient(id);
  if (connection == NULL)
    return ;
  try
    {
      (this->*_timer.at(type))(clt, connection);
    }
  catch (std::out_of_range &err)
    {
      (void)err;
    }
}

void				Server::init(const std::uint16_t &port)
{
  this->_srv->initServer(port, this);
}

void				Server::run()
{
  this->_srv->loop();
}
