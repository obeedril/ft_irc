#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <map>
#include "Library.hpp"

class Server
{
	private:
		std::string								_serverName;
		std::vector<std::string>				_configTokens;
		int										_port;
		int										_sockfd;
		sockaddr_in								_sockaddr;
		std::string								_password;
		std::string								_admin;
		std::map<std::string, std::string>		_operators;

		std::vector<std::string> splitLines(std::string path);
		std::vector<std::string> checkBraces(std::string buf);
		int onlySpaces(std::string buf);
		int emptyOrComments(std::string buf);
		void deleteSpaces(std::string *str);
		void newDeleteSpaces(std::string *str);


	public:
		Server(int port, const std::string &password);
		~Server() {};
		void parseConfig();
};

#endif