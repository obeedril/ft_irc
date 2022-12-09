#pragma once

#include <string>
#include <map>

class Server
{
	private:
		std::string								_serverName;
		std::string 							_configTokens[COUNT_TOKENS];
		int										_port;
		// std::vector<User *>						_connectedUsers;
		// std::vector<struct pollfd>				_userFDs;
		int										_sockfd;
		sockaddr_in								_sockaddr;
		std::string								_password; // это пароль админа?
		std::string								_admin;
		std::map<std::string, std::string>		_operators;
		// std::map<std::string, Channel *>		_channels;
		// std::map<std::string, Method>			_commands;
		// History									_nicknamesHistory;

		// Server();
		Server(const Server& copy);
		Server	&operator=(const Server& other);

		// Commands

		// int										passCmd(const Message &msg, User &user);
		// int										nickCmd(const Message &msg, User &user);
		// int										userCmd(const Message &msg, User &user);
		// int										operCmd(const Message &msg, User &user);
		// int										quitCmd(const Message &msg, User &user);
		// int										privmsgCmd(const Message &msg, User &user);
		// int										awayCmd(const Message &msg, User &user);
		// int										noticeCmd(const Message &msg, User &user);
		// int										whoCmd(const Message &msg, User &user);
		// int										whoisCmd(const Message &msg, User &user);
		// int										whowasCmd(const Message &msg, User &user);
		// int										modeCmd(const Message &msg, User &user);
		// int										topicCmd(const Message &msg, User &user);
		// int										joinCmd(const Message &msg, User &user);
		// int										inviteCmd(const Message &msg, User &user);
		// int										kickCmd(const Message &msg, User &user);
		// int										partCmd(const Message &msg, User &user);
		// int										namesCmd(const Message &msg, User &user);
		// int										listCmd(const Message &msg, User &user);
		// int										wallopsCmd(const Message &msg, User &user);
		// int										pingCmd(const Message &msg, User &user);
		// int										pongCmd(const Message &msg, User &user);
		// int										isonCmd(const Message &msg, User &user);
		// int										userhostCmd(const Message &msg, User &user);
		// int										versionCmd(const Message &msg, User &user);
		// int										infoCmd(const Message &msg, User &user);
		// int										adminCmd(const Message &msg, User &user);
		// int										timeCmd(const Message &msg, User &user);
		// int										rehashCmd(const Message &msg, User &user);
		// int										killCmd(const Message &msg, User &user);
		// int										restartCmd(const Message &msg, User &user);

		// Helpful things for commands

		// int										handleChanFlags(const Message &msg, User &user);
		// int										handleUserFlags(const Message &msg, User &user);
		// int										checkConnection(User &user);
		// void									notifyUsers(User &user, const std::string &notification);
std::vector<std::string> splitLines(std::string path);
// std::vector<std::string> splitLines(char * path);
std::vector<std::string> checkBraces(std::string buf);
int onlySpaces(std::string buf);
int emptyOrComments(std::string buf);
void deleteSpaces(std::string *str);
void bracesCounter(std::vector<std::string> res);
// void checkTokens(std::vector<std::string> res);


	public:
		Server(int port, const std::string &password);
		~Server();
		void parseConfig();

		// Getters

		// const int								&getSockfd() const;

		// Server setup

		// void									createSocket();
		// void									bindSocket();
		// void									listenSocket();
		// void									grabConnection();
		// void									processMessages();
		// int										hadleMessages(User &user);
		// void									deleteBrokenConnections();
		// void									deleteEmptyChannels();
		// void									checkConnectionWithUsers();
		// void 									fillOperatorsList(std::map<std::string, std::string> &operators, JSON::Object *confOperators);
		// void									loadConfig( void );

		// Other methods

		// bool									containsNickname(const std::string &nickname) const;
		// bool									containsChannel(const std::string &name) const;
		// void									sendMOTD(const User &user) const;
		// int										connectToChannel(const User &user, const std::string &name, const std::string &key);
		// void									inviteToChannel(const User &user, const std::string &nickname, const std::string &chanName);
		// User									*getUserByName(const std::string &name);
};