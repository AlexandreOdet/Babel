//
// QNetwork.hh for QNetwork in /home/lopez_t/Babel/API/Network
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Fri Oct 09 14:34:39 2015 Thibaut Lopez
// Last update Tue Oct 13 18:15:35 2015 leo BOURREL
//

#ifndef	QNETWORK_HH
#define	QNETWORK_HH

#define BLOCK_SIZE 8

#include "ICNetwork.hh"
#include <signal.h>
#include <exception>
#include <QtWidgets>
#include <QtNetwork>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>

class Client;
class QNetworkSession;
class QTcpSocket;

class QNetworkException : public std::exception
{
public:
  QNetworkException(int const _err=0) throw() : err(_err) {}
  ~QNetworkException() throw() {}

   int  which() const throw()
   {
       return err;
   }

private:
  int err;
};

class	QNetwork : public QDialog, public ICNetwork
{
  Q_OBJECT

private:
  QTcpSocket	*tcpSocket;
  quint16      blockSize;
  Client      *client;
  bool        readHeader;
  std::vector<unsigned int> intHeader;
  bool        _connect;

public:
  QNetwork(Client *,  QWidget *parent = 0);
  virtual ~QNetwork();

  bool		initServeur(const std::string &host, const uint16_t &port);

public slots :
  void		Sread();
  void		Swrite(int const cmd, const std::string &data);

private slots:
  void		connected();
  void		displayError(QAbstractSocket::SocketError err);
};

#endif
