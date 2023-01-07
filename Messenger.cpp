#include "Library.hpp"

Messenger::Messenger() {
	ChannelsStorage channels;
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

void Messenger::setReadyMessInMessageByFd(std::string str, int senderFd) {
	std::map<int, Message>::iterator it = messages.find(senderFd);
	std::string tmp = it->second.getReadyMess();
	if (!tmp.empty())
		tmp = "";
	it->second.setReadyMess(str);
}

void Messenger::setRawMessInMessageByFd(std::string str, int senderFd) {
	std::map<int, Message>::iterator it = messages.find(senderFd);
	std::string tmp = it->second.getRawMessage();
	if (!tmp.empty())
		tmp = "";
	it->second.setRawMessage(str);
}

std::map<int, Message> Messenger::getMessages() {
	return messages;
}

std::string Messenger::getRawMessageByFd(int senderFd) {
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end()) {
		//std::cout << "senderFd " << senderFd << std::endl;
		//std::cout << "getRawMessageByFd INN:" << it->second.getRawMessage() << std::endl;
		return it->second.getRawMessage();
	}
	return "";
}


std::string Messenger::getReadyMessByFd(int senderFd) {

	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		return it->second.getReadyMess();
	return "";
}

std::string Messenger::getSystemMsg(int senderFd) {
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		return it->second.getMessForSender();
	return "";
}

std::string Messenger::getCmdInMessageByFd(int senderFd) {
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		return it->second.getCmd();
	return "";
}


void Messenger::parserPrivmsg(Message &mess){
	std::string tmp = "";
	std::string recievers_str = "";
	std::vector<std::string> tmp_list;
	size_t len = mess.getRawMessage().length();
	tmp = mess.getRawMessage().substr(mess.getCmd().length() + 1, len);

	size_t pos = tmp.find(":", 0);
	recievers_str = tmp.substr(0, pos);
	std::cout << "recivers_str |" << recievers_str << "|" << std::endl;

	tmp_list = splitString(recievers_str, ',');

	// for(int i = 0; i < static_cast<int>(tmp_list.size()); i++) {
    //     std::cout << "'" << tmp_list[i] << "' ";
    // }
	// std::cout << std::endl;

	for(int i = 0; i < static_cast<int>(tmp_list.size()); i++) {
       tmp_list[i] = strTrimBegin(tmp_list[i], ' ');
    }


//------------------------------------------------------
    // for(int i = 0; i < static_cast<int>(tmp_list.size()); i++) {
    //     std::cout << "'" << tmp_list[i] << "' ";
    // }
	// std::cout << std::endl;

//------------------------------------------------------

	mess.setListOfRecievers(tmp_list);

	if (mess.getListOfRecievers().empty()){
		std::cout << "recivers are not identifire" << std::endl;
		return;
	}
	
	// mess.setReadyMess(":Sender!Sender@127.0.0.1 PRIVMSG " +tmp.substr(pos + 1, len));
	mess.setReadyMess(tmp.substr(pos + 1, len) + "\n");

	//------------------------------------------------------
    // for(int i = 0; i < static_cast<int>(mess.getListOfRecievers().size()); i++) {
    //     std::cout << "'" << mess.getListOfRecievers()[i] << "' ";
    // }
	// std::cout << std::endl;

//------------------------------------------------------

	// ------------------------------
	// std::cout << "tmp |" << tmp << "|" << std::endl;
	// std::cout << "readyMess |" << mess.getReadyMess() << "|" << std::endl;
	// std::cout << "rawMess |" << mess.getRawMessage() << "|" << std::endl;
	// ------------------------------

}

bool Messenger::checkRegistered(int userFd) {
	std::map<int, User>::iterator it_user = map_users.find(userFd);
	if (it_user->second.getLogin() != ""
			&& it_user->second.getUserName() != ""
			&& it_user->second.getPassword() != "") 
		it_user->second.setRegistFlag(true);
	return(it_user->second.getRegistFlag());
}

std::vector<int> Messenger::getDeq(int senderFd) {
	std::vector<int> tmp;
	std::map<int, Message>::iterator it = messages.find(senderFd);
	if (it != messages.end())
		tmp = it->second.getDeque();
	return (tmp);
}

