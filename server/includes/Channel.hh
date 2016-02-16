#ifndef CHANNEL_HH_
#define CHANNEL_HH_

#include <string>
#include <list>

typedef std::list<std::string> stringlist;

class Channel
{
public:
  Channel(const std::string &);
  ~Channel();
  bool			addPeople(const std::string &);
  bool			banPeople(const std::string &);
  bool			removePeople(const std::string &);
  bool			unbanPeople(const std::string &);
  const stringlist	&getPeoples() const;
  const std::string	&getName() const;
  uint16_t		getProxyPort() const;
private:
  std::string		name;
  stringlist		people;
  stringlist		banlist;
};

#endif //CHANNEL_H_
