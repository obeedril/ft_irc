#include "Library.hpp"

bool gFlagExit = 1;


//https://github.com/barimehdi77/ft_irc    https://github.com/levensta/IRC-Server/blob/master/srcs/main.cpp


void signalHandler(int signum) {
	(void)signum;
	std::cout << "\x1b[1;95m" << "\b\b> Bye-bye!\n" << "\x1b[0m"; 
	gFlagExit = 0; 
}

int main(int argc, const char* argv[]) {
	// const char*	config;

	if (argc != 3) {
		std::cout << "\x1b[1;70m" << "> ERROR: Enter ./ircserv <port> <password>" << "\n" << "\x1b[0m";
		return (0);
	}

    int port = atoi(argv[1]);

	if (port < 1024 || port > 49151)
	{
		std::cout << "\x1b[1;70m" << "> ERROR: wrong user port" << "\n" << "\x1b[0m" << std::endl;
		return (0);
	}
	signal(SIGINT, signalHandler); 


	// std::vector<Server*> vectorServers;

	// for (std::vector<ServerPairs>::iterator it = parser->getServers().begin(); it!= parser->getServers().end(); ++it) {
		Server *irc_serv = new Server(port, argv[1]);
		FILE* fp = fopen(PATH_TO_CONFIG, "r+");
		if (!fp) {
			std::cerr << "\x1b[1;95m" << "> Can't open config file!\n" << "\x1b[0m";
			return 0;
		}
		// std::fclose(fp); //вернуть!!!
		try {
			irc_serv->parseConfig();
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}

		
	// 	vectorServers.push_back(ff_serv);
	// }
	// std::cout << "\x1b[1;95m" << "> WebServer Run!\n" << "\x1b[0m";
	// Core* main_core = new Core(vectorServers);
	while (gFlagExit) {
        std::cout << "\x1b[1;95m" << "> IRC server run!\n" << "\x1b[0m";
                    break ; // временно!
		// main_core->run();
	}
	// delete parser;
	// delete main_core;
	// for (int i = 0; i < count_server; i++) {
	// 	delete vectorServers[i];
	// }
	return (0);
}


/*
В конструктор сервера
Server::Server(int port, const std::string &password) :  port(port), password(password)
	// Read MOTD
	std::string		line;
	std::ifstream	motdFile("conf/IRCat.motd");
	if (motdFile.is_open())
	{
		while (getline(motdFile, line))
			motd.push_back(line);
		motdFile.close();
	}
	
	loadConfig();
	//Check config for set correct values ?
}

void Server::loadConfig() {
	
	static bool wasLoaded = false;

	JSON::Object *conf = NULL;
	try {
		conf = JSON::parseFile("conf/IRConf.json");
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	if (conf != NULL) {

		//Getting values from config
		name = conf->get("servername")->toStr();
		info = conf->get("info")->toStr();
		version = conf->get("version")->toStr();
		debuglvl = conf->get("debuglvl")->toStr();
		comments = conf->get("comments")->toStr();
		describe = conf->get("describe")->toStr();
		adminName = conf->get("adminName")->toStr();
		adminEmail = conf->get("adminEmail")->toStr();
		adminNickname = conf->get("adminNickname")->toStr();
		allowedIP = inet_addr(conf->get("allowedIP")->toStr().c_str());
		maxChannels = static_cast<unsigned long>(conf->get("maxChannels")->toNum());
		maxInactiveTimeout = static_cast<unsigned long>(conf->get("maxInactiveTimeout")->toNum());
		maxResponseTimeout = static_cast<unsigned long>(conf->get("maxResponseTimeout")->toNum());
		
		operators.clear();
		fillOperatorsList(operators, conf->get("operators")->toObj());

		delete conf;
		wasLoaded = true;
	}
	else if (wasLoaded != true) {

		//Set defaults
		name = "IRCat";
		info = "None";
		version = "None";
		debuglvl = "None";
		comments = "None";
		describe = "None";
		adminName = "None";
		adminEmail = "None";
		adminNickname = "None";
		allowedIP = 0UL;
		maxChannels = 10;
		maxInactiveTimeout = 120;
		maxResponseTimeout = 60;
	}

	//Only for debug
	std::cout << "CONFIG" << std::endl;
	std::cout << "servername: " << name << std::endl;
	std::cout << "info: " << info << std::endl;
	std::cout << "version: " << version << std::endl;
	std::cout << "debuglvl: " << debuglvl << std::endl;
	std::cout << "comments: " << comments << std::endl;
	std::cout << "describe: " << describe << std::endl;
	std::cout << "adminName: " << adminName << std::endl;
	std::cout << "adminEmail: " << adminEmail << std::endl;
	std::cout << "adminNickname: " << adminNickname << std::endl;
	std::cout << "maxChannels: " << maxChannels << std::endl;	
	std::cout << "maxInactiveTimeout: " << maxInactiveTimeout << std::endl;	
	std::cout << "maxResponseTimeout: " << maxResponseTimeout << std::endl;	
	
	struct in_addr paddr;
	paddr.s_addr = allowedIP;
	std::cout << "allowedIP(int): " << allowedIP << std::endl;
	std::cout << "allowedIP(str): " << inet_ntoa(paddr) << std::endl;

	std::map<std::string, std::string>::iterator beg = operators.begin();
	std::map<std::string, std::string>::iterator end = operators.end();
	std::map<std::string, std::string>::iterator it;

	for (it = beg; it != end; it++)	{
		std::cout << "Login: " << it->first << " " << "Hash: " << it->second << std::endl;
	}
}

void Server::fillOperatorsList(std::map<std::string, std::string> &operators, JSON::Object *confOperators) {
	std::map<std::string, JSON::AType *>::iterator beg = confOperators->begin();
	std::map<std::string, JSON::AType *>::iterator end = confOperators->end();
	std::map<std::string, JSON::AType *>::iterator it;

	for (it = beg; it != end; it++)
	{
		if (it->second != NULL)
		{
			operators.insert(std::pair<std::string, std::string>(it->first, it->second->toStr()));
		}
	}
}

*/