#include "execCmd.hh"
#include "client.hh"

execCmd::execCmd(Client *c) : client(c)
{
  this->cmdMap[3] = &execCmd::update;
  this->cmdMap[4] = &execCmd::add;
  this->cmdMap[5] = &execCmd::del;
  this->cmdMap[6] = &execCmd::status;
  this->cmdMap[7] = &execCmd::join;
  this->cmdMap[8] = &execCmd::leave;
  this->cmdMap[9] = &execCmd::call;
  this->cmdMap[10] = &execCmd::cancel;
  this->cmdMap[13] = &execCmd::send;
  this->cmdMap[14] = &execCmd::ack;
  this->cmdMap[17] = &execCmd::bye;
  this->cmdMap[18] = &execCmd::ping;

  this->retMap[1] = &execCmd::retLogin;
  this->retMap[2] = &execCmd::retLogin;
  this->retMap[5] = &execCmd::doNothing;
  this->retMap[7] = &execCmd::addConversation;
  this->retMap[8] = &execCmd::doNothing;
  this->retMap[13] = &execCmd::addToConv;
  this->retMap[100] = &execCmd::doNothing;
  this->retMap[200] = &execCmd::doNothing;
  this->retMap[201] = &execCmd::addContact;
  this->retMap[202] = &execCmd::addConversation;
  this->retMap[203] = &execCmd::doNothing;
  this->retMap[302] = &execCmd::printReturn;
  this->retMap[303] = &execCmd::printReturn;
  this->retMap[304] = &execCmd::printReturn;
  this->retMap[321] = &execCmd::printReturn;
  this->retMap[341] = &execCmd::printReturn;
  this->retMap[342] = &execCmd::printReturn;
  this->retMap[371] = &execCmd::printReturn;
  this->retMap[372] = &execCmd::printReturn;
  this->retMap[373] = &execCmd::printReturn;

  this->errCode[303] = " : Does not exist";
  this->errCode[304] = " : Not Handshaken";
  this->errCode[321] = " : exist already ";
  this->errCode[341] = " : Already a Friend";
  this->errCode[342] = " : Already Pending";
  this->errCode[371] = " : Already in this channel";
  this->errCode[372] = " : User disconnected";
  this->errCode[373] = " : Already requested";
}

execCmd::~execCmd()
{
}

void    execCmd::findMessage(int const cmd, std::string const &args)
{
  try
  {
    if (cmd >= 3 && cmd <= 18)
    {
      std::map<int, _cmd>::iterator it = cmdMap.find(cmd);
      if (it != cmdMap.end())
        (this->*cmdMap[cmd])(args);
      else
        std::cerr << "Unknown command" << std::endl;
    }
    else if (waitingReturn.size() > 0 && cmd != 100)
    {
      std::map<int, ret>::iterator it = retMap.find(waitingReturn.front());
      if (it != retMap.end())
        (this->*retMap[waitingReturn.front()])(cmd, args);
      else
        std::cerr << "Unknown command" << std::endl;
      if (cmd != 180)
        this->waitingReturn.pop_front();
    }
    else
    {
      std::map<int, ret>::iterator it = retMap.find(cmd);
      if (it != retMap.end())
        (this->*retMap[cmd])(cmd, args);
      else
        std::cerr << "Unknown command" << std::endl;
    }
  }
  catch (std::out_of_range err)
  {
    std::cout << err.what() << std::endl;
  }
}

void    execCmd::addReturn(int const cmd)
{
  this->waitingReturn.push_back(cmd);
}

void    execCmd::addWaitAdd(std::string const &name)
{
  this->waitingAdd.push_back(name);
}

void    execCmd::addWaitJoin(std::string const &name)
{
  this->waitingJoin.push_back(name);
}

void    execCmd::update(std::string const &args)
{
  std::cout << "Update : " << args << std::endl;
}

void    execCmd::add(std::string const &args)
{
  if (client->getMainWin()->questionPopUp("Add " + args + " to your friends ?") == true)
    client->writeToNet(4, args);
  else
  {
    client->writeToNet(5, args);
    waitingAdd.pop_front();
  }
}

