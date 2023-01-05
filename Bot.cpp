#include "Library.hpp"

Bot::Bot(User *my_client) {
	gameNum = 50;
	bot_com = NO_COMM;
	this->my_client = my_client;
	startGame = 0;
	bad_dialog = 0;
	message = "";
}

Bot::~Bot() {

}

int Bot::callBot(std::string msg) {
	bot_com = NO_COMM;
	char		tmp[4048];
	int			index = 0;
	std::string substr[5] = {"BOT", "PLAY", "WEATHER", "BYE_BOT"};
	std::string new_msg = "";

	for(int i = 0; i < 5; i++) {
		//std::cout << substr[i] << std::endl;
		index = msg.find(substr[i], 0);
		if (index == 0) {
			bot_com = static_cast<t_bot_command>(i);
			break;
		}
	}
	if (startGame > 0 && bad_dialog <= 3) {
		bot_com = PLAY;
	}
	else if (bad_dialog > 3) {
		bot_com = BYE_BOT;
	}
	std::cout << "bot_com: " << bot_com << std::endl;
	std::cout << "msg: " << msg << std::endl;
	switch (bot_com)
	{
		case HELLO_BOT: 
			sprintf(tmp, "%s %s\n", "BOT", ": Hi! I'm Kitty the robot! Do you want to PLAY numbers or find out the WEATHER?\r\n");
			new_msg = tmp;
			message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
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
			message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
			bad_dialog++;
			break;
	}
	return 0;
}

int Bot::getWeather() {
		int fd;
	fd = open("weather.html", O_WRONLY | O_CREAT | O_TRUNC, 0755);
	
	pid_t	pid;
	char	**cmd;
	std::string curl("/usr/bin/curl");
	std::string cmd_1("-X");
	std::string cmd_2("GET");
	std::string site("https://p.ya.ru/?via=f");

	cmd = new char*[4];
	for(int i = 0; i < 4; i++) {
		cmd[i] = NULL;
	}
	cmd[0] = strdup(curl.c_str());
	cmd[1] = strdup(cmd_1.c_str());
	cmd[2] = strdup(cmd_2.c_str());
	cmd[3] = strdup(site.c_str());
	cmd[4] = NULL;

	pid = fork();
	if (pid != 0) {
		waitpid(pid, NULL, 0);
	}
	else if (pid == 0) {
		dup2(fd, 1);
		close(fd);
		if (execve(curl.c_str(), cmd, NULL) == -1) {
			std::cout << "\x1b[1;70m" << "> ERROR: Execve fail." << "\n" << "\x1b[0m";
		}
		exit(0);
	}
	for(int i = 0; i < 4; i++) {
		free(cmd[i]);
	}
	delete cmd;
	close(fd);
	parserWeather();
	remove("weather.html");
	return(bad_dialog = 0);
}


int Bot::parserWeather() {
	size_t			pos;
	size_t			p_1;
	size_t			p_2;
	std::string		line = "";
	std::string		get_weather = "";
	std::string		substr_weather = "";
	std::string		div[4] = {"\"today-forecast\"", "\"preposition\"", "\"city\"", "\"temp\""};
    std::ifstream	in("weather.html"); 

    if (in.is_open()) {
        getline(in, line);
		for (int i = 0; i < 4; i++) {
			pos = line.find(div[i]);
			if (pos != std::string::npos) {
				p_1 = line.find(">", pos);
				p_2 = line.find("<", pos);
				if (p_1 != std::string::npos && p_2 != std::string::npos) {
					if (i == 0) {
						get_weather.append(line.substr(p_1 + 1, 12));
						substr_weather.append(line.substr(p_1 + 13, p_2 - p_1 - 13));
					}
					else {
						get_weather.append(" ");
						get_weather.append(line.substr(p_1 + 1, p_2 - p_1 - 1));
						if (i == 3) {
							get_weather.append(",");
							get_weather.append(substr_weather);
						}
					}
				}
			}
			else {
				break;
			}
		}
    }
    in.close(); 
	char	tmp[512];
	if (get_weather.length() == 0) {
		get_weather.append(": The weather is always Nice.\r\n");
	}
	sprintf(tmp, "%s : %s\n", "BOT", get_weather.c_str());
	message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
	return(0);
}

int Bot::playGame(std::string msg) {
	int 	k = -1;
	int		part = 0;
	char	tmp[512];

	if (startGame == 0) {
		sprintf(tmp, "%s %s %s\n", "BOT", ": Make a number from 1 to 100. I'll guess it in 7 tries! READY?",
		"Use the next words: MORE, LESS, YES\r\n");
		message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
		startGame = 1;
	}
	else if (startGame == 1) {
		sprintf(tmp, "%s %s %d?\r\n", "BOT", ": Okay! This number is", gameNum);
		message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
		startGame = 2;
	}
	else if (msg.find("YES", 0) != std::string::npos && startGame > 1) {
		sprintf(tmp, "%s %s\n", "BOT", ": YAHOO! I WIN!\r\n");
		message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
		startGame = 0;
		gameNum = 50;
	}
	else if (msg.find("LESS", 0) != std::string::npos || msg.find("MORE", 0) != std::string::npos) {
		if (msg.find("MORE", 0) != std::string::npos) {
			k = 1;
		}
		part = (100 / pow(2, startGame));
		if (part < 1) {
			part = 1;
		}
		gameNum += part * k;
		sprintf(tmp, "%s %s %d?\r\n", "BOT", ": Okay! This number is", gameNum);
		message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
		startGame++;
	}
	else {
		sprintf(tmp, "%s %s\n", "BOT", ": Use the next words: MORE, LESS, YES\r\n");
		message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
		return(++bad_dialog);
	}
	return(bad_dialog = 0);
}

void Bot::exitBot() {
	char	tmp[512];
	if (bad_dialog > 3) {
		sprintf(tmp, "%s %s\n", "BOT", ": It is so boooored =( Bye, bye!\r\n");
	}
	else {
		sprintf(tmp, "%s %s\n", "BOT", ": Bye, bye!\r\n");
	}
	message.append(tmp);//my_client->setMessage(static_cast<std::string>(tmp));
	my_client->setBotDialog(FINISH);
	bad_dialog = 0;
}

std::string Bot::getMsg() {
	return message;
}

void Bot::setMsg(std::string msg) {
	message.clear();
	message = "";
	message = msg;
}