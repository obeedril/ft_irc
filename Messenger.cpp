#include "Library.hpp"

Messenger::Messenger(){
}

Messenger::~Messenger(){

}

void Messenger::insertMessage(int senderFd, Message mess){
	messages.insert(std::pair<int, Message> (senderFd, mess));
}

void Messenger::deleteMessage(int senderFd){
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		messages.erase(it);
}

void Messenger::setMessages(std::map<int, Message> _messages){
	messages = _messages;
}

std::map<int, Message> Messenger::getMessages(){
	return messages;
}

std::string Messenger::getRawMessageByFd(int senderFd){
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		return it->second.getRawMessage();
	return "";
}

std::string Messenger::parsRecvStr(std::string str){

	std::string cmd = "";

	if (str.find("USER", 0) != std::string::npos){
		cmd = "USER";
		std::cout << "cmd USER" << std::endl;
	}
	if (str.find("NICK", 0) != std::string::npos){
		cmd = "NICK";
		std::cout << "cmd NICK" << std::endl;
	}
	if (str.find("PASS", 0) != std::string::npos){
		cmd = "PASS";
		std::cout << "cmd PASS" << std::endl;
	}
	if (str.find("QUIT", 0) != std::string::npos){
		cmd = "QUIT";
		std::cout << "cmd QUIT" << std::endl;
	}
	if (str.find("PRIVMSG", 0) != std::string::npos){
		cmd = "PRIVMSG";
		std::cout << "cmd PRIVMSG" << std::endl;
	}
	if (str.find("NOTICE", 0) != std::string::npos){
		cmd = "NOTICE";
		std::cout << "cmd NOTICE" << std::endl;
	}
	if (str.find("JOIN", 0) != std::string::npos){
		cmd = "JOIN";
		std::cout << "cmd JOIN" << std::endl;
	}
	if (str.find("KICK", 0) != std::string::npos){
		cmd = "KICK";
		std::cout << "cmd KICK" << std::endl;
	}
	if (str.find("CAP LS", 0) != std::string::npos){
		cmd = "CAP LS";
		std::cout << "cmd CAP LS" << std::endl;
	}
	if (str.find("BOT", 0) != std::string::npos){
		cmd = "BOT";
		std::cout << "cmd BOT" << std::endl;
	}
	return cmd;
}

void Messenger::parseBuffer(int senderFd){

	std::cout << "AAAAAA" << std::endl;

// std::string message;
// std::string command;
size_t pos; 

std::map<int, Message>::iterator it = messages.find(senderFd);
if (it != messages.end()){

	std::cout << "BBBB" << std::endl;
	std::cout << "it1->second.getRawMessage()" << it->second.getRawMessage() << "|" << std::endl;


	// std::map<int, User>::iterator it1 = map_users.find(user_fd);
	// if (it1 == map_users.end()) {
	// 	write(2, "Users not found\n", 26);
	// 	return ; ////!!!!
	// }

	it->second.setRawMessage(it->second.getRawMessage().append("\r\n"));
	pos = it->second.getRawMessage().find(":", 0);
	size_t posEnd = it->second.getRawMessage().find("\r\n", pos);
	
	if (pos != std::string::npos){
		it->second.setPrefix(it->second.getRawMessage().substr(0, pos));
		it->second.setRawMessage(it->second.getRawMessage().substr(pos + 1, posEnd));
	}
		std::cout << "it1->second.getPrefix()" << it->second.getPrefix() << "|" << std::endl;

	std::cout << "CCCCCC" << std::endl;
	std::cout << "it1->second.getRawMessage()" << it->second.getRawMessage() << "|" << std::endl;

	pos = it->second.getRawMessage().find(" ", 0);
	std::cout << "pos = " << pos << "|" << std::endl;
	std::cout << "posEnd = " << posEnd << "|" << std::endl;
	if (pos != std::string::npos && posEnd != std::string::npos) {
		std::cout << "DDDDD" << std::endl;
		it->second.setCmd(it->second.getRawMessage().substr(0, pos));
		it->second.setRawMessage(it->second.getRawMessage().substr(pos + 1, posEnd));

		// pos = message.find(" ", 0);
		// mess.receiver = message.substr(0, pos);
		// message = message.substr(pos + 1, posEnd);
		it->second.setRestMess(it->second.getRawMessage().substr(0, it->second.getRawMessage().find("\n") - 1));

		// std::string tmp = message.substr(pos + 1, posEnd);

		// it1->second.setMessage(tmp.substr(0, tmp.find("\n") - 1)); // добавить

		std::cout << "command |" << it->second.getCmd()  << "|" << std::endl;
		// std::cout << "message |" << mess.receiver << "|" << std::endl;
		std::cout << "restMess |" << it->second.getRestMess() << "|" << std::endl;

		// pair<iterator,bool> insert (const value_type& val)
		// messages.insert(std::pair<int, t_message> (user.getUserFd(), mess));
		//insertMessage(user, message);

		// std::string commands[8] = {"USER", "NICK",	"PASS",    "QIUT",	"PRIVMSG",	"NOTICE", "JOIN", "KICK"};

		
		// for (int i = 0; i < 8; i++){
		// 	if (commands[i] == mess.cmd) {
		// 		it1->second.setCommand(static_cast<t_command>(i));
		// 	}
		// }
	}
}


}