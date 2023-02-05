#ifndef MESSENGER_HPP
# define MESSENGER_HPP

# include "Library.hpp"

class Messenger {

private:
		std::map<int, Message>  messages;
		std::map<int, Bot>		map_robots;
		std::map<int, User>		map_users;
		std::string				serverName;

		ChannelsStorage			channels;
		

public:

	Messenger(std::string server);
	~Messenger();
	
	void	parserPrivmsg(Message &mess, User &user);
	void	setMessages(std::map<int, Message> _messages);
	std::map<int, Message> getMessages();
	void	insertMessage(int senderFd, Message mess);
	void	deleteMessage(int senderFd);
	std::string getRawMessageByFd(int senderFd);
	std::string getReadyMessByFd(int senderFd);
	std::string getServerName();
	std::map<int, User> getMapUsers();

	void	parsRecvStr(std::string str, int userFd);
	void	dequeMaker(User *user, int flag);

	int		userCmd(const std::string &msg, User* sender);
	std::vector<std::string> stringSplitForUserCmd(const std::string &line, char delimiter);
	std::string tostring2(std::vector<std::string> &v);
	std::string initBot(int user_fd, std::string msg);
	void	deleteBot(int senderFd);

	void	sendMotd(User* sender);

    int		passCmd(const std::string &msg, User* sender);
	int		quitCmd(const std::string &msg, User* sender);
	int		nickCmd(const std::string &msg, User* sender);
	int		whoAmICmd(User* sender);
	int		stringOutputMaker(User *user, int err, const std::string &description, const std::string &command);
	void	checkAdmin(User* sender);
	std::vector<int> getDeq(int senderFd);

	void addUser(int user_fd, User new_user);
	int  getUserFd(int Fd);
	User *getUser(int fd);
	int	deleteUser(int fd);
	int	getUserFdByLogin(std::string login);

    void setReadyMessInMessageByFd(std::string str, int senderFd);
	void setRawMessInMessageByFd(std::string str, int senderFd); 
    std::string getCmdInMessageByFd(int senderFd);

	std::string getSystemMsg(int senderFd);
	std::string findNameKick(Message mess);

	ChannelsStorage	*getChannels();

};

#endif