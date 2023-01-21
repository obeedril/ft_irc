#ifndef MESSENGER_HPP
# define MESSENGER_HPP

# include "Library.hpp"

class Messenger {

private:
		std::map<int, Message>  messages;
		std::map<int, Bot>      map_robots;
		std::map<int, User>		map_users;

		ChannelsStorage			channels;
		

public:

		Messenger();
		~Messenger();
	

	void parserPrivmsg(Message &mess, User &user);
	void setMessages(std::map<int, Message> _messages);
	std::map<int, Message> getMessages();
	void insertMessage(int senderFd, Message mess);
	void deleteMessage(int senderFd);
	std::string getRawMessageByFd(int senderFd);
	std::string getReadyMessByFd(int senderFd);

	void parsRecvStr(std::string str, int userFd);

	void dequeMaker(User *user, int flag); //flag 1 = for 1 user, flag 2 = to all except user in argument, flag 3 = to all, flag 4 = to channel except user in argument (NOT WORKS!!!!), flag 5 = to channel and user in argument (NOT WORKS!!!!)

	// USER cmd
	int		userCmd(const std::string &msg, User* sender);
	int		replyError(User *user, int err, const std::string &str1, const std::string &str2);
	std::vector<std::string> stringSplitForUserCmd(const std::string &line, char delimiter);
	// std::string tostring(std::vector<std::string> &v);
	std::string tostring2(std::vector<std::string> &v);
	std::string initBot(int user_fd, std::string msg);
	void deleteBot(int senderFd);

	void sendMotd(User* sender);

	// PASS cmd
    int	passCmd(const std::string &msg, User* sender);
	// NICK cmd
	int	nickCmd(const std::string &msg, User* sender);
	int	whoAmICmd(User* sender);
	int stringOutputMaker(User *user, int err, const std::string &description, const std::string &command);
	// std::vector<std::string> stringSplit2(const std::string &line, std::string delimiter);

	//    void setMess(Message _mess);
	//    Message getMess();
	std::vector<int> getDeq(int senderFd);
	void setDeq(std::vector<int>);

	//map_users 
	void addUser(int user_fd, User new_user);
	int  getUserFd(int Fd);
	User *getUser(int fd);
	int	deleteUser(int fd);
	int	getUserFdByLogin(std::string login);

    void setReadyMessInMessageByFd(std::string str, int senderFd);
	void setRawMessInMessageByFd(std::string str, int senderFd); // удалить! это пока нет печати через readymess
    std::string getCmdInMessageByFd(int senderFd);

	std::string getSystemMsg(int senderFd);
	std::string findNameKick(Message mess);

	ChannelsStorage	getChannels();
	bool  checkLagcheck(std::string str);

};

#endif