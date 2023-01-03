#include "Library.hpp"

Bot::Bot(User *my_client) {
	gameNum = 50;
	UserFd = UserFd;
	bot_com = NO_COMM;
	this->my_client = my_client;
	startGame = 0;
}

Bot::~Bot() {

}

int Bot::callBot(std::string msg) {
	bot_com = NO_COMM;
	char		tmp[4048];
	int			index = 0;
	std::string substr[5] = {"BOT", "PLAY", "WEATHER", "BYE_BOT"};

	for(int i = 0; i < 5; i++) {
		std::cout << substr[i] << std::endl;
		index = msg.find(substr[i], 0);
		if (index == 0) {
			bot_com = static_cast<t_bot_command>(i);
			break;
		}
	}
	if (startGame > 0) {
		bot_com = PLAY;
	}
	std::cout << "bot_com: " << bot_com << std::endl;
	std::cout << "msg: " << msg << std::endl;
	switch (bot_com)
	{
		case HELLO_BOT: 
			sprintf(tmp, "%s %s\n", "BOT", ": Hi! I'm Kitty the robot! Do you want to PLAY numbers or find out the WEATHER?\r\n");
			my_client->setMessage(static_cast<std::string>(tmp));
			my_client->setBotDialog(YES);
			break;
		case PLAY:
			playGame(msg);
			break;
		case WEATHER:
			getWeather();
			break;
		case BYE_BOT:
			exitBot();
			break;
		default:
			sprintf(tmp, "%s %s\n", "BOT",
			": Its Robot Kitty! I still here =) Pick one of them command PLAY, WEATHER or BYE_BOT.\r\n");
			my_client->setMessage(static_cast<std::string>(tmp));
			break;
	}
	return 0;
}

int Bot::getWeather() {
	std::ofstream file("weather.html", std::ofstream::out | std::ofstream::trunc);
	pid_t	pid;
	char	**cmd;
	std::string curl("curl");
	std::string cmd_1("-X");
	std::string cmd_2("GET");
	std::string site("https://p.ya.ru/?via=f");
	std::string redir(">");
	std::string html("weather.html");

	cmd = new char*[6];
	for(int i = 0; i < 6; i++) {
		cmd[i] = NULL;
	}
	cmd[0] = strdup(curl.c_str());
	cmd[1] = strdup(cmd_1.c_str());
	cmd[2] = strdup(cmd_2.c_str());
	cmd[3] = strdup(site.c_str());
	cmd[4] = strdup(redir.c_str());
	cmd[5] = strdup(html.c_str());
	pid = fork();
	if (pid != 0) {
		waitpid(pid, NULL, 0);
	}
	else if (pid == 0) {
		if (execve(cmd[0], cmd, NULL) == -1) {
			std::cout << "\x1b[1;70m" << "> ERROR: Execve fail." << "\n" << "\x1b[0m";
		}
		exit(0);
	}
	for(int i = 0; i < 6; i++) {
		free(cmd[i]);
	}
	delete cmd;
	char	tmp[512];
	sprintf(tmp, "%s %s\n", "BOT",
			": The weather is always Nice.\r\n");
			my_client->setMessage(static_cast<std::string>(tmp));
	return(0);
}

int Bot::playGame(std::string msg) {
	int 	k = -1;
	int		part = 0;
	char	tmp[512];

	if (startGame == 0) {
		sprintf(tmp, "%s %s %s\n", "BOT", ": Make a number from 1 to 100. I'll guess it in 7 tries! READY?",
		"Use the next words: MORE, LESS, YES\r\n");
		my_client->setMessage(static_cast<std::string>(tmp));
		startGame = 1;
	}
	else if (startGame == 1) {
		sprintf(tmp, "%s %s %d?\r\n", "BOT", ": Okay! This number is", gameNum);
		my_client->setMessage(static_cast<std::string>(tmp));
		startGame = 2;
	}
	else if (msg == "YES" && startGame > 1) {
		sprintf(tmp, "%s %s\n", "BOT", ": YAHOO! I WIN!\r\n");
		my_client->setMessage(static_cast<std::string>(tmp));
		startGame = 0;
		gameNum = 50;
	}
	else if (msg == "LESS" || msg == "MORE") {
		if (msg == "MORE") {
			k = 1;
		}
		part = (100 / pow(2, startGame));
		if (part < 1) {
			part = 1;
		}
		gameNum += part * k;
		sprintf(tmp, "%s %s %d?\r\n", "BOT", ": Okay! This number is", gameNum);
		my_client->setMessage(static_cast<std::string>(tmp));
		startGame++;
	}
	else {
		sprintf(tmp, "%s %s\n", "BOT", ": Use the next words: MORE, LESS, YES\r\n");
		my_client->setMessage(static_cast<std::string>(tmp));
		return(-1);
	}
	return(0);
}

void Bot::exitBot() {
	char	tmp[512];

	sprintf(tmp, "%s %s\n", "BOT", ": Bye, bye!\r\n");
	my_client->setMessage(static_cast<std::string>(tmp));
	my_client->setBotDialog(FINISH);
}