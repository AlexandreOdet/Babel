#include "client.hh"

Client::Client(const char *ip, const char *port)
{
  this->net = new QNetwork(this);
  if (!isDigits(port))
    throw std::string("Invalid Port");
  this->net->initServeur(ip, std::stoi(port));
  this->exec = new execCmd(this);
  this->connected = false;
  this->mainWin = new Mainwidget(this);
  this->loginWin = new login(this);
}

Client::~Client()
{
  if (this->mainWin)
  {
    writeToNet(21, "");
    delete this->mainWin;
  }
  if (this->net)
    delete this->net;
  if (this->loginWin)
    delete this->loginWin;
}

bool    Client::isDigits(const std::string &str) const
{
  return str.find_first_not_of("0123456789") == std::string::npos;
}

void    Client::startLogin()
{
  this->connected = false;
  this->loginWin->show();
}

void    Client::startMain(std::string const &user)
{
  std::string winName("Babel - " + user);

  this->user = new contact(user, 0);
  this->connected = true;
  this->mainWin->setWindowTitle(winName.c_str());
  this->mainWin->show();
}

void    Client::sendToCmd(int const cmd, std::string const &args) const
{
  exec->findMessage(cmd, args);
}

void    Client::writeToNet(int const cmd, std::string const &args) const
{
  if (cmd == 4)
    this->exec->addWaitAdd(args);
  else if (cmd == 5)
    this->exec->addWaitJoin(args);
  else if (cmd != 19)
    this->exec->addReturn(cmd);
  this->net->Swrite(cmd, args);
}

std::string         Client::getName() const
{
  return user->getName();
}

bool                Client::getConnect() const
{
  return connected;
}

login               *Client::getLoginWin() const
{
  return loginWin;
}

Mainwidget          *Client::getMainWin() const
{
 return mainWin;
}

conversationManager Client::getConvManager() const
{
  return conversations;
}

contactManager      Client::getContacts() const
{
  return contacts;
}

void                Client::setConvManager(conversationManager &new_conv)
{
  this->conversations = new_conv;
}

void                Client::setContacts(contactManager &new_contact)
{
  this->contacts = new_contact;
}

std::vector<unsigned int> Client::charToInt(const unsigned char *str) const
{
  std::vector<unsigned int> vec;

  for (unsigned int i = 0; i < BLOCK_SIZE; i++)
  {
    if (i % 4 == 0)
    {
      unsigned int tmp = 0;

      tmp = str[i];
      tmp += (str[i + 1] > 0) ? (str[i + 1] * 255) + 1 : 0;
      tmp += (str[i + 2] > 0) ? (str[i + 2] * 255 * 255) + 1 : 0;
      tmp += (str[i + 3] > 0) ? (str[i + 3] * 255 * 255 * 255) + 1 : 0;
      vec.push_back(tmp);
    }
  }
  return vec;
}
