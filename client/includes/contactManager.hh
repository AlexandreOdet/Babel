#ifndef contactManager_hh
#define contactManager_hh

#include "contact.hh"
#include <vector>

class contactManager
{
private:
  std::vector<contact*>  contacts;
  int                    cid;

public:
  contactManager();
  ~contactManager();

  void     createcontact(const std::string &name);
  void     deletecontact(const int id);
  void     deletecontact(const std::string &name);

  void     printcontacts() const;

  contact  *getcontact(const int id) const;
  contact  *getcontact(const std::string &name) const;
  std::vector<contact*> getcontacts() const;
};

#endif
