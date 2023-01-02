#ifndef MESSENGER_HPP
# define MESSENGER_HPP

# include "Library.hpp"

class Messenger {

private:

    // Message mess;
    std::map<int, Message> messages;

public:
    Messenger();
    ~Messenger();
   void parseBuffer(Message &message, User &user);

   void setMessages(std::map<int, Message> _messages);
   std::map<int, Message> getMessages();
   void insertMessage(User &user, Message &mess);

//    void setMess(Message _mess);
//    Message getMess();



};

#endif