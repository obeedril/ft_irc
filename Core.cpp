# include "Library.hpp"

Core::Core(int port_) {
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
	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
	{
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
				//sprintf(bufWrite, "IRC : client %s just left\n", it1->second.getUserName().c_str());
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
	new_user.setUserFd(user_fd);
	new_user.setUserName("Default_name");
	new_user.setCommand(NO_COMMAND);
	new_user.setBotDialog(false);
	map_users.insert(std::pair<int, User> (user_fd, new_user));

	//new_user.count_cli = count_cli;
	id[user_fd] = count_cli;
	count_cli++;
	FD_SET(user_fd, &active_);
	std::map<int, User>::iterator it1 = map_users.find(user_fd);
	char tmp[4048];
	sprintf(tmp, "IRC : client %d just arrived\n", new_user.getUserFd());
	it1->second.setMessage(static_cast<std::string>(tmp));
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
	std::cout << "write: " << current_fd << std::endl;
	std::cout << "it1->second.getBotDialog(): " << it1->second.getBotDialog() << std::endl;
	if (it1->second.getBotDialog() == true) {
		std::cout << "send from BOT: " << it1->second.getMessage().c_str() << std::endl;
		send(current_fd, it1->second.getMessage().c_str(), it1->second.getMessage().length(), 0);
	}
	else {
		for(int s = 0; s <= max; ++s) {
			if (FD_ISSET(s, &write_) && s != current_fd) {
				send(s, it1->second.getMessage().c_str(), it1->second.getMessage().length(), 0);
			}
		}
	}
	it1->second.setMessage("");
	return (0);
};

int		Core::readFromUser(int user_fd) {
	length_message = 0;
	char tmp[4048];
	length_message = recv(user_fd, tmp, 42*4096, 0);
	if (length_message > 0)
		parser_message(user_fd, tmp); 
	return (length_message);
};	

void Core::parser_message(int user_fd, char *bufRead) {
	char							tmp[4048];
	std::string						tmpstr(bufRead);
	size_t							pos;
	std::map<int, User>::iterator	it1;
	char							*istr;

	pos = tmpstr.find("\r\n");
	if (pos != std::string::npos) {
		tmpstr = tmpstr.substr(0, pos);
		std::cout << " CUT: " << tmpstr  << std::endl;
	}

	it1 = map_users.find(user_fd);

	istr = strstr(bufRead, "USER");
	if (istr != NULL)
		it1->second.setCommand(USER);
	else 
		it1->second.setCommand(NO_COMMAND);
	//----------- BOT ---------
	istr = strstr(bufRead, "BOT");
	if (istr != NULL || it1->second.getBotDialog() == true)
		it1->second.setCommand(BOT);
	else 
		it1->second.setCommand(NO_COMMAND);
	//----------- BOT ---------
	it1->second.setMessage(tmpstr);
	switch (it1->second.getCommand())
		{
		case BOT: //----------- BOT ---------
			initBot(&it1->second, tmpstr);
			break;
			//----------- BOT ---------
		case USER:
			//
			sprintf(tmp, "%d %s\n", 311, " USER guest tolmoon tolsun :Hard Code");
			it1->second.setMessage(static_cast<std::string>(tmp));
			break;
		case NICK:
			//
			break;
		case PASS:
			//
			break;
		case PRIVMSG:
			//
			break;
		case NOTICE:
			//
			break;
		case JOIN:
			//
			break;
		case KICK:
			//
			break;
		default:
			std::cout << "default Message: " << it1->second.getMessage().c_str() << std::endl;
			sprintf(tmp, "%s %d: %s\n", it1->second.getUserName().c_str(),
			it1->second.getUserFd(), it1->second.getMessage().c_str());
			std::cout << "new Message: " << tmp << std::endl;
			std::string tmpstr(tmp);
			it1->second.setMessage(tmpstr);
			break;
		}
};

void Core::initBot(User *my_client, std::string msg) {
	std::map<int, Bot>::iterator it1 = map_robots.find(my_client->getUserFd());
	if (it1 == map_robots.end()) {
		std::cout << "new BOT: " << msg << std::endl;
		my_client->setBotDialog(true);
		Bot new_bot(my_client);
		map_robots.insert(std::pair<int, Bot> (my_client->getUserFd(), new_bot));
		new_bot.callBot(msg);
	}
	else {
		std::cout << "old BOT: " << msg << std::endl;
		it1->second.callBot(msg);
	}
}

