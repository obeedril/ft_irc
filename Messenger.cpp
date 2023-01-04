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

bool Messenger::checkRegistered(User* sender) {
	if (!(*sender).getLogin().empty() && !(*sender).getUserName().empty() && !(*sender).getPassword().empty()) 
		(*sender).setRegistFlag(true);
	return((*sender).getRegistFlag());
}

void Messenger::parsRecvStr(std::string str, User* sender, std::map<int, User>::iterator begin, std::map<int, User>::iterator end) {
	std::map<int, Message>::iterator it = messages.find((*sender).getUserFd());
	bool flag = checkRegistered(sender);

	if (str.find("USER", 0) != std::string::npos){
		it->second.setCmd("USER");
		std::cout << ">> FIND cmd USER!" << std::endl;
		if (userCmd(it->second.getRawMessage(), sender, begin, end))
			it->second.setCmd("");
	}
	else if (str.find("NICK", 0) != std::string::npos){
		it->second.setCmd("NICK");
		std::cout << "cmd NICK" << std::endl;
	}
	else if (str.find("PASS", 0) != std::string::npos){
		it->second.setCmd("PASS");
		std::cout << "cmd PASS" << std::endl;
	}
	else if (str.find("QUIT", 0) != std::string::npos && flag == true){
		it->second.setCmd("QUIT");
		std::cout << "cmd QUIT" << std::endl;
	}
	else if (str.find("PRIVMSG", 0) != std::string::npos && flag == true){
		it->second.setCmd("PRIVMSG");
		std::cout << "cmd PRIVMSG" << std::endl;
	}
	else if (str.find("NOTICE", 0) != std::string::npos && flag == true){
		it->second.setCmd("USER");
		std::cout << "cmd NOTICE" << std::endl;
	}
	else if (str.find("JOIN", 0) != std::string::npos && flag == true){
		it->second.setCmd("JOIN");
		std::cout << "cmd JOIN" << std::endl;
	}
	else if (str.find("KICK", 0) != std::string::npos && flag == true){
		it->second.setCmd("USER");
		std::cout << "cmd KICK" << std::endl;
	}
	else if (str.find("CAP LS", 0) != std::string::npos){
		it->second.setCmd("CAP LS");
		std::cout << "cmd CAP LS" << std::endl;
		it->second.setRawMessage("\r\n");
	}
	else if (str.find("BOT", 0) != std::string::npos && flag == true){
		it->second.setCmd("BOT");
		std::cout << "cmd BOT" << std::endl;
	}

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

/* USER command */

std::vector<std::string> Messenger::stringSplit(const std::string &line, char delimiter) {
	std::vector<std::string> words;
	size_t pos = line.find( delimiter );
    size_t initialPos = 0;
    words.clear();
int counter = 0;
    while( pos != std::string::npos && counter++ < 4) {
        words.push_back( line.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = line.find( delimiter, initialPos );
    }

words.push_back( line.substr( initialPos, std::max( pos, line.size() ) - initialPos ));
    return words;
}

std::string Messenger::tostring(std::vector<std::string> &v)
{
    std::stringstream os;
	for (std::vector<std::string>::iterator iter = v.begin(); iter < v.end(); iter++) {
		os << *iter;
	}
    return os.str();
}

int	Messenger::userCmd(const std::string &msg, User* sender, std::map<int, User>::iterator begin, std::map<int, User>::iterator end) {
	// std::cout << "it1->second.getMessage() = " << msg << std::endl;
	std::map<int, Message>::iterator	it1; // !!!
	it1 = messages.find((*sender).getUserFd());
	// int userFd = it1->first;// !!!

	std::vector<std::string> arr = stringSplit(msg, ' ');
	arr.erase(arr.begin()); // удаляем из команды элемент USER
	std::cout << "arr[0]полсе делита = " << arr[0] << std::endl;
	if (arr.size() < 4) {
		// std::string tmp(arr.begin(), arr.end());

    std::stringstream ss;
    for (std::vector<std::string>::iterator it3 = arr.begin(); it3 != arr.end(); it3++)    {

            ss << " ";
        ss << *it3;
    }
 
    std::cout << ss.str() << std::endl;  


		return(replyError(sender, ERR_NEEDMOREPARAMS, tostring(arr), ""));
	}
	for (; begin != end; begin++)
		{
			if (arr[0] == begin->second.getUserName()) { // arr[0] тк слово USER уже удалили
				std::cout << "повтор ника" << std::endl;
				return(replyError(sender, ERR_ALREADYREGISTRED, "", ""));
			}
		}
	(*sender).setUserName(arr[0]);// arr[0] тк слово USER уже удалили

	it1->second.setRawMessage(arr[0] + " " + "Пользователь " + arr[0] + " ворвался в чат!\r\n");
	if (arr[arr.size() - 1][0] == ':')
		(*sender).setRealName(arr[arr.size() - 1].substr(1));
	else
		(*sender).setRealName(arr[arr.size() - 1]);
	// std::cout << "RealName = " << (*sender).getRealName() << std::endl;
	return 0;
}


int		Messenger::replyError(User *user, int err, const std::string &str, const std::string &arg) {
	// std::string	msg = ":" + user.getServName() + " ";
	// std::cout << "1111повтор ника" << std::endl;
	std::string	msg = ":" + user->getServName() + " ";
	std::stringstream	ss;
	ss << err;
	msg += ss.str() + " " + user->getLogin();

	(void)arg; // ?????

	switch (err)
	{
	// case ERR_NOSUCHNICK:
	// 	msg += " " + str + " :No such nick/channel\n";
	// 	break;
	// case ERR_NOSUCHSERVER:
	// 	msg += " " + str + " :No such server\n";
	// 	break;
	// case ERR_NOSUCHCHANNEL:
	// 	msg += " " + str + " :No such channel\n";
	// 	break;
	// case ERR_CANNOTSENDTOCHAN:
	// 	msg += " " + str + " :Cannot send to channel\n";
	// 	break;
	// case ERR_TOOMANYCHANNELS:
	// 	msg += " " + str + " :You have joined too many channels\n";
	// 	break;
	// case ERR_WASNOSUCHNICK:
	// 	msg += " " + str + " :There was no such nickname\n";
	// 	break;
	// case ERR_TOOMANYTARGETS:
	// 	msg += " " + str + " :Duplicate recipients. No str delivered\n";
	// 	break;
	// case ERR_NOORIGIN:
	// 	msg += " :No origin specified\n";
	// 	break;
	// case ERR_NORECIPIENT:
	// 	msg += " :No recipient given (" + str + ")\n";
	// 	break;
	// case ERR_NOTEXTTOSEND:
	// 	msg += " :No text to send\n";
	// 	break;
	// case ERR_NOTOPLEVEL:
	// 	msg += " " + str + " :No toplevel domain specified\n";
	// 	break;
	// case ERR_WILDTOPLEVEL:
	// 	msg += " " + str + " :Wildcard in toplevel domain\n";
	// 	break;
	// case ERR_UNKNOWNCOMMAND:
	// 	msg += " " + str + " :Unknown command\n";
	// 	break;
	// case ERR_NOMOTD:
	// 	msg += " :MOTD File is missing\n";
	// 	break;
	// case ERR_NOADMININFO:
	// 	msg += " " + str + " :No administrative info available\n";
	// 	break;
	// case ERR_FILEERROR:
	// 	msg += " :File error doing \n" + str + " on " + arg + "\n";
	// 	break;
	// case ERR_NONICKNAMEGIVEN:
	// 	msg += " :No nickname given\n";
	// 	break;
	// case ERR_ERRONEUSNICKNAME:
	// 	msg += " " + str + " :Erroneus nickname\n";
	// 	break;
	// case ERR_NICKNAMEINUSE:
	// 	msg += " " + str + " :Nickname is already in use\n";
	// 	break;
	// case ERR_NICKCOLLISION:
	// 	msg += " " + str + " :Nickname collision KILL\n";
	// 	break;
	// case ERR_USERNOTINCHANNEL:
	// 	msg += " " + str + " " + arg + " :They aren't on that channel\n";
	// 	break;
	// case ERR_NOTONCHANNEL:
	// 	msg += " " + str + " :You're not on that channel\n";
	// 	break;
	// case ERR_USERONCHANNEL:
	// 	msg += " " + str + " " + arg + " :is already on channel\n";
	// 	break;
	// case ERR_NOLOGIN:
	// 	msg += " " + str + " :User not logged in\n";
	// 	break;
	// case ERR_SUMMONDISABLED:
	// 	msg += " :SUMMON has been disabled\n";
	// 	break;
	// case ERR_USERSDISABLED:
	// 	msg += " :USERS has been disabled\n";
	// 	break;
	// case ERR_NOTREGISTERED:
	// 	msg += " :You have not registered\n";
	// 	break;
	case ERR_NEEDMOREPARAMS:
		msg += " " + str + " :Not enough parameters\n";
		break;
	case ERR_ALREADYREGISTRED:
		msg += " :You may not reregister\n";
		break;
	// case ERR_NOPERMFORHOST:
	// 	msg += " :Your host isn't among the privileged\n";
	// 	break;
	// case ERR_PASSWDMISMATCH:
	// 	msg += " :Password incorrect\n";
	// 	break;
	// case ERR_YOUREBANNEDCREEP:
	// 	msg += " :You are banned from this server\n";
	// 	break;
	// case ERR_KEYSET:
	// 	msg += " " + str + " :Channel key already set\n";
	// 	break;
	// case ERR_CHANNELISFULL:
	// 	msg += " " + str + " :Cannot join channel (+l)\n";
	// 	break;
	// case ERR_UNKNOWNMODE:
	// 	msg += " " + str + " :is unknown mode char to me\n";
	// 	break;
	// case ERR_INVITEONLYCHAN:
	// 	msg += " " + str + " :Cannot join channel (+i)\n";
	// 	break;
	// case ERR_BANNEDFROMCHAN:
	// 	msg += " " + str + " :Cannot join channel (+b)\n";
	// 	break;
	// case ERR_BADCHANNELKEY:
	// 	msg += " " + str + " :Cannot join channel (+k)\n";
	// 	break;
	// case ERR_NOPRIVILEGES:
	// 	msg += " :Permission Denied- You're not an IRC operator\n";
	// 	break;
	// case ERR_CHANOPRIVSNEEDED:
	// 	msg += " " + str + " :You're not channel operator\n";
	// 	break;
	// case ERR_CANTKILLSERVER:
	// 	msg += " :You cant kill a server!\n";
	// 	break;
	// case ERR_NOOPERHOST:
	// 	msg += " :No O-lines for your host\n";
	// 	break;
	// case ERR_UMODEUNKNOWNFLAG:
	// 	msg += " :Unknown MODE flag\n";
	// 	break;
	// case ERR_USERSDONTMATCH:
	// 	msg += " :Cant change mode for other users\n";
	// 	break;
	default:
		msg += "UNKNOWN ERROR\n";
		break;
	}
	// std::cout << "222повтор ника" << std::endl;
	send(user->getUserFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	// std::cout << "333повтор ника" << std::endl;
	return (-1);
}

