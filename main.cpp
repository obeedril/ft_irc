#include "Library.hpp"

bool gFlagExit = 1;


//https://github.com/barimehdi77/ft_irc    https://github.com/levensta/IRC-Server/blob/master/srcs/main.cpp


void signalHandler(int signum) {
	(void)signum;
	std::cout << "\x1b[1;95m" << "\b\b> Bye-bye!\n" << "\x1b[0m"; 
	gFlagExit = 0; 
}

int main(int argc, const char* argv[]) {

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

	Server irc_serv = Server(port, argv[1]);
	FILE* fp = fopen(PATH_TO_CONFIG, "r+");
	if (!fp) {
		std::cerr << "\x1b[1;95m" << "> Can't open config file!\n" << "\x1b[0m";
		return 0;
	}
	std::fclose(fp);
	try {
		irc_serv.parseConfig();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}


	Core core_irc = Core(port);
	std::cout << "\x1b[1;95m" << "> IRC server run!\n" << "\x1b[0m";

	
	while (gFlagExit) {
		core_irc.run();
	}
	// delete irc_serv;
	return (0);
}