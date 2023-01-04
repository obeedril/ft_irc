#include "Library.hpp"

Messenger::Messenger() {

}

Messenger::~Messenger() {

}

void Messenger::insertMessage(int senderFd, Message mess) {
	messages.insert(std::pair<int, Message> (senderFd, mess));
}

void Messenger::deleteMessage(int senderFd) {
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		messages.erase(it);
}

void Messenger::setMessages(std::map<int, Message> _messages) {
	messages = _messages;
}

std::map<int, Message> Messenger::getMessages() {
	return messages;
}

std::string Messenger::getRawMessageByFd(int senderFd) {
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		return it->second.getRawMessage();
	return "";
}

void Messenger::parsRecvStr(std::string str, User* sender) {

	std::map<int, Message>::iterator it = messages.find((*sender).getUserFd());
	//bool flag = checkRegistered(sender);

	if (str.find("USER", 0) != std::string::npos) {
		it->second.setCmd("USER");
		std::cout << "cmd USER" << std::endl;
	}
	if (str.find("NICK", 0) != std::string::npos) {
		it->second.setCmd("NICK");
		std::cout << "cmd NICK" << std::endl;
	}
	if (str.find("PASS", 0) != std::string::npos) {
		it->second.setCmd("PASS");
		std::cout << "cmd PASS" << std::endl;
	}
	if (str.find("QUIT", 0) != std::string::npos) {
		it->second.setCmd("QUIT");
		std::cout << "cmd QUIT" << std::endl;
	}
	if (str.find("PRIVMSG", 0) != std::string::npos) {
		it->second.setCmd("PRIVMSG");
		std::cout << "cmd PRIVMSG" << std::endl;
	}
	if (str.find("NOTICE", 0) != std::string::npos) {
		it->second.setCmd("USER");
		std::cout << "cmd NOTICE" << std::endl;
	}
	if (str.find("JOIN", 0) != std::string::npos) {
		it->second.setCmd("JOIN");
		std::cout << "cmd JOIN" << std::endl;
	}
	if (str.find("KICK", 0) != std::string::npos) {
		it->second.setCmd("USER");
		std::cout << "cmd KICK" << std::endl;
	}
	if (str.find("CAP LS", 0) != std::string::npos) {
		it->second.setCmd("CAP LS");
		std::cout << "cmd CAP LS" << std::endl;
	}
	if (str.find("BOT", 0) != std::string::npos || sender->getBotDialog() == YES) {
		it->second.setCmd("BOT");
		it->second.setRawMessage(initBot(sender, it->second.getRawMessage()));
		std::cout << "cmd BOT" << std::endl;
	}
}

std::string Messenger::initBot(User *my_client, std::string msg) {
	std::string		tmp = "";
	std::map<int, Bot>::iterator it1 = map_robots.find(my_client->getUserFd());

	if (it1 == map_robots.end()) {
		std::cout << "new BOT: " << msg << std::endl;
		my_client->setBotDialog(YES);
		Bot new_bot(my_client);
		map_robots.insert(std::pair<int, Bot> (my_client->getUserFd(), new_bot));
		std::cout << "BEFORE callBot" << std::endl;
		new_bot.callBot(msg);
		std::cout << "AFTER callBot" << std::endl;
		tmp = new_bot.getMsg();
		new_bot.setMsg("");
	}
	else {
		std::cout << "old BOT: " << msg << std::endl;
		it1->second.callBot(msg);
		tmp = it1->second.getMsg();
		it1->second.setMsg("");
	}
	std::cout << "END initBot" << std::endl;
	return(tmp);
}

void Messenger::deleteBot(int senderFd) {
	std::map<int, Bot>::iterator it = map_robots.find(senderFd);
	if (it != map_robots.end()) {
		map_robots.erase(it);
	}
}
