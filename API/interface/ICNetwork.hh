//
// ICNetwork.hh for ICNetwork in /home/lopez_t/Babel/API/interface
//
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
//
// Started on  Thu Oct  8 15:54:43 2015 Thibaut Lopez
// Last update Tue Oct 13 17:52:17 2015 leo BOURREL
//

#ifndef	ICNetwork_HH
#define	ICNetwork_HH

#include <iostream>
#include <cstdint>

class	ICNetwork
{
public:
  virtual ~ICNetwork() {}

  virtual bool		initServeur(const std::string &host, const uint16_t &port) = 0;
  virtual void    Sread() = 0;
  virtual void    Swrite(int const cmd, const std::string &data) = 0;
};

#endif
