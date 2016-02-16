//
// main.cpp for main in /home/lopez_t/Babel
// 
// Made by Thibaut Lopez
// Login   <lopez_t@epitech.net>
// 
// Started on  Tue Oct 20 11:31:02 2015 Thibaut Lopez
// Last update Sat Nov  7 12:12:06 2015 Thibaut Lopez
//

#include "Server.hh"
#include <signal.h>
#include <cstdlib>

int	main(void)
{
  Server	srv;

  srand((unsigned int)time(NULL));
  srv.init(6667);
  srv.run();
  return (0);
}

