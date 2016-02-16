#ifndef ILOGIN_H
#define ILOGIN_H

#include <unistd.h>

class Client;

namespace Ui {
class ILogin;
}

class ILogin
{
public:
  virtual ~ILogin(){};

  virtual void read_return(int const, int const) = 0;
};

#endif // ILOGIN_H
