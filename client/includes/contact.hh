#ifndef contact_hh
#define contact_hh

#include "contact.hh"
#include <iostream>
#include <vector>

enum STATUS {
  INACTIVE = 0,
  ACTIVE = 1
};

class contact
{
private:
  STATUS        status;
  std::string   name;
  int           id;

public:
  contact(const std::string &_name, const int _id);
  ~contact();

  std::string               getName() const;
  int                       getId() const;

  void                      setName(std::string const &);
  void                      setId(int const);
};

#endif
