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
			bot_com = static_cast<t_bot_comm>(i);
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
			sprintf(tmp, "%s %s\n", "BOT", "Hi! I'm Robby the robot! Do you want to PLAY numbers or find out the WEATHER?\r\n");
			my_client->setMessage(static_cast<std::string>(tmp));
			my_client->setBotDialog(true);
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
			"Its Robby robot! I still here =) Pick one of them command PLAY, WEATHER or BYE_BOT.\r\n");
			my_client->setMessage(static_cast<std::string>(tmp));
			break;
	}
	return 0;
}

int Bot::getWeather() {
	return(0);
}

int Bot::playGame(std::string msg) {
	int 	k = 0;
	char	tmp[4048];
	int		index;
	std::string substr[4] = {"LESS", "MORE", "YES", "PLAY"};
	for(int i = 0; i < 4; i++) {
		std::cout << substr[i] << std::endl;
		index = msg.find(substr[i], 0);
		if (index != 0 && i == 3) {
			sprintf(tmp, "%s %s\n", "BOT", "Use Next words: MORE, LESS, YES?\r\n");
			my_client->setMessage(static_cast<std::string>(tmp));
			return(-1);
		}
	}
	if (startGame == 0) {
		sprintf(tmp, "%s %s\n", "BOT", "Make a number from 1 to 100. I'll guess it in 7 tries! READY?\r\n");
		my_client->setMessage(static_cast<std::string>(tmp));
		startGame = 1;
	}
	else if (msg == "YES") {
		sprintf(tmp, "%s %s %d?\n", "BOT", "YAHOO! I WIN!\r\n", gameNum);
		my_client->setMessage(static_cast<std::string>(tmp));
		startGame = 0;
		gameNum = 50;
	}
	else {
		if (msg == "MORE") {
			k = 1;
		}
		if (msg == "LESS") {
			k = -1;
		}
		if (startGame == 1) {
			sprintf(tmp, "%s %s %d?\r\n", "BOT", "Okay! This number is", gameNum);
			my_client->setMessage(static_cast<std::string>(tmp));
			startGame = 2;
		}
		else if (startGame == 2) {
			gameNum += (100 / 4) * k;
			sprintf(tmp, "%s %s %d?\r\n", "BOT", "Okay! This number is", gameNum);
			my_client->setMessage(static_cast<std::string>(tmp));
			startGame = 3;
		}
		else if (startGame == 3) {
			gameNum += (100 / 8) * k;
			sprintf(tmp, "%s %s %d?\r\n", "BOT", "Okay! This number is", gameNum);
			my_client->setMessage(static_cast<std::string>(tmp));
			startGame = 4;
		}
		else if (startGame == 4) {
			gameNum += (100 / 16) * k;
			sprintf(tmp, "%s %s %d?\r\n", "BOT", "Okay! This number is", gameNum);
			my_client->setMessage(static_cast<std::string>(tmp));
			startGame = 5;
		}
		else if (startGame == 5) {
			gameNum += (100 / 32) * k;
			sprintf(tmp, "%s %s %d?\r\n", "BOT", "Okay! This number is", gameNum);
			my_client->setMessage(static_cast<std::string>(tmp));
			startGame = 6;
		}
		else if (startGame == 6) {
			gameNum += 1 * k;
			sprintf(tmp, "%s %s %d?\r\n", "BOT", "Okay! This number is", gameNum);
			my_client->setMessage(static_cast<std::string>(tmp));
			startGame = 7;
		}
		else if (startGame == 7) {
			gameNum += 1 * k;
			sprintf(tmp, "%s %s %d?\n", "BOT", "Okay! This number is", gameNum);
			my_client->setMessage(static_cast<std::string>(tmp));
			startGame = 8;
		}
	}
	return(0);
}

void Bot::exitBot() {
	char		tmp[4048];
	sprintf(tmp, "%s %s\n", "BOT", "Bye, bye!\r\n");
	my_client->setMessage(static_cast<std::string>(tmp));
	my_client->setBotDialog(false);
}