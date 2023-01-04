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

	User new_user(_irc_serv);
	Message new_message;
	new_user.setUserFd(user_fd);
	new_user.setUserName("");
	// new_user.setCommand(NO_COMMAND);
	map_users.insert(std::pair<int, User> (user_fd, new_user));

	//new_user.count_cli = count_cli;
	id[user_fd] = count_cli;
	count_cli++;
	FD_SET(user_fd, &active_);

	// std::map<int, User>::iterator it1 = map_users.find(user_fd);
	// char tmp[4048];
	// std::string str;
	// sprintf(tmp, "IRC : client %d just arrived\n", user_fd); // new_user.getUserFd()
	// new_message.setRawMessage(str.append(tmp));
	// std::cout << "str " << str << std::endl;
	// storage_messages->insertMessage(user_fd, new_message); // new_user.getUserFd()
	// std::cout << "new mess " << new_message.getRestMess() << std::endl;
	// std::cout << "storage_messages->getMessageByFd(new_user.getUserFd()) " << storage_messages->getMessageByFd(new_user.getUserFd()) << std::endl;
	// it1->second.setMessage(static_cast<std::string>(tmp));

	// writeToUser(user_fd);
	return (0);
};

void	Core::error(int err_type) {
	std::cerr << err_type;
	write(2, "Fatal error\n", 12);
	exit(1);
};

int		Core::writeToUser(int current_fd) {
	std::map<int, User>::iterator itForUsers = map_users.find(current_fd);


	// std::map<int, User>::iterator itForMess = map_users.find(current_fd);


	if (itForUsers == map_users.end()) {
		write(2, "Users not found\n", 26);
		return (0) ; ////!!!!
	} 
	else  {
		std::string str = storage_messages->getRawMessageByFd(current_fd);

		std::string strForMess = storage_messages->getCmdInMessageByFd(current_fd);

std::cout << ">> getRawMessageByFd = " << str << std::endl;
std::cout << ">>> getCmdInMessageByFd " << strForMess << std::endl;

		// std::map<int, Message>::iterator itForMess = storage_messages->getMessages().find(current_fd);
		std::cout << "write: " << current_fd << std::endl;
		if (strForMess.empty()) {
			storage_messages->deleteMessage(current_fd);
			return (0);
		}
		if (itForUsers->second.getBotDialog() != NO) {
			send(current_fd, str.c_str(), str.length(), 0);
			storage_messages->deleteMessage(current_fd);
			if (itForUsers->second.getBotDialog() == FINISH) {
				itForUsers->second.setBotDialog(NO);
				storage_messages->deleteBot(current_fd);
			}
			return (0);
		}
		for(int s = 0; s <= max; ++s) {
			if (FD_ISSET(s, &write_) && s != current_fd) {
				//  std::cout << "str " << str << std::endl;
				// std::cout << "itForMess->second.restMess " << itForMess->second.getRestMess() << std::endl;
				// send(s, itForMess->second.getMessage().c_str(), itForMess->second.getMessage().length(), 0);
				send(s, str.c_str(), str.length(), 0);
				storage_messages->deleteMessage(current_fd);
			}
		}
	}

	return (0);
};

int		Core::readFromUser(int user_fd) {

	Message new_message;
	// storage_messages->insertMessage();
	std::string str;
	std::string cmd;
	// std::map<int, User>::iterator it1 = map_users.find(user_fd);
	length_message = 0;
	char tmp[4048];
	length_message = recv(user_fd, tmp, 42*4096, 0);
	str.append(tmp);
	str = str.substr(0, str.find("\r\n", 0)+1);
	if (length_message > 0){
		// parser_message(user_fd, tmp);

		new_message.setRawMessage(str);
		storage_messages->insertMessage(user_fd, new_message);
		storage_messages->parsRecvStr(str, &(map_users.find(user_fd))->second, map_users.begin(), map_users.end());
	
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


void Core::setServ(Server *newServ) {
	_irc_serv = newServ;
}

std::string Core::getServName() {
	return _irc_serv->getServName();
}
