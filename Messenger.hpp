#ifndef MESSENGER_HPP
# define MESSENGER_HPP

# include "Library.hpp"

class Messenger {

private:
    std::map<int, Message>  messages;
    std::map<int, Bot>      map_robots;

public:
    Messenger();
    ~Messenger();
   void parseBuffer(int senderFd);

   void setMessages(std::map<int, Message> _messages);
   std::map<int, Message> getMessages();
   void insertMessage(int senderFd, Message mess);
   void deleteMessage(int senderFd);
   std::string getRawMessageByFd(int senderFd);
   bool checkRegistered(User* sender) ;

  void parsRecvStr(std::string str, User* sender);

  std::string initBot(User *my_client, std::string msg);
  void deleteBot(int senderFd);

//    void setMess(Message _mess);
//    Message getMess();

};

#endif