#ifndef IMAINWIDGET_H
#define IMAINWIDGET_H

#include <iostream>
#include <sstream>

class Client;

namespace Ui {
    class IMainWidget;
}

class IMainWidget
{
public:
  virtual ~IMainWidget(){};

  virtual void writeChannel(std::string const &name, std::string const &text) = 0;
  virtual void addContact(std::string const &, bool const) = 0;
  virtual void addChannel(std::string const &) = 0;
  virtual void deleteContact(std::string const &) = 0;
  virtual void deleteChannel(std::string const &) = 0;
  virtual void changeColor(std::string const &, bool const) = 0;
  virtual void updateConv(std::string const &) = 0;

  virtual void openPopUp(std::string const &) = 0;
  virtual void failurePopUp(std::string const &) = 0;
  virtual bool questionPopUp(std::string const &) = 0;
};

#endif // IMAINWIDGET_H
