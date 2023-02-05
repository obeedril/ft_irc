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
		std::string								_argvPass;
		std::string								_configPass;
		std::string								_admin;
		std::map<std::string, std::string>		_operators;
		std::vector<std::string>				_motd;

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
		std::string	getServName();
		std::vector<std::string>	getMotd();

		std::string	getArgPass();
		std::string	getConfigPass();
		std::string	getAdminLogin();
		const std::map<std::string, std::string>&	getOperatorsMap();
};

#endif