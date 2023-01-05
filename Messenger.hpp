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
   void setReadyMessInMessageByFd(std::string str, int senderFd);
   std::map<int, Message> getMessages();
   void insertMessage(int senderFd, Message mess);
   void deleteMessage(int senderFd);
   std::string getRawMessageByFd(int senderFd);
   bool checkRegistered(User* sender) ;
   std::string getCmdInMessageByFd(int senderFd);
   std::string getReadyMessInMessageByFd(int senderFd);


  void parsRecvStr(std::string str, User* sender, std::map<int, User>::iterator begin, std::map<int, User>::iterator end);


// USER cmd
    int		userCmd(const std::string &msg, User* sender, std::map<int, User>::iterator begin, std::map<int, User>::iterator end);
    int		replyError(User *user, int err, const std::string &str1, const std::string &str2);
    // int		replyAnswer(User *user, int code, const std::string &str);
    std::vector<std::string> stringSplit(const std::string &line, char delimiter);
    std::string tostring(std::vector<std::string> &v);
    std::string initBot(User *my_client, std::string msg);
    void deleteBot(int senderFd);
    void	printWelcome(User* sender, std::map<int, Message>::iterator	it1, std::string name, int flag);
    void	sendMotd(User* sender);

// PASS cmd
    int	passCmd(const std::string &msg, User* sender);

//    void setMess(Message _mess);
//    Message getMess();

};

#endif