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
	User	new_user(_irc_serv);
	Message	new_message;
	new_user.setUserFd(user_fd);

	new_user.setUserName("");
	new_user.setBotDialog(NO);
	storage_messages->addUser(user_fd, new_user); //map_users.insert(std::pair<int, User> (user_fd, new_user));

	//new_user.count_cli = count_cli;
	//id[user_fd] = count_cli;
	//count_cli++;
	FD_SET(user_fd, &active_);
	return (0);
};

void	Core::error(int err_type) {
	std::cerr << err_type;
	write(2, "Fatal error\n", 12);
	exit(1);
};

int		Core::writeToUser(int current_fd) {

	std::vector<int> deque; 
	deque = storage_messages->getDeq(current_fd);
	std::string str = storage_messages->getRawMessageByFd(current_fd); // заменить на readyMess
	std::cout << "str109 |" << str << "|" << std::endl;

	for(int i = 0; i < static_cast<int>(deque.size()); i++) {
		if (FD_ISSET(deque[i], &write_)) {
			send(deque[i], str.c_str(), str.length(), 0);
			storage_messages->deleteMessage(current_fd);

		}
	}
	return (0);
};

int		Core::readFromUser(int user_fd) {
	Message		new_message;
	std::string	str;
	std::string	cmd;
	// std::map<int, User>::iterator it1 = map_users.find(user_fd);
	length_message = 0;
	char tmp[4048];
	length_message = recv(user_fd, tmp, 42*4096, 0);
	str.append(tmp);
	std::cout << "RECV STR0 |" << str << "|" << std::endl;
	str = str.substr(0, str.find("\r\n", 0) + 2);
	std::cout << "RECV STR1 |" << str << "|" << std::endl;
	if (length_message > 0){
		new_message.setRawMessage(str);
		storage_messages->insertMessage(user_fd, new_message);
		storage_messages->parsRecvStr(str, user_fd);
	}
	return (length_message);
};

Messenger* Core::getStorage_messages(){
	return storage_messages;
};


void Core::setServ(Server *newServ) {
	_irc_serv = newServ;
}

std::string Core::getServName() {
	return _irc_serv->getServName();
}
