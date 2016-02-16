#ifndef conversationManager_hh
#define conversationManager_hh

#include "conversation.hh"
#include <cstdlib>
#include <vector>

class conversationManager
{
private:
  std::vector<conversation*>  conversations;
  int                         cid;

public:
  conversationManager();
  ~conversationManager();

  void          addMsgToConv(std::string const &, std::string const &, std::string const &);
  void          createConversation(const std::string &name, const bool priv = true);
  void          deleteConversation(const int id);
  void          deleteConversation(const std::string &name);

  void          printConversations() const;
  void          BanContact(const std::string &name, const std::string &contact);
  void          BanContact(const int id, const std::string &contact);

  conversation  *getConversation(const int id) const;
  conversation  *getConversation(const std::string &name) const;
  std::vector<conversation*>  getConversations() const;
  std::list<s_msg>     getMessages(const std::string &name) const;

};

#endif
