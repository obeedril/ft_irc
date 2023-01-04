# include "Library.hpp"

Core::Core(int port_) {

	storage_messages = new Messenger();
	listen_sock = 0;
	this->port = port_;
	max = 0;
	count_cli = 0;
	bzero(&id, sizeof(id));
	FD_ZERO(&active_);

	listen_sock = socket(AF_INET, SOCK_STREAM, 0); 
	if (listen_sock < 0) {
		error(-1);
	}

	max = listen_sock;
	FD_SET(listen_sock, &active_);

	const int trueFlag = 1;
	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0) {
		std::cout << "setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	len_adr = sizeof(servaddr);
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = (1 << 24) | 127;
	servaddr.sin_port = (port >> 8) | (port << 8);

	if ((bind(listen_sock, (const struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) { 
		error(-1); 
	} 
	if (listen(listen_sock, 128) < 0) {
		error(-1); 
	}
};

Core::~Core() {
};

void	Core::run() {
	read_ = write_ =  active_;
	if (select(max + 1, &read_, &write_, NULL, NULL) < 0) { 
		return ;
	} 
	for (int s = 0; s <= max; ++s) {
		if(FD_ISSET(s, &read_) && s == listen_sock) {
			int res = createNewSocket();
			if (res < 0) {
				return ;
			}
			break ;
		}
		if (FD_ISSET(s, &read_) && s != listen_sock) {
			readFromUser(s);
			std::map<int, User>::iterator it1 = map_users.find(s);
			if (it1 == map_users.end()) {
				write(2, "Users not found\n", 26);
				return ; ////!!!!
			}
			if (length_message <= 0) {
				sprintf(bufWrite, "IRC : client %s just left\n", it1->second.getUserName().c_str());
				writeToUser(s);
				FD_CLR(s, &active_);
				close(s);
				break ;
			}
			else {
				writeToUser(s);
				// for (int i = 0, j = 0; i < length_message; ++i, ++j) {
					// str[j] = bufRead[i];
					// if (str[j] == '\n') {
					// 	str[j] = '\0';
					// 	check_message(s);
					// 	writeToUser(s);
					// 	j = -1;
					// }
				// }
			}
		}
	}
};

int		Core::createNewSocket() {
	int user_fd = accept(listen_sock, (struct sockaddr *)&servaddr, &len_adr);
	if (user_fd < 0) {
		return (-1);
	}
	if (max < user_fd) {
		max = user_fd;
	}

	User new_user;
	Message new_message;
	new_user.setUserFd(user_fd);
	new_user.setUserName("Default_name");
	// new_user.setCommand(NO_COMMAND);
	map_users.insert(std::pair<int, User> (user_fd, new_user));

	//new_user.count_cli = count_cli;
	id[user_fd] = count_cli;
	count_cli++;
	FD_SET(user_fd, &active_);

	// std::map<int, User>::iterator it1 = map_users.find(user_fd);
	char tmp[4048];
	std::string str;
	sprintf(tmp, "IRC : client %d just arrived\n", user_fd); // new_user.getUserFd()
	new_message.setRawMessage(str.append(tmp));
	// std::cout << "str " << str << std::endl;
	storage_messages->insertMessage(user_fd, new_message); // new_user.getUserFd()
	// std::cout << "new mess " << new_message.getRestMess() << std::endl;
	// std::cout << "storage_messages->getMessageByFd(new_user.getUserFd()) " << storage_messages->getMessageByFd(new_user.getUserFd()) << std::endl;
	// it1->second.setMessage(static_cast<std::string>(tmp));

	writeToUser(user_fd);
	return (0);
};

void	Core::error(int err_type) {
	std::cerr << err_type;
	write(2, "Fatal error\n", 12);
	exit(1);
};

int		Core::writeToUser(int current_fd) {
	std::map<int, User>::iterator it1 = map_users.find(current_fd);

	if (it1 == map_users.end()) {
		write(2, "Users not found\n", 26);
		return (0) ; ////!!!!
	} 
	else  {
		// std::map<int, Message>::iterator it = storage_messages->getMessages().find(current_fd);
		std::string str = storage_messages->getRawMessageByFd(current_fd);
		std::cout << "write: " << current_fd << std::endl;
		if (it1->second.getBotDialog() != NO) {
			send(current_fd, str.c_str(), str.length(), 0);
			storage_messages->deleteMessage(current_fd);
			if (it1->second.getBotDialog() == FINISH) {
				it1->second.setBotDialog(NO);
				storage_messages->deleteBot(current_fd);
			}
			return (0);
		}
		for(int s = 0; s <= max; ++s) {
			if (FD_ISSET(s, &write_) && s != current_fd) {
				//  std::cout << "str " << str << std::endl;
				// std::cout << "it1->second.restMess " << it->second.getRestMess() << std::endl;
				// send(s, it1->second.getMessage().c_str(), it1->second.getMessage().length(), 0);
				send(s, str.c_str(), str.length(), 0);
				storage_messages->deleteMessage(current_fd);
			}
		}
	}
	it1->second.setMessage("");
	return (0);
};

int		Core::readFromUser(int user_fd) {

	Message new_message;
	// storage_messages->insertMessage();
	std::string str;
	std::string cmd;
	//std::map<int, User>::iterator it1 = map_users.find(user_fd);
	length_message = 0;
	char tmp[4048];
	length_message = recv(user_fd, tmp, 42*4096, 0);
	str.append(tmp);
	str = str.substr(0, str.find("\r\n", 0)+1);
	if (length_message > 0){
		// parser_message(user_fd, tmp);

		new_message.setRawMessage(str);
		storage_messages->insertMessage(user_fd, new_message);
		storage_messages->parsRecvStr(str, &(map_users.find(user_fd))->second);
	
		//new_message.setRawMessage(str.append(tmp));
		//new_message.setRawMessage(str.append("\r\n"));
		//std::cout << "RECV  fd|" << user_fd << "|" << new_message.getRawMessage() << "|" << std::endl;
		//storage_messages->insertMessage(user_fd, new_message);
		//storage_messages->parseBuffer(user_fd);
	}

	return (length_message);
};

// void Core::setStorage_messages(Messenger &_storage_messeges){
// 	storage_messages = _storage_messeges;
// };
Messenger* Core::getStorage_messages(){
	return storage_messages;
};


std::vector<std::string> stringSplit(const std::string &line, char delimiter) {
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

std::string Core::tostring(std::vector<std::string> &v)
{
    std::stringstream os;
	for (std::vector<std::string>::iterator iter = v.begin(); iter < v.end(); iter++) {
		os << *iter;
	}
    return os.str();
}

int	Core::userCmd(const std::string &msg, int user_fd) {
	std::cout << "it1->second.getMessage() = " << msg << std::endl;
	std::map<int, User>::iterator	it1; // !!!
	it1 = map_users.find(user_fd);// !!!

	std::vector<std::string> arr = stringSplit(msg, ' ');
	arr.erase(arr.begin()); // удаляем из команды элемент USER
	std::cout << "arr[0]полсе делита = " << arr[0] << std::endl;
	if (arr.size() < 4) {
		// std::string tmp(arr.begin(), arr.end());
		return(replyError(it1->second, ERR_NEEDMOREPARAMS, tostring(arr), NULL));
	}
	for (std::map<int, User>::iterator it2 = map_users.begin(); it2 != map_users.end(); it2++)
		{
			if (arr[0] == it2->second.getUserName()) { // arr[0] тк слово USER уже удалили
				std::cout << "повтор ника" << std::endl;
				return(replyError(it2->second, ERR_ALREADYREGISTRED, NULL, NULL));
			}
		}
	it1->second.setUserName(arr[0]);// arr[0] тк слово USER уже удалили
	std::cout << "user_fd = " << user_fd << std::endl;
	std::cout << "UserName = " << it1->second.getUserName() << std::endl;

	if (arr[arr.size() - 1][0] == ':')
		it1->second.setRealName(arr[arr.size() - 1].substr(1));
	else
		it1->second.setRealName(arr[arr.size() - 1]);
	std::cout << "RealName = " << it1->second.getRealName() << std::endl;
	return 0;
}

	void Core::setServ(Server *newServ) {
		_irc_serv = newServ;
	}

	std::string Core::getServName() {
		return _irc_serv->getServName();
	}

int		Core::replyError(User &user, int err, const std::string &str, const std::string &arg) {
	// std::string	msg = ":" + user.getServName() + " ";
	std::string	msg = ":" + getServName() + " ";
	std::stringstream	ss;
	ss << err;
	msg += ss.str() + " " + user.getLogin();

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
	send(user.getUserFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	return (-1);
}
