#ifndef MESSENGER_HPP
# define MESSENGER_HPP

# include "Library.hpp"

class Messenger {

private:
		std::map<int, Message>  messages;
		std::map<int, Bot>      map_robots;
		std::map<int, User>			map_users;

public:

		Messenger();
		~Messenger();
	

   void parserPrivmsg(Message mess);
	 void setMessages(std::map<int, Message> _messages);
	 std::map<int, Message> getMessages();
	 void insertMessage(int senderFd, Message mess);
	 void deleteMessage(int senderFd);
	 std::string getRawMessageByFd(int senderFd);
	 bool checkRegistered(int userFd);
   std::string getReadyMessByFd(int senderFd);

	void parsRecvStr(std::string str, int userFd);


		// USER cmd
		int		userCmd(const std::string &msg, User* sender, std::map<int, User>::iterator begin, std::map<int, User>::iterator end);
		int		replyError(User *user, int err, const std::string &str1, const std::string &str2);
		std::vector<std::string> stringSplit(const std::string &line, char delimiter);
		std::string tostring(std::vector<std::string> &v);
		std::string initBot(int user_fd, std::string msg);
		void deleteBot(int senderFd);
    void	printWelcome(User* sender, std::map<int, Message>::iterator	it1, std::string name, int flag);
    void	sendMotd(User* sender);

// PASS cmd
    int	passCmd(const std::string &msg, User* sender);

		//    void setMess(Message _mess);
		//    Message getMess();
		std::vector<int> getDeq(int senderFd);
		void setDeq(std::vector<int>);

		//map_users 
		void addUser(int user_fd, User new_user);
		int  getUserFd(int Fd);


};

#endif