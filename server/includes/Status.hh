//
// Status.hh for Status in /home/lopez_t/Babel/server
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Fri Oct 30 16:10:16 2015 Thibaut Lopez
// Last update Fri Oct 30 18:38:02 2015 Thibaut Lopez
//

#ifndef	STATUS_HH
#define	STATUS_HH

#include <iostream>
#include <string>
#include <map>

enum			eStatus
  {
    CONNECTED,
    DISCONNECTED,
    AWAY,
    UNKNOWN
  };

class					Status
{
private:
  std::map<eStatus, std::string>	_convert;

public:
  Status();
  virtual ~Status();

  const eStatus				&convert(const std::string &status) const;
  const std::string			&convert(const eStatus &status) const;
};

#endif