void    execCmd::del(std::string const &args)
{
  client->getMainWin()->deleteContact(args);
}

void    execCmd::status(std::string const &args)
{
  std::string login = args.substr(0, args.find_first_of(' '));

  std::string state = args.substr(login.size() + 1, args.size() - login.size() - 1);
  if (state.compare("connected") == 0)
    this->client->getMainWin()->changeColor(login, true);
  else
    this->client->getMainWin()->changeColor(login, false);
}

void    execCmd::join(std::string const &args)
{
  if (client->getMainWin()->questionPopUp("Start a conversation with " + args + " ?") == true)
    client->writeToNet(7, args);
}

void    execCmd::leave(std::string const &args)
{
  std::istringstream iss(args);
  std::string login;
  iss >> login;

  conversationManager v = client->getConvManager();
  v.deleteConversation(args.substr(0, args.find_first_of(' ')));
  this->client->setConvManager(v);
  this->client->getMainWin()->deleteChannel(login);
}

void    execCmd::call(std::string const &args)
{
  std::cout << "Call : " << args << std::endl;
}

void    execCmd::cancel(std::string const &args)
{
  std::cout << "Cancel : " << args << std::endl;
}

void    execCmd::send(std::string const &args)
{
  std::string msg(args);
  std::string conv;
  std::string user;

  conv = msg.substr(0, msg.find_first_of(' '));
  msg.erase(0, conv.size() + 1);
  user = msg.substr(0, msg.find_first_of(' '));
  msg.erase(0, user.size() + 1);

  /* Ajoute le message a la conversation */
  conversationManager  tmp = client->getConvManager();
  tmp.addMsgToConv(msg, user, conv);
  this->client->setConvManager(tmp);

  /* Update l'interface */
  this->client->getMainWin()->updateConv(conv);
}

void    execCmd::ack(std::string const &args)
{
  std::cout << "Ack : " << args << std::endl;
}

void    execCmd::bye(std::string const &args)
{
  std::cout << "BYE : " << args << std::endl;
}

void    execCmd::ping(std::string const &args)
{
  this->client->writeToNet(19, args);
}

void    execCmd::retLogin(int const ret, std::string const & args)
{
  if (waitingReturn.front() == 1 || waitingReturn.front() == 2)
    this->client->getLoginWin()->read_return(waitingReturn.front(), ret);
  (void)args;
}

void    execCmd::addContact(int const ret, std::string const &args)
{
  std::string login = args.substr(0, args.find_first_of(' '));

  if (args.back() == 'P')
    add(login);
  else
  {
    contactManager tmp = client->getContacts();

    if (tmp.getcontacts().size() == 0 || tmp.getcontact(login) != NULL)
    {
      std::string state = args.substr(login.size() + 1, args.size() - login.size() - 3);
      if (state.compare("connected") == 0)
        this->client->getMainWin()->addContact(login, true);
      else
        this->client->getMainWin()->addContact(login, false);
    }
  }
  (void)ret;
}

void    execCmd::addConversation(int const ret, std::string const &args)
{
  /*
    Gerer channel private / public
  */

  std::string         conv(args);
  conv.erase(0, 2);
  std::istringstream  iss(conv);
  iss >> conv;

  conversationManager tmp = client->getConvManager();
  if (tmp.getConversations().size() == 0 || tmp.getConversation(conv) != NULL)
    this->client->getMainWin()->addChannel(conv);
  (void)ret;
}

void    execCmd::addToConv(int const ret, std::string const &args)
{
  (void)args;
  (void)ret;
}

void    execCmd::doNothing(int const ret, std::string const &args)
{
  (void)ret;
  (void)args;
}

void    execCmd::printReturn(int const ret, std::string const &args)
{
  std::string tmp;

  if ((ret >= 340 && ret <= 350) || ret == 303 || 304 || ret == 321)
    waitingAdd.pop_front();
  else if (ret > 370 && ret < 374)
    waitingJoin.pop_front();
  else
    std::cout << "code : " << ret << std::endl;

  tmp = this->errCode[ret];

  if (client->getConnect() == true)
    client->getMainWin()->openPopUp(tmp);
  else
    std::cout << tmp << std::endl;
  (void)args;
}
