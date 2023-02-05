#include "Library.hpp"

Messenger::Messenger(std::string server) : channels(server) {
	serverName = server;
	//ChannelsStorage channels(server);
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

std::string Messenger::getServerName() {
	return serverName;
}

std::map<int, User> Messenger::getMapUsers() {
	return map_users;
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


void Messenger::parserPrivmsg(Message &mess, User &user){
	std::string tmp = "";
	std::string receivers_str = "";
	
	size_t len = mess.getRawMessage().length();
	tmp = mess.getRawMessage().substr(mess.getCmd().length() + 1, len);

	size_t pos = tmp.find(":", 0); //
	if (pos == std::string::npos){
		mess.setReceiver("");
		mess.setReadyMess("");
	} else {
		receivers_str = tmp.substr(0, pos);
		mess.setReceiver(strTrimBegin(receivers_str, ' '));
		mess.setReadyMess(tmp.substr(pos + 1, len) + "\n");
	}
	tmp = "";
	
	if(mess.getReadyMess() != "" && mess.getReceiver() != "") {
		tmp.append(":" + user.getLogin() + "!" + user.getLogin() + "@127.0.0.1 ");
		tmp.append(mess.getCmd() + " " + mess.getReceiver() + " :" + mess.getReadyMess() + "\n");
		mess.setReadyMess(tmp);
	} else {
		mess.setReadyMess("");
		tmp.append(":" + serverName + " " + toString(ERR_NEEDMOREPARAMS) + " " + mess.getCmd() + " :Not enough parameters\n");
		mess.setMessForSender(tmp);
	}
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

bool  Messenger::checkLagcheck(std::string msg) {
	write(1, "START------------\n", 14);
	if (msg.find("LAGCHECK") != std::string::npos) {
		write(1, "------------\n", 14);
		return(true);
	}
	return(false);
}

void Messenger::parsRecvStr(std::string str, int userFd) {
	std::map<int, Message>::iterator it = messages.find(userFd);
	std::map<int, User>::iterator it_user = map_users.find(userFd);
	std::string strCmd = "";
	std::string uppStr = "";

	if (str.find("CAP") == std::string::npos){
		size_t pos = str.find(" ", 0); //
		// if (pos != std::string::npos){
			strCmd = str.substr(0, pos);
		// }
		uppStr = toUpperCase(str, strCmd);
		str = str.substr(strCmd.length());

		uppStr.append(str);

	} else 
		uppStr = str;
    
	it->second.setRawMessage(uppStr);
	dequeMaker(&it_user->second, TO_ALL_BUT_NO_THIS_USER);
	if (it_user->second.getPassword() == "" && uppStr.find("PASS", 0) == std::string::npos 
		&& uppStr.find("CAP LS", 0) == std::string::npos && uppStr.find("PING", 0) == std::string::npos
		&& uppStr.find("CAP END", 0) == std::string::npos && uppStr.find("WHOAMI", 0) == std::string::npos
		&& uppStr.find("QUIT", 0) == std::string::npos) {
		// чтобы без проля нельзя было логиниться
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setCmd("");
		// it->second.setMessForSender(&it_user->second, 464, "Password is nessesary. Please enter your password first", "");
		stringOutputMaker(&it_user->second, ERR_PASSWDMISMATCH, "Password is nessesary. Please enter your password first", "");
		it->second.setMessForSender(getReadyMessByFd(it_user->second.getUserFd()));
		setReadyMessInMessageByFd("", it_user->second.getUserFd());
		return ;
	}
	if (uppStr.find("PASS", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		if (passCmd(it->second.getRawMessage(), &it_user->second) == 0)
			it->second.setCmd("PASS");
	}
	else if (uppStr.find("NICK", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		if (nickCmd(it->second.getRawMessage(), &it_user->second) == 0)
			it->second.setCmd("NICK");
	}
	else if (uppStr.find("USER", 0) != std::string::npos) {
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setCmd("USER"); //??????
		userCmd(it->second.getRawMessage(), &it_user->second);
	}
	else if (uppStr.find("QUIT", 0) != std::string::npos){
		dequeMaker(&it_user->second, TO_ALL_BUT_NO_THIS_USER);
		it->second.setCmd("QUIT");
		quitCmd(it->second.getRawMessage(), &it_user->second);
	}
	else if (uppStr.find("WHOAMI", 0) != std::string::npos){
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setCmd("WHOAMI");
		whoAmICmd(&it_user->second);
	}
	else if (uppStr.find("BOT") != std::string::npos || it_user->second.getBotDialog() == YES) {
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setCmd("BOT");
		it_user->second.setBotDialog(YES);
		it->second.setMessForSender(initBot(userFd, it->second.getRawMessage()));
		if (it_user->second.getBotDialog() == FINISH) {
			std::cout << "deleteBot" << std::endl;
			deleteBot(userFd);
		}
	}
	else if (uppStr.find("PRIVMSG", 0) != std::string::npos){
		it->second.setCmd("PRIVMSG");
		parserPrivmsg(it->second, it_user->second);
		dequeMaker(&it_user->second, LIST_OF_RECIEVERS);
	}
	else if (uppStr.find("NOTICE", 0) != std::string::npos){
		it->second.setCmd("NOTICE");
		parserPrivmsg(it->second, it_user->second);
		dequeMaker(&it_user->second, LIST_OF_RECIEVERS);
	}
	else if (uppStr.find("JOIN", 0) != std::string::npos) {
		it->second.setCmd("JOIN");
		std::cout << "cmd JOIN" << std::endl;
		it->second.setMessForSender(channels.joinToCannel(uppStr, &it_user->second, SYSTEM_MSG));
		it->second.setReadyMess(channels.joinToCannel(uppStr, &it_user->second, TO_CHANNEL));
		it->second.setChannel(channels.parserChannelInMsg(uppStr));
		dequeMaker(&it_user->second, TO_CHANNEL_BUT_NO_THIS_USER);
	}
	else if (uppStr.find("TOPIC", 0) != std::string::npos) {
		//<TOPIC #kvirc :GO>
		it->second.setCmd("TOPIC");
		std::cout << "cmd TOPIC" << std::endl;
		it->second.setReadyMess(channels.addTopicToCannel(uppStr, &it_user->second));
		it->second.setChannel(channels.parserChannelInMsg(uppStr));
		dequeMaker(&it_user->second, TO_CHANNEL);
		
	}
	else if (uppStr.find("KICK", 0) != std::string::npos) {
		it->second.setCmd("KICK");
		it->second.setChannel(channels.parserChannelInMsg(uppStr));
		dequeMaker(&it_user->second, TO_CHANNEL_BUT_NO_THIS_USER);
		it->second.setMessForSender(channels.kickUser(uppStr, &it_user->second, SYSTEM_MSG));
		if (it->second.getMessForSender().find("353") != std::string::npos) {
			it->second.setReadyMess(channels.kickUser(uppStr, &it_user->second, TO_CHANNEL));
		}
		std::cout << "cmd KICK" << std::endl;
	}
	else if (uppStr.find("PART", 0) != std::string::npos) {
		it->second.setCmd("PART");
		it->second.setChannel(channels.parserChannelInMsg(uppStr));
		dequeMaker(&it_user->second, TO_CHANNEL);
		it->second.setReadyMess(channels.partChannel(uppStr, &it_user->second));
		std::cout << "cmd PART" << std::endl;
	}
	else if (uppStr.find("CAP LS", 0) != std::string::npos) {
		it->second.setCmd("CAP LS");
		dequeMaker(&it_user->second, ONE_USER);
		it->second.setReadyMess(":" + serverName + " CAP * LS :=PASS NICK USER PRIVMSG NOTICE JOIN KICK QUIT\n");
	}
	else if (uppStr.find("PING", 0) != std::string::npos) {
		it->second.setCmd("PING");
		it->second.setMessForSender(":" + serverName + " PONG :@127.0.0.1\n");
	}
	else {
		return ;
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

void	Messenger::checkAdmin(User* sender) {

	if (sender->getLogin() == sender->getServ()->getAdminLogin() && sender->getPassword() == sender->getServ()->getArgPass())
			sender->setIsAdminServer(true);
	else 
		sender->setIsAdminServer(false);
	std::map<std::string, std::string>::const_iterator itStart= sender->getServ()->getOperatorsMap().begin();
	std::map<std::string, std::string>::const_iterator itEnd = sender->getServ()->getOperatorsMap().end();
	for (; itStart != itEnd; itStart++) {
		if (sender->getLogin() == itStart->first && sender->getPassword() == itStart->second) {
			sender->setIsOperatorServer(true);
			return ;
		}
	}
	sender->setIsOperatorServer(false);
}


void	Messenger::sendMotd(User* sender) {
	// checkAdmin(sender); // не работает!
	std::vector<std::string> vec = sender->getMotdFromServer();
	std::string tmp = tostring2(vec);
	if (tmp.empty()) {
		// replyError(sender, ERR_NOMOTD, "", "");
		stringOutputMaker(sender, ERR_NOMOTD, "MOTD File is missing", ""); // по идее здесь не дб логина в строке, но сейчас она печатается
		return;
	}

	//--- отправка кода 001
	std::string	msg = ":" + serverName + " ";
	std::stringstream	ss;
	ss << RLP_REGIST_OK;
	msg += "00" + ss.str() + " " + sender->getLogin() + " ";
	ss.str( "" );
	msg += ":Welcome to IRC, ";
	msg += sender->getLogin() + "!" + sender->getUserName() + "@" + HOST + "\n";
	// -----начало мотд
	msg += ":" + serverName + " ";
	ss << RPL_MOTDSTART;
	msg += ss.str() + " " + sender->getLogin() + " :- " + serverName + " Message of the day - \n";
	ss.str( "" );
	// -----сам мотд
	msg += ":" + serverName + " ";
	ss << RPL_MOTD; //????
	msg += ss.str() + " " + sender->getLogin() + " :- " + tmp + "\n";
	ss.str( "" );
	// -----конец мотд
	msg += ":" + serverName + " ";
	ss << RPL_ENDOFMOTD; //????
	msg += ss.str() + " " + sender->getLogin() + " :End of /MOTD command" + "\n";
	// return msg;
	// setReadyMessInMessageByFd(msg, sender->getUserFd());
	std::map<int, Message>::iterator it = messages.find(sender->getUserFd());
	it->second.setMessForSender(msg);
}

void Messenger::dequeMaker(User *user, int flag) {
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
	}
	else if (flag == TO_ALL) {
		for (; it_u != map_users.end(); it_u++) {
			if (it_u->second.getRegistFlag() == true)
				deque_users.push_back(it_u->first);
		}
	}
	else if (flag == TO_CHANNEL_BUT_NO_THIS_USER) {
		deque_users = channels.getDequeByChannel(it->second.getChannel(), user);
	}
	else if (flag == TO_CHANNEL) {
		deque_users = channels.getDequeByAllInChannel(it->second.getChannel(), user);
	}
	else if (flag == ANOTHER_ONE_USER) {
		if (it_u->second.getRegistFlag() == true)
				deque_users.push_back(it_u->first);
	}
	// else if (flag == SYSTEM_MSG) {
	// 	// it->second.setDeque(deque_users);
	// }
	else if (flag == LIST_OF_RECIEVERS) {
		std::string receiver = it->second.getReceiver();
		std::cout << "\x1b[1;94m" << "getReceiver: " << it->second.getReceiver() << "\x1b[0m" << std::endl;
        if(channels.getChannelByName(receiver)->name == receiver) {
			deque_users = channels.getDequeByChannel(receiver, user);
        }
        else {
            if(getUserFdByLogin(receiver) != -1) {
                deque_users.push_back(getUserFdByLogin(receiver));
            }
            else {
                deque_users.push_back(user->getUserFd());
            }
        }
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

	if (sender->getPassword().length() > 0)
		tmp += "Your Password is " + sender->getPassword() + "\n";
	else
		tmp += "No Password\n";

	tmp += "Am I a ServerAdmin?  ";
	tmp += sender->getIsAdminServer() ? "yes" : "no";
	tmp += "\n";
	tmp += "Am I a ServerOperator?  ";
	tmp += sender->getIsOperatorServer() ? "yes" : "no";
	tmp += "\n";

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
				checkAdmin(sender); // не работает!	
		}
		else if (sender->getUserName() != "") {
			std::string	msg = ":" + sender->getLogin() + "!" + sender->getUserName() + "@" + HOST + " NICK :" + arr[0] + "\n";
			setReadyMessInMessageByFd(msg, sender->getUserFd()); // всем ли давать рассылку о смене ника? влияет ли то, были ли он ранее до конца зареган?
			if (sender->getRegistFlag() == 0 && sender->getUserName() != "") {
				sender->setRegistFlag(true);
				sendMotd(sender);
			}
			// checkAdmin(sender); // не работает!
		}
	(*sender).setLogin(arr[0]);
	checkAdmin(sender); // не работает!	
	return (0);
}

int Messenger::stringOutputMaker(User *user, int err, const std::string &description, const std::string &command) {
	// добавляет логин, код ошибки и имя сервера, кладет в rawMessage
	std::string	msg = ":" + serverName + " ";
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
		msg += " :"  + description;
	msg += "\n";
	setReadyMessInMessageByFd(msg, user->getUserFd());
	return 0;
}

void Messenger::deleteBot(int senderFd) {
	std::map<int, Bot>::iterator it = map_robots.find(senderFd);
	if (it != map_robots.end()) {
		map_robots.erase(it);
	}
}

// QUIT cmd
int	Messenger::quitCmd(const std::string &msg, User* sender) {
	size_t posStart = msg.find(" ");
	size_t posEnd = msg.find("\n");
	std::string tmp;
	if (posStart != std::string::npos) {
		if (posEnd != std::string::npos)
			tmp = msg.substr(posStart + 1, posEnd - posStart);
		else 
			tmp = msg.substr(posStart + 1);
	}
	else {
		tmp = "User " + sender->getLogin() + " left the chat";
	}
	if (sender->getRegistFlag() == false)
		stringOutputMaker(sender, RPL_CLOSING, tmp, "QUIT");
	return 0;
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
	checkAdmin(sender);
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

ChannelsStorage	*Messenger::getChannels(){
	return &channels;
}

