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
   bool checkRegistered(User* sender) ;

  void parsRecvStr(std::string str, User* sender, std::map<int, User>::iterator begin, std::map<int, User>::iterator end);

// USER cmd
    int		userCmd(const std::string &msg, User* sender, std::map<int, User>::iterator begin, std::map<int, User>::iterator end);
    int		replyError(User *user, int err, const std::string &str1, const std::string &str2);
    std::vector<std::string> stringSplit(const std::string &line, char delimiter);
    std::string tostring(std::vector<std::string> &v);

//    void setMess(Message _mess);
//    Message getMess();

};

#endif