#ifndef MESSENGER_HPP
# define MESSENGER_HPP

# include "Library.hpp"

class Messenger {

private:

    std::map<int, Message> messages;

public:
    Messenger();
    ~Messenger();
   void parseBuffer(int senderFd);

   void setMessages(std::map<int, Message> _messages);
   std::map<int, Message> getMessages();
   void insertMessage(int senderFd, Message mess);
   void deleteMessage(int senderFd);
   std::string getRawMessageByFd(int senderFd);

   std::string parsRecvStr(std::string str);


//    void setMess(Message _mess);
//    Message getMess();

};

#endif