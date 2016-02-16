#include <QApplication>
#include <QNetwork.hh>
#include "login.h"
#include "client.hh"

int	main(int argc, char **argv)
{
  QApplication	app(argc, argv);

  try
  {
    Client    *cli;
    if (argc != 3)
      cli = new Client();
    else
      cli = new Client(argv[1], argv[2]);
    cli->startLogin();
    return app.exec();
  }
  catch (std::string const &str)
  {
    std::cout << str << std::endl;
  }
  return 0;
}
