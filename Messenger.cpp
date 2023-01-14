#include "Library.hpp"

Messenger::Messenger() {
	ChannelsStorage channels();
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
	// std::cout << "recivers_str |" << recievers_str << "|" << std::endl;

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
		// std::cout << "recivers are not identifire" << std::endl;
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
	if (it == messages.end() || it_user == map_users.end()) {
		write(2, "Iterator ERROR!\n", 17);
		exit(1);
	}
	dequeMaker(&it_user->second, TO_ALL_BUT_NO_THIS_USER);
	if (it_user->second.getPassword() == "" && str.find("PASS", 0) == std::string::npos 
		&& str.find("CAP LS", 0) == std::string::npos && str.find("PING", 0) == std::string::npos
		&& str.find("CAP END", 0) == std::string::npos) {
		// чтобы без проля нельзя было логиниться
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setCmd("");
		stringOutputMaker(&it_user->second, 464, "Password is nessesary. Please enter your password first", "");
		return ;
	}

	if (str.find("PASS", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		// it->second.setCmd("PASS");
		if (passCmd(it->second.getRawMessage(), &it_user->second) == 0)
			it->second.setCmd("PASS");
	}
	else if (str.find("NICK", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		if (nickCmd(it->second.getRawMessage(), &it_user->second) == 0)
			it->second.setCmd("NICK");
		// 	it->second.setCmd("NICK");
		// else 
		// 	it->second.setCmd("");
		// std::string mss = "";
		// mss.append("Hello!\n");
		//mss.append("@127.0.0.1 ");
		//mss.append(vector_string[0] +  ":Rita\n");
		// it->second.setReadyMess(mss);
		// std::cout << "'" << mss << "' " << std::endl;
	}
	else if (str.find("USER", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		userCmd(it->second.getRawMessage(), &it_user->second);
		// 	it->second.setCmd("USER");
		// else 
		// 	it->second.setCmd("");
		// std::string mss = "";
		// mss.append(":IRC-kitty 001 Dlana :Welcome to the IRC-kitty IRC Network Dlana!Dlana@127.0.0.1\n");;
		// it->second.setReadyMess(mss);
		// std::cout << "'" << mss << "' " << std::endl;
	}
	else if (str.find("QUIT", 0) != std::string::npos) {
		it->second.setCmd("QUIT");
		std::cout << "cmd QUIT" << std::endl;
	}
	else if (str.find("WHOAMI", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		whoAmICmd(&it_user->second);
	}
	else if (str.find("PRIVMSG", 0) != std::string::npos){
		it->second.setCmd("PRIVMSG");
		//parserPrivmsg(it->second);
		//dequeMaker(&it_user->second, LIST_OF_RECIEVERS);
		std::vector<std::string> vec_msg = splitString(str, ' ');
		if(vec_msg.size() >= 2 && channels.getChannelByName(vec_msg[1]).name == vec_msg[1]) {
			it->second.setDeque(channels.getDequeByChannel(vec_msg[1], &it_user->second));
		}
		else {
			if(vec_msg.size() >= 2 && getUserFdByLogin(vec_msg[1]) != -1) {
				dequeMaker(getUser(getUserFdByLogin(vec_msg[1])), ONE_USER);
			}
			else {
				it->second.setReadyMess(":ERROR!\n");
			}
		}
		std::cout << "cmd PRIVMSG" << std::endl;
		if(vec_msg.size() >= 3) {
			std::string tmp = "";
			tmp.append(vec_msg[1] + "\n");
			it->second.setReadyMess(tmp);
		}
	}
	else if (str.find("NOTICE", 0) != std::string::npos){
		it->second.setCmd("NOTICE");
		std::cout << "cmd NOTICE" << std::endl;
	}
	else if (str.find("JOIN", 0) != std::string::npos) {
		it->second.setCmd("JOIN");
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setMessForSender(channels.joinToCannel(str, &it_user->second));
		std::cout << "cmd JOIN" << std::endl;
	}
	else if (str.find("KICK", 0) != std::string::npos) {
		it->second.setCmd("KICK");
		it->second.setMessForSender(channels.kickUser(str, &it_user->second));
		std::cout << "cmd KICK" << std::endl;
	}
	else if (str.find("CAP LS", 0) != std::string::npos) {
		it->second.setCmd("CAP LS");
		// std::cout << "cmd CAP LS" << std::endl;

		dequeMaker(&it_user->second, ONE_USER);
		it->second.setReadyMess(":IRC-kitty CAP * LS :=PASS NICK USER PRIVMSG NOTICE JOIN KICK QUIT\n");

	}
	// else if (str.find("PING", 0) != std::string::npos) {
	// 	it->second.setCmd("CAP LS");
	// 	std::cout << "cmd CAP LS" << std::endl;
	// 	dequeMaker(&it_user->second, ONE_USER);
	// 	it->second.setRawMessage(":IRC-kitty 451  :You have not registered\n");
	// }
	else if (str.find("BOT", 0) != std::string::npos || it_user->second.getBotDialog() == YES) {
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setCmd("BOT");
		it_user->second.setBotDialog(YES);
		it->second.setMessForSender(initBot(userFd, it->second.getRawMessage()));
		if (it_user->second.getBotDialog() == FINISH) {
			std::cout << "deleteBot" << std::endl;
			deleteBot(userFd);
		}
	}
	// else if (it_user->second.getRegistFlag() == 1) {
	// 	std::cout << "ERROR 288 Messenger!" << std::endl;
	// 	std::stringstream	ss;
	// 	ss << ERR_UNKNOWNCOMMAND;
	// 	std::string	msg = ":" + it_user->second.getServName() + " " + ss.str();
	// 	std::vector<std::string> vec = splitString2(str, ' ');
	// 	// std::string tmp = splitString2(str, ' ');
	// 	msg += " " + it_user->second.getUserName() + " " + " :Unknown command\n";
	// 	//it->second.setMessForSender("421 :Unknown command\n");
	// }
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
	if (sender->getUserName() != "")
		// return(replyError(sender, ERR_ALREADYREGISTRED, "", ""));
		return(stringOutputMaker(sender, ERR_ALREADYREGISTRED, "You may not reregister", "USER"));

	std::vector<std::string> arr = stringSplitForUserCmd(msg, ' ');
	if (arr.size() < 5) {
		// return(replyError(sender, ERR_NEEDMOREPARAMS, tostring(arr), ""));
		return(stringOutputMaker(sender, ERR_NEEDMOREPARAMS, "Not enough parameters", "USER"));
	}
	arr.erase(arr.begin()); // удаляем из команды элемент USER
	if (sender->getUserName() != "" && arr[0] == sender->getUserName()) {
		// return(replyError(sender, ERR_ALREADYREGISTRED, "", ""));
		return(stringOutputMaker(sender, ERR_ALREADYREGISTRED, "You may not reregister", ""));
	}
	(*sender).setUserName(arr[0]);// arr[0] тк слово USER уже удалили
	if (sender->getRegistFlag() == 0 && sender->getLogin() != "") {
		sender->setRegistFlag(true);
		sendMotd(sender);
	}
	if (arr[arr.size() - 1][0] == ':')
		(*sender).setRealName(arr[arr.size() - 1].substr(1));
	else
		(*sender).setRealName(arr[arr.size() - 1]);
	return 0;
}

std::vector<std::string> Messenger::stringSplitForUserCmd(const std::string &line, char delimiter) {
	// size_t posRN = line.find("\r\n");
		size_t posRN = line.find("\r");
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

std::string Messenger::tostring2(std::vector<std::string> &v)
{
    std::stringstream os;
	for (std::vector<std::string>::iterator iter = v.begin(); iter < v.end(); iter++) {
		os << *iter;
	}
    return os.str();
}

void	Messenger::sendMotd(User* sender) {
	std::vector<std::string> vec = sender->getMotdFromServer();
	std::string tmp = tostring2(vec);
	if (tmp.empty()) {
		// replyError(sender, ERR_NOMOTD, "", "");
		stringOutputMaker(sender, ERR_NOMOTD, "MOTD File is missing", ""); // по идее здесь не дб логина в строке, но сейчас она печатается
		return;
	}

	//--- отправка кода 001
	std::string	msg = ":" + sender->getServName() + " ";
	std::stringstream	ss;
	ss << RLP_REGIST_OK;
	msg += "00" + ss.str() + " " + sender->getLogin() + " ";
	ss.str( "" );
	msg += ":Welcome to IRC, ";
	msg += sender->getLogin() + "!" + sender->getUserName() + "@" + HOST + "\n";
	// -----начало мотд
	msg += ":" + sender->getServName() + " ";
	ss << RPL_MOTDSTART;
	msg += ss.str() + " " + sender->getLogin() + " :- " + sender->getServName() + " Message of the day - \n";
	ss.str( "" );
	// -----сам мотд
	msg += ":" + sender->getServName() + " ";
	ss << RPL_MOTD; //????
	msg += ss.str() + " " + sender->getLogin() + " :- " + tmp + "\n";
	ss.str( "" );
	// -----конец мотд
	msg += ":" + sender->getServName() + " ";
	ss << RPL_ENDOFMOTD; //????
	msg += ss.str() + " " + sender->getLogin() + " :End of /MOTD command" + "\n";
	// return msg;
	// setReadyMessInMessageByFd(msg, sender->getUserFd());
	std::map<int, Message>::iterator it = messages.find(sender->getUserFd());
	it->second.setMessForSender(msg);
}

void Messenger::dequeMaker(User *user, int flag) {
	//формирует очередь получателей
	//flag 1 = for 1 user, 
	// flag 2 = to all except user in argument, 
	// flag 3 = to all, 
	// flag 4 = to channel except user in argument (NOT WORKS!!!!), 
	// flag 5 = to channel and user in argument (NOT WORKS!!!!)
	// flag 6 = ANOTHER_ONE_USER 6 - for privmsg
	std::map<int, Message>::iterator it = messages.find(user->getUserFd());
	std::map<int, User>::iterator it_u = map_users.begin();
	if (it->second.getDeque().size())
		it->second.getDeque().clear();
	std::vector<int> deque_users;
	
	if (flag == ONE_USER) {
		deque_users.push_back(user->getUserFd());
	}
	else if (flag == TO_ALL_BUT_NO_THIS_USER) {
		for (; it_u != map_users.end(); it_u++) {
			if (it_u->first != user->getUserFd() && it_u->second.getRegistFlag() == true)
				deque_users.push_back(it_u->first);
			}
		// it->second.setDeque(deque_users);
	}
	else if (flag == TO_ALL) {
		for (; it_u != map_users.end(); it_u++) {
			if (it_u->second.getRegistFlag() == true)
				deque_users.push_back(it_u->first);
		}
		// it->second.setDeque(deque_users);
	}
	else if (flag == TO_CHANNEL_BUT_NO_THIS_USER) {
		// Написать реализацию, чтобы слало на канал, кроме самого юзера!!!
		deque_users = channels.getDequeByChannel("#kvirc", user);
	}
	else if (flag == TO_CHANNEL) {
		deque_users = channels.getDequeByChannel("#kvirc", user); //заменить msg!!!!!!!!!!!!!
		// Написать реализацию, чтобы слало на канал!!!
	}
	else if (flag == ANOTHER_ONE_USER) {
		if (it_u->second.getRegistFlag() == true)
				deque_users.push_back(it_u->first);
		// Написать реализацию, чтобы слало на канал!!!
	}
	else if (flag == SYSTEM_MSG) {
		// it->second.setDeque(deque_users);
	}
	else if (flag == LIST_OF_RECIEVERS){

		int fd = 0;

		std::vector<std::string>::iterator it_vec = it->second.getListOfRecievers().begin();
		for(; it_vec != it->second.getListOfRecievers().end(); it_vec++){
			fd = getUserFdByLogin(*it_vec);
			if (fd != -1)
				deque_users.push_back(fd);
		}
		
		// it->second.setDeque(deque_users);
		// списку получателей
	}
	it->second.setDeque(deque_users);
}

// whoAmICmd cmd
int	Messenger::whoAmICmd(User* sender) {
	std::stringstream	ss;
	ss << sender->getUserFd();
	std::string tmp = "Your fd is " + ss.str() + "\n";
	if (sender->getLogin().length() > 0)
		tmp += "Your Nick is " + sender->getLogin() + "\n";
	else
		tmp += "No Nick\n";

	if (sender->getUserName().length() > 0)
		tmp += "Your UserName is " + sender->getUserName() + "\n";
	else
		tmp += "No UserName\n";

	tmp += "Your Password is " + sender->getPassword() + "\n"; // убрать!!!!
	if (sender->getRegistFlag() == true) {
		tmp += "Your current Channel is " + sender->getChannelHere() + "\n";
	}
	else
		tmp += "Watch out! You're NOT registred yet\n";
	setReadyMessInMessageByFd(tmp, sender->getUserFd());
	return 0;
}

// NICK cmd
int	Messenger::nickCmd(const std::string &msg, User* sender) {
	std::vector<std::string> arr = splitString2(msg, ' ');
	if (arr.size() == 1 || *(arr[1].begin()) == '\n' || *(arr[1].begin()) == '\r')
		return(stringOutputMaker(sender, ERR_NONICKNAMEGIVEN, "No nickname given", "") + 1);
	arr.erase(arr.begin()); 

	if (map_users.size() > 1) { // включается ли сюда сам юзер если он еще не зареган? это влияет на эту проверку
		for (std::map<int, User>::iterator it_u = map_users.begin(); it_u != map_users.end(); it_u++)
		{
			if (arr[0] == it_u->second.getLogin() && it_u->second.getUserFd() != sender->getUserFd())
				return(stringOutputMaker(sender, ERR_NICKNAMEINUSE, "Nickname is already in use", "") + 1); // не тестила на реальном серваке!
		}
	}
		if (arr[0] == sender->getLogin())
				// return(1);
				return(stringOutputMaker(sender, 0, "You entered your current nick", "") + 1);
		else if (sender->getUserName() == "" && sender->getLogin() != "") {
				// return(1);
				stringOutputMaker(sender, 0, "User changed nick", "NICK");	
		}
		else if (sender->getUserName() != "") {
			std::string	msg = ":" + sender->getLogin() + "!" + sender->getUserName() + "@" + HOST + " NICK :" + arr[0];
			setReadyMessInMessageByFd(msg, sender->getUserFd()); // всем ли давать рассылку о смене ника? влияет ли то, были ли он ранее до конца зареган?
			if (sender->getRegistFlag() == 0 && sender->getUserName() != "") {
				sender->setRegistFlag(true);
				sendMotd(sender);
			}
		}
	(*sender).setLogin(arr[0]);
	return (0);
}

int Messenger::stringOutputMaker(User *user, int err, const std::string &description, const std::string &command) {
	// добавляет логин, код ошибки и имя сервера, кладет в rawMessage
	std::string	msg = ":" + user->getServName() + " ";
	std::stringstream	ss;
	if (err != 0)
	{ 
		ss << err;
		msg += ss.str() + " ";
	}
	msg += user->getLogin();
	if (command != "")
		msg += " "  + command;
	if (description != "")
		msg += " :"  + description + "\n";
	setReadyMessInMessageByFd(msg, user->getUserFd());
	return 0;
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
	std::vector<std::string> arr = splitString2(msg, ' '); //splitString2 не работает =(
	if (arr.size() == 1 || *(arr[1].begin()) == '\n' || *(arr[1].begin()) == '\r')
		// return(replyError(sender, ERR_NEEDMOREPARAMS, "PASS", ""));
		return(stringOutputMaker(sender, ERR_NEEDMOREPARAMS, "Not enough parameters", "PASS") + 1);
	arr.erase(arr.begin());
	if (arr[0].find(":") == 0)
		arr[0] = arr[0].substr(1);
	if (currentPass != "" && arr[0] == currentPass)
		// return(replyError(sender, ERR_ALREADYREGISTRED, "PASS", ""));
		return(stringOutputMaker(sender, ERR_ALREADYREGISTRED, "You may not reregister", "") + 1);

	(*sender).setPassword(arr[0]);
	return (0);
}

int Messenger::getUserFdByLogin(std::string login) {
	for (std::map<int, User>::iterator it = map_users.begin(); it != map_users.end(); it++) {
		if (login.compare(it->second.getLogin()) == 0)
			return it->first;	
	}
	return -1;
}

User *Messenger::getUser(int fd) {
	User *tmp = new User();
	std::map<int, User>::iterator it = map_users.find(fd);
	if (it != map_users.end()) {
		return(&it->second);
	}
	return(tmp);
}

int Messenger::deleteUser(int fd) {
	std::map<int, User>::iterator it = map_users.find(fd);
	if (it != map_users.end()) {
		map_users.erase(fd);
		return 0;
	}
	return -1;
}

ChannelsStorage	Messenger::getChannels(){
	return channels;
}

