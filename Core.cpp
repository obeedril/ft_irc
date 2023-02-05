# include "Library.hpp"

Core::Core(int port_, Server *serv) {
	_irc_serv = serv;
	storage_messages = new Messenger(_irc_serv->getServName());
	listen_sock = 0;
	this->port = port_;
	max = 0;
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
	delete storage_messages;
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
			if (storage_messages->getUserFd(s) == -1) {
				return ;
			}
			else if (length_message <= 0 || storage_messages->getCmdInMessageByFd(s) == "QUIT") {
				quitUser(s);
				FD_CLR(s, &active_);
				close(s);
				break ;
			}
			else {
				writeToUser(s);
			}
		}
	}
};

void Core::quitUser(int fd) {
	std::string msg = "";

	storage_messages->dequeMaker(storage_messages->getUser(fd), TO_ALL_BUT_NO_THIS_USER);
	msg = storage_messages->getChannels()->updateChannels(storage_messages->getUser(fd), "", DELETE_USER);
	storage_messages->setReadyMessInMessageByFd(msg, fd);
	storage_messages->deleteUser(fd);
	if (storage_messages->getMapUsers().size() > 1) {
		writeToUser(fd);
	}	
}

int		Core::createNewSocket() {
	int user_fd = accept(listen_sock, (struct sockaddr *)&servaddr, &len_adr);
	if (user_fd < 0) {
		return (-1);
	}
	if (max < user_fd) {
		max = user_fd;
	}
	User	new_user(_irc_serv);
	Message	new_message;
	new_user.setUserFd(user_fd);

	new_user.setUserName("");
	new_user.setBotDialog(NO);
	storage_messages->addUser(user_fd, new_user);
	fcntl(user_fd, F_SETFL, O_NONBLOCK);
	FD_SET(user_fd, &active_);
	return (0);
};

int		Core::writeToUser(int current_fd) {
	std::vector<int> deque; 
	deque = storage_messages->getDeq(current_fd);
	std::string msg = storage_messages->getReadyMessByFd(current_fd);
	std::string systemMsg = storage_messages->getSystemMsg(current_fd);
	std::cout << "\x1b[1;96m" << "Count deque: " << deque.size() << "\x1b[0m" << std::endl;
	if (msg != "") {
		std::cout << "---> msg: \n" << msg  << "<---" << std::endl;
	}
	if (systemMsg != "") {
		std::cout << "---> systemMsg: \n" << systemMsg << "<---" << std::endl;
		send(current_fd, systemMsg.c_str(), systemMsg.length(), 0);
	}
	for(int i = 0; i < static_cast<int>(deque.size()); i++) {
		if (FD_ISSET(deque[i], &write_)) {
			send(deque[i], msg.c_str(), msg.length(), 0);
		}
	}
	storage_messages->deleteMessage(current_fd);
	return (0);
};

int		Core::readFromUser(int user_fd) {
	Message		new_message;
	char		tmp[4048];
	std::string	str = "";

	length_message = 0;
	length_message = recv(user_fd, tmp, 42*4096, 0);
	str.append(tmp);
	str = getFirstLine(str);
	std::cout << "RECV STR: <" << str << ">" << std::endl;
	new_message.setRawMessage(str);
	storage_messages->insertMessage(user_fd, new_message);
	storage_messages->parsRecvStr(str, user_fd);
	return (length_message);
};

Messenger* Core::getStorage_messages() {
	return storage_messages;
};

void Core::setServ(Server *newServ) {
	_irc_serv = newServ;
}	

Server * Core::getServ(void) {
	return _irc_serv;;
}

void	Core::error(int err_type) {
	std::cerr << err_type;
	write(2, "Fatal error\n", 12);
	exit(1);
};