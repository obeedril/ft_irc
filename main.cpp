#include "Library.hpp"

bool gFlagExit = 1;



void signalHandler(int signum) {
	(void)signum;
	std::cout << "\x1b[1;95m" << "\b\b> Bye-bye!\n" << "\x1b[0m"; 
	gFlagExit = 0; 
}

bool checkToOpen(void) {
	FILE* fp = fopen(PATH_TO_CONFIG, "r+");
	if (!fp) {
		std::cerr << "\x1b[1;95m" << "> Can't open config file!\n" << "\x1b[0m";
		return 1;
	}
	std::fclose(fp);
	return 0;
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

	Server irc_serv = Server(port, argv[2]);
	if (checkToOpen())
		return 1;
	try {
		irc_serv.parseConfig();
		Core core_irc = Core(port, &irc_serv);
		std::cout << "\x1b[1;95m" << "> IRC server run!\n" << "\x1b[0m";
		while (gFlagExit) {
		core_irc.run();
	}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	// Core core_irc = Core(port, &irc_serv);
	// std::cout << "\x1b[1;95m" << "> IRC server run!\n" << "\x1b[0m";
	// while (gFlagExit) {
	// 	core_irc.run();
	// }
	return (0);
}