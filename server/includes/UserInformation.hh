#ifndef USERINFORMATION_HH_
#define USERINFORMATION_HH_

#include <vector>
#include "Status.hh"

class UserInformation
{
public:
  UserInformation(const std::string &login, const std::string &password);
  ~UserInformation();
  bool					operator==(UserInformation &cmp) const;
  bool					operator!=(UserInformation &cmp) const;
  bool					addContact(const std::string &login);
  bool					delContact(const std::string &login);
  const std::vector<std::string>	&getContacts() const;
  const std::string			&getPassword() const;
  void					setPassword(const std::string &password_);
  const std::string			&getLogin() const;
  void					setLogin(const std::string &login_);
  const eStatus				&getStatus() const;
  void					setStatus(const eStatus &status_);
  bool					addPending(const std::string &login);
  bool					delPending(const std::string &login);
  const std::vector<std::string>	&getPendings() const;
  std::string				stringify() const;

private:
  std::string				login;
  std::string				password;
  std::vector<std::string>		contacts;
  std::vector<std::string>		pendings;
  eStatus				status;
};

#endif //USERINFORMATION_HH_