void Messenger::addUser(int user_fd, User new_user) {
	std::map<int, User>::iterator it1 = map_users.find(user_fd);
	if (it1 != map_users.end()) {
		write(2, "addUser ERROR\n", 17);
		return ; ////!!!!
	}
	else {
		map_users.insert(std::pair<int, User> (user_fd, new_user));
	}
}

int  Messenger::getUserFd(int Fd) {
	std::map<int, User>::iterator it1 = map_users.find(Fd);
	if (it1 == map_users.end()) {
		write(2, "getUserFd ERROR\n", 17);
		return -1; ////!!!!
	}
	return (it1->second.getUserFd());
}

void Messenger::parsRecvStr(std::string str, int userFd) {
	std::map<int, Message>::iterator it = messages.find(userFd);
	std::map<int, User>::iterator it_user = map_users.find(userFd);
	bool flag = checkRegistered(userFd);
	dequeMaker(&it_user->second, TO_ALL_BUT_NO_THIS_USER);
	flag = true; // >>>>>> del!!!!!!!!!
	
	// std::vector<int> deque_users;
	//std::map<int, User>::iterator it_u = map_users.begin();
	// for (std::map<int, User>::iterator it_u = map_users.begin(); it_u != map_users.end(); it_u++) {
		// if (it_u->first != userFd)
			// deque_users.push_back(it_u->first);
	// }
	// it->second.setDeque(deque_users);

	if (str.find("PASS", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setCmd("PASS");
		passCmd(it->second.getRawMessage(), &it_user->second);
	}
	else if (str.find("NICK", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		if (nickCmd(it->second.getRawMessage(), &it_user->second) == 0)
			it->second.setCmd("NICK");
		else 
			it->second.setCmd("");
	}
	else if (str.find("USER", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		if (userCmd(it->second.getRawMessage(), &it_user->second) == 0)
			it->second.setCmd("USER");
		else 
			it->second.setCmd("");
	}
	else if (str.find("QUIT", 0) != std::string::npos && flag == true){
		it->second.setCmd("QUIT");
		std::cout << "cmd QUIT" << std::endl;
	}
	else if (str.find("PRIVMSG", 0) != std::string::npos && flag == true){
		it->second.setCmd("PRIVMSG");
		parserPrivmsg(it->second);
		dequeMaker(&it_user->second, LIST_OF_RECIEVERS);
		std::cout << "cmd PRIVMSG" << std::endl;
	}
	else if (str.find("NOTICE", 0) != std::string::npos && flag == true){
		it->second.setCmd("NOTICE");
		std::cout << "cmd NOTICE" << std::endl;
	}
	else if (str.find("JOIN", 0) != std::string::npos && flag == true) {
		it->second.setCmd("JOIN");
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setMessForSender(channels.joinToCannel(str, &it_user->second));
		std::cout << "cmd JOIN" << std::endl;
	}
	else if (str.find("KICK", 0) != std::string::npos && flag == true) {
		it->second.setCmd("KICK");
		findNameKick(it->second);
		it->second.setMessForSender(channels.kickUser(str, &it_user->second));
		std::cout << "cmd KICK" << std::endl;
	}
	else if (str.find("CAP LS", 0) != std::string::npos) {
		it->second.setCmd("CAP LS");
		std::cout << "cmd CAP LS" << std::endl;
		it->second.setReadyMess("rrrr\n");
	}
	else if ((str.find("BOT", 0) != std::string::npos || it_user->second.getBotDialog() == YES)
				&& flag == true) {
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setCmd("BOT");
		it_user->second.setBotDialog(YES);
		it->second.setMessForSender(initBot(userFd, it->second.getRawMessage()));
		if (it_user->second.getBotDialog() == FINISH) {
			std::cout << "deleteBot" << std::endl;
			deleteBot(userFd);
		}
	}
}

std::string Messenger::findNameKick(Message mess) {
	std::vector<int> deque; 
	deque = mess.getDeque();
	for(int i = 0; i < static_cast<int>(deque.size()); i++) {
		std::map<int, User>::iterator it_user = map_users.find(deque[i]);
		if (it_user != map_users.end()) {
			return it_user->second.getUserName();
		}
	}
	return "";
}

std::string Messenger::initBot(int user_fd, std::string msg) {
	std::string		tmp = "";
	std::map<int, Bot>::iterator it1 = map_robots.find(user_fd);
	std::map<int, User>::iterator it_user = map_users.find(user_fd);

	if (it1 == map_robots.end()) {
		std::cout << "new BOT: " << msg << std::endl;
		Bot new_bot(&it_user->second);
		map_robots.insert(std::pair<int, Bot> (user_fd, new_bot));
		new_bot.callBot(msg);
		tmp = new_bot.getMsg();
		new_bot.setMsg("");
	}
	else {
		std::cout << "old BOT: " << msg << std::endl;
		it1->second.callBot(msg);
		tmp = it1->second.getMsg();
		it1->second.setMsg("");
	}
	return(tmp);
}


/* USER command */


int	Messenger::userCmd(const std::string &msg, User* sender) {
	if (sender->getLogin() == "") {
		printWelcome(sender, "Please enter your PASS and NICK before USER-command", 0);
		setReadyMessInMessageByFd(msg, sender->getUserFd());
		setRawMessInMessageByFd(msg, sender->getUserFd());
		return 1;
	}
	// std::cout << "it1->second.getMessage() = " << msg << std::endl;
	std::map<int, Message>::iterator	it1; // !!!
	it1 = messages.find((*sender).getUserFd());
	// int flag = 0;
	// if (!sender->getUserName().empty())
	// 	flag = 1;
	// int userFd = it1->first;// !!!

	std::vector<std::string> arr = stringSplit(msg, ' ');
	arr.erase(arr.begin()); // удаляем из команды элемент USER
	// std::cout << "arr[0]полсе делита = " << arr[0] << std::endl;
	if (arr.size() < 4) {
		return(replyError(sender, ERR_NEEDMOREPARAMS, tostring(arr), ""));
	}
	if (sender->getUserName() != "" && arr[0] == sender->getUserName()) {
		return(replyError(sender, ERR_ALREADYREGISTRED, "", ""));
	}
	(*sender).setUserName(arr[0]);// arr[0] тк слово USER уже удалили

	if (arr[arr.size() - 1][0] == ':')
		(*sender).setRealName(arr[arr.size() - 1].substr(1));
	else
		(*sender).setRealName(arr[arr.size() - 1]);
	printWelcome(sender, arr[0], 1);
	return 0;
}

std::vector<std::string> Messenger::stringSplit(const std::string &line, char delimiter) {
	size_t posRN = line.find("\r\n");
	std::string tmp;
	if (posRN != std::string::npos) {
		tmp = line.substr(0, posRN);
	}
	else {
		tmp = line;
	}
	std::vector<std::string> words;
	size_t pos = tmp.find( delimiter );
    size_t initialPos = 0;
    words.clear();
int counter = 0;
    while( pos != std::string::npos && counter++ < 4) {
        words.push_back( tmp.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = tmp.find( delimiter, initialPos );
    }

words.push_back( tmp.substr( initialPos, std::max( pos, tmp.size() ) - initialPos ));
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

void	Messenger::sendMotd(User* sender) {
	std::vector<std::string> vec = sender->getMotdFromServer();
	std::string tmp = tostring(vec);
	if (tmp.empty())
		replyError(sender, ERR_NOMOTD, "", "");
	else
	{
		std::string	msg = ":" + sender->getUserName() + " ";
		std::stringstream	ss;
		ss << RPL_MOTDSTART;
		msg += ss.str() + " " + sender->getUserName() + " ";
		msg += ":- " + sender->getServName() + " Message of the day - \r\n";
		ss.str( "" );
		ss << RPL_MOTD; //????
		msg += ss.str() + " " + sender->getUserName() + " ";
		msg += ":- " + tmp + "\r\n";
		ss.str( "" );
		ss << RPL_ENDOFMOTD; //????
		msg += ss.str() + " " + sender->getUserName() + " ";
		msg += ":End of /MOTD command\r\n";
		std::map<int, Message>::iterator it = messages.find(sender->getUserFd());
		if (it != messages.end()) {
			it->second.setMessForSender(msg);
		}
		//send(sender->getUserFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL); // ок ли????
		// setReadyMessInMessageByFd(msg, sender->getUserFd());
	}
}

void	Messenger::printWelcome(User* sender, std::string str, int flag) {
	// флаг 0 - вывод сообщение из аргумента, flag 1 - username ; 2 - pass; 3 - nick

	if (flag == 0) {
		std::string msg = ":" + sender->getServName() + " :" + str + "\r\n";
		setReadyMessInMessageByFd(msg, sender->getUserFd());
		setRawMessInMessageByFd(msg, sender->getUserFd());
	}
	else if (flag == 1) {
		// std::string msg = ":" + sender->getServName + " :Recieved USERNAME " + sender->getUserName() + "\r\n";
		std::string msg = ":" + sender->getServName() + " User" + " Пользователь " + sender->getUserName() + \
		" aka (" + sender->getRealName() + ") ворвался в чат!" + "\r\n";
		dequeMaker(sender, TO_ALL);
		setReadyMessInMessageByFd(msg, sender->getUserFd());
		setRawMessInMessageByFd(msg, sender->getUserFd());

		sendMotd(sender);
	}
	else if (flag == 2) {
		std::string msg = ":" + sender->getServName() + " User" + " Recieved a password" + "\r\n";
		setReadyMessInMessageByFd(msg, sender->getUserFd());
		setRawMessInMessageByFd(msg, sender->getUserFd());
	}
	else if (flag == 3) {
		std::string msg = ":" + sender->getServName() + " User" + " Recieved NICK " + sender->getLogin() + "\r\n";
		setReadyMessInMessageByFd(msg, sender->getUserFd());
		setRawMessInMessageByFd(msg, sender->getUserFd());
	}
}

void Messenger::dequeMaker(User *user, int flag) {
	//формирует очередь получателей
	//flag 1 = for 1 user, 
	// flag 2 = to all except user in argument, 
	// flag 3 = to all, 
	// flag 4 = to channel except user in argument (NOT WORKS!!!!), 
	// flag 5 = to channel and user in argument (NOT WORKS!!!!)
	std::map<int, Message>::iterator it = messages.find(user->getUserFd());
	if (it->second.getDeque().size())
		it->second.getDeque().clear();
	std::vector<int> deque_users;
	
	if (flag == ONE_USER) {
		deque_users.push_back(user->getUserFd());
		it->second.setDeque(deque_users);
	}
	else if (flag == TO_ALL_BUT_NO_THIS_USER) {
		for (std::map<int, User>::iterator it_u = map_users.begin(); it_u != map_users.end(); it_u++) {
			if (it_u->first != user->getUserFd())
				deque_users.push_back(it_u->first);
			}
		it->second.setDeque(deque_users);
	}
	else if (flag == TO_ALL) {
		for (std::map<int, User>::iterator it_u = map_users.begin(); it_u != map_users.end(); it_u++)
				deque_users.push_back(it_u->first);
		it->second.setDeque(deque_users);
	}
	else if (flag == TO_CHANNEL_BUT_NO_THIS_USER) {
		// Написать реализацию, чтобы слало на канал, кроме самого юзера!!!

	}
	else if (flag == TO_CHANNEL) {
		// Написать реализацию, чтобы слало на канал!!!
	}
	else if (flag == SYSTEM_MSG) {
		it->second.setDeque(deque_users);
	}
	else if (flag == LIST_OF_RECIEVERS){

		int fd = 0;

		std::vector<std::string>::iterator it_vec = it->second.getListOfRecievers().begin();
		for(; it_vec != it->second.getListOfRecievers().end(); it_vec++){
			fd = getUserFdByLogin(*it_vec);
			if (fd != -1)
				deque_users.push_back(fd);
		}
		
		it->second.setDeque(deque_users);
		// списку получателей
	}
}

// NICK cmd
int	Messenger::nickCmd(const std::string &msg, User* sender) {
	if (sender->getPassword() == "") {
		printWelcome(sender, "Please enter your PASS before NICK-command", 0);
		setReadyMessInMessageByFd(msg, sender->getUserFd());
		setRawMessInMessageByFd(msg, sender->getUserFd());
		return 1;
	}

	std::map<int, Message>::iterator	it1; // !!!
	it1 = messages.find((*sender).getUserFd());
	std::vector<std::string> arr = stringSplit(msg, ' ');

	arr.erase(arr.begin()); 

std::cout << "<<<<<sender->getLogin()>>>>>" << sender->getLogin() <<"|"<< std::endl;
std::cout << "<<<<<sender->getPassword()>>>>>" << sender->getPassword() <<"|"<< std::endl;
std::cout << "<<<<<sender->getUserName()>>>>>" << sender->getUserName() <<"|"<< std::endl;

	// if (sender->getLogin() != "" && arr[0] == sender->getLogin())
	// 	return(replyError(sender, ERR_ERRONEUSNICKNAME, arr[0], ""));

	if (map_users.size() > 1) {
		std::map<int, User>::iterator it_u = map_users.begin();
		for (; it_u != map_users.end(); it_u++)
		{
			if (arr[0] == it_u->second.getLogin() && it_u->second.getUserFd() != sender->getUserFd())
				return(replyError(sender, ERR_NICKNAMEINUSE, arr[0], arr[0]));
			else if (arr[0] == it_u->second.getLogin() && it_u->second.getUserFd() == sender->getUserFd())
				return(1);
		}
	}
	(*sender).setLogin(arr[0]);
	printWelcome(sender, arr[0], 3);
	return (0);
}

// делает сообщение об ошибке
int		Messenger::replyError(User *user, int err, const std::string &str, const std::string &arg) {
	// std::string	msg = ":" + user.getServName() + " ";
	// std::cout << "1111повтор ника" << std::endl;
	std::string	msg = ":" + user->getServName() + " ";
	std::stringstream	ss;
	ss << err;
	msg += ss.str() + " " + user->getLogin();
	// std::cout << "msg = " << msg << std::endl;
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
	case ERR_UNKNOWNCOMMAND:
		msg += " " + str + " :Unknown command\n";
		break;
	case ERR_NOMOTD:
		msg += " :MOTD File is missing\n";
		break;
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
	// 	msg += ss.str() + " " + user->getLogin();
	// 	msg += " " + str + " :Erroneus nickname\n";
	// 	// msg = ":" + user->getServName() + " 432" + " default " +  user->getLogin() + "\r\r\r\r\r\r :Erroneus nickname\n";
	// 	// std::cout << ">> >>>>>> msg = " << msg << std::endl;

	// 	break;
	case ERR_NICKNAMEINUSE:
		msg += ss.str() + " " + str;
		msg += " " + str + " :Nickname is already in use\n";
		break;
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
	dequeMaker(user, ONE_USER); //мб излишне. на всякий пожарный
	setReadyMessInMessageByFd(msg, user->getUserFd());
	setRawMessInMessageByFd(msg, user->getUserFd());
	// send(user->getUserFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	std::cout << ">> msg = " << msg << std::endl;
	return (-1);
}


void Messenger::deleteBot(int senderFd) {
	std::map<int, Bot>::iterator it = map_robots.find(senderFd);
	if (it != map_robots.end()) {
		map_robots.erase(it);
	}
}

// PASS cmd
int	Messenger::passCmd(const std::string &msg, User* sender) {
	std::string currentPass = sender->getPassword();
	std::vector<std::string> arr = stringSplit(msg, ' ');
	if (arr.size() != 2)
		return(replyError(sender, ERR_NEEDMOREPARAMS, "", ""));
	arr.erase(arr.begin()); 
	if (currentPass != "" && arr[0] == currentPass)
		return(replyError(sender, ERR_ALREADYREGISTRED, "", ""));
	(*sender).setPassword(arr[0]);
	printWelcome(sender, arr[0], 2);
	return (0);
}

int Messenger::getUserFdByLogin(std::string login){

	std::map<int, User>::iterator it = map_users.begin();
	for (; it != map_users.end(); it++){
		if (login.compare(it->second.getLogin()) == 0)
			return it->first;	
	}
	return -1;
}

