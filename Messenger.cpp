#include "Library.hpp"

Messenger::Messenger(){



}

Messenger::~Messenger(){

}

// void Messenger::parseBuffer(std::string buf, int user_fd){

	void Messenger::parseBuffer(char*buf, User &user){
	
	t_message mess;

	std::string message;
	// std::string command;
	size_t pos; // Olya, privet!!!!!!! 
	// привееееет! :)

	message.append(buf);
;
	// std::map<int, User>::iterator it1 = map_users.find(user_fd);
	// if (it1 == map_users.end()) {
	// 	write(2, "Users not found\n", 26);
	// 	return ; ////!!!!
	// }

	pos = message.find(" ", 0);
	size_t posEnd = message.find("\r\n", pos);
	if (pos != std::string::npos && posEnd != std::string::npos) {
		mess.cmd = message.substr(0, pos);
		message = message.substr(pos + 1, posEnd);

		pos = message.find(" ", 0);
		mess.receiver = message.substr(0, pos);
		message = message.substr(pos + 1, posEnd);
		mess.restMess = message.substr(0, message.find("\n") - 1);

		// std::string tmp = message.substr(pos + 1, posEnd);

		// it1->second.setMessage(tmp.substr(0, tmp.find("\n") - 1)); // добавить

		std::cout << "command |" << mess.cmd  << "|" << std::endl;
		std::cout << "message |" << mess.receiver << "|" << std::endl;
		std::cout << "restMest |" << mess.restMess<< "|" << std::endl;

		// pair<iterator,bool> insert (const value_type& val)
		messages.insert(std::pair<int, t_message> (user.getUserFd(), mess));

		// std::string commands[8] = {"USER", "NICK",	"PASS",    "QIUT",	"PRIVMSG",	"NOTICE", "JOIN", "KICK"};

		
		// for (int i = 0; i < 8; i++){
		// 	if (commands[i] == mess.cmd) {
		// 		it1->second.setCommand(static_cast<t_command>(i));
		// 	}
		// }
	}


}