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

std::string Messenger::getMessageByFd(int senderFd){
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		return it->second.getRestMess();
	return "";
}


void Messenger::parseBuffer(int senderFd){

	std::cout << "AAAAAA" << std::endl;

// std::string message;
// std::string command;
size_t pos; // Olya, privet!!!!!!! 

std::map<int, Message>::iterator it1 = messages.find(senderFd);
if (it1 != messages.end()){

	std::cout << "BBBB" << std::endl;


	// std::map<int, User>::iterator it1 = map_users.find(user_fd);
	// if (it1 == map_users.end()) {
	// 	write(2, "Users not found\n", 26);
	// 	return ; ////!!!!
	// }

	pos = it1->second.getRawMessage().find(":", 0);
	size_t posEnd = it1->second.getRawMessage().find("\r\n", pos);
	if (pos != std::string::npos){
		it1->second.setPrefix(it1->second.getRawMessage().substr(0, pos));
		it1->second.setRawMessage(it1->second.getRawMessage().substr(pos + 1, posEnd));
	}
	std::cout << "CCCCCC" << std::endl;
	pos = it1->second.getRawMessage().find(" ", 0);
	if (pos != std::string::npos && posEnd != std::string::npos) {
		std::cout << "DDDDD" << std::endl;
		it1->second.setCmd(it1->second.getRawMessage().substr(0, pos));
		it1->second.setRawMessage(it1->second.getRawMessage().substr(pos + 1, posEnd));

		// pos = message.find(" ", 0);
		// mess.receiver = message.substr(0, pos);
		// message = message.substr(pos + 1, posEnd);
		it1->second.setRestMess(it1->second.getRawMessage().substr(0, it1->second.getRawMessage().find("\n") - 1));

		// std::string tmp = message.substr(pos + 1, posEnd);

		// it1->second.setMessage(tmp.substr(0, tmp.find("\n") - 1)); // добавить

		std::cout << "command |" << it1->second.getCmd()  << "|" << std::endl;
		// std::cout << "message |" << mess.receiver << "|" << std::endl;
		std::cout << "restMess |" << it1->second.getRestMess() << "|" << std::endl;

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