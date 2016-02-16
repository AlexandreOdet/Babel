//
// QNetwork.cpp for QNetwork in /home/lopez_t/Babel/API/Network
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Fri Oct 09 14:34:39 2015 Thibaut Lopez
// Last update Tue Oct 13 18:26:40 2015 leo BOURREL
//

#include "QNetwork.hh"
#include "client.hh"

QNetwork::QNetwork(Client *c, QWidget *parent) : QDialog(parent)
{
  tcpSocket = new QTcpSocket;
  client = c;
  readHeader = true;
}

QNetwork::~QNetwork()
{

}

bool		QNetwork::initServeur(const std::string &host, const uint16_t &port)
{
  QString	tmp(host.c_str());

  tcpSocket->connectToHost(tmp, port);

  connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
  connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(Sread()));
  connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

  return true;
}

void		QNetwork::Swrite(int const cmd, const std::string &data)
{
  int size = data.size();

  tcpSocket->write((char*)&cmd, sizeof(int));
  tcpSocket->write((char*)&size, sizeof(int));
  tcpSocket->write(data.c_str(), size);
}

void		QNetwork::Sread()
{
  if (readHeader == true)
  {
    char header[BLOCK_SIZE];
    if (tcpSocket->bytesAvailable() < BLOCK_SIZE)
      return;
    tcpSocket->read(header, BLOCK_SIZE);
    intHeader = client->charToInt((unsigned char*)header);
  }

  if (tcpSocket->bytesAvailable() >= intHeader[1])
  {
    char args[intHeader[1]];
    tcpSocket->read(args, intHeader[1]);

    std::string toSend;
    toSend.append(args, intHeader[1]);

    this->client->sendToCmd(intHeader[0], toSend);
    readHeader = true;
    intHeader.clear();
    if (tcpSocket->bytesAvailable() > 0)
      Sread();
  }
  else
    readHeader = false;
}

void		QNetwork::displayError(QAbstractSocket::SocketError err)
{
  if (err == QAbstractSocket::HostNotFoundError)
  {
    std::cerr << "The host was not found. Please check the";
    std::cerr << " host name and port settings." << std::endl;
  }
  else if (err == QAbstractSocket::ConnectionRefusedError)
  {
    std::cerr << "The connection was refused by the peer." << std::endl;
    std::cerr << "Please check that the host name and port settings are correct." << std::endl;
  }
  else
    std::cerr << "The following error occurred " << (int)err << std::endl;
  QTimer::singleShot(250, qApp, SLOT(quit()));
}

void		QNetwork::connected()
{
}
