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
			break;
		}
		if (FD_ISSET(s, &read_) && s != listen_sock) {
			readFromUser(s);
			
			std::map<int, User>::iterator it1 = map_users.find(s);
			if (it1 == map_users.end()) {
				write(2, "Users not found\n", 26);
				return ; ////!!!!
			}
			if (length_message <= 0) {
				sprintf(bufWrite, "IRC : client %d just left\n", it1->second.getUserFd());
				writeToUser(s);
				FD_CLR(s, &active_);
				close(s);
				break ;
			}
			else {
				for (int i = 0, j = 0; i < length_message; ++i, ++j) {
					str[j] = bufRead[i];
					if (str[j] == '\n') {
						str[j] = '\0';
						sprintf(bufWrite, "%s %d: %s\n", it1->second.getUserName().c_str(), it1->second.getUserFd(), str);
						writeToUser(s);
						j = -1;
					}
				}
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
	map_users.insert(std::pair<int, User> (user_fd, new_user));

	new_user.count_cli = count_cli;
	id[user_fd] = count_cli;
	count_cli++;
	FD_SET(user_fd, &active_);

	sprintf(bufWrite, "IRC : client %d just arrived\n", new_user.getUserFd());
	writeToUser(user_fd);
	return (0);
};

void	Core::error(int err_type) {
	std::cerr << err_type;
	write(2, "Fatal error\n", 12);
	exit(1);
};

int		Core::writeToUser(int current_fd) {
	for(int s = 0; s <= max; ++s) {
		if (FD_ISSET(s, &write_) && s != current_fd) {
			send(s, bufWrite, strlen(bufWrite), 0);
		}
	}
	return (0);
};

int		Core::readFromUser(int user_fd) {
	length_message = 0;
	length_message = recv(user_fd, bufRead, 42*4096, 0);
	//parser message!!!!
	//
	return (0);
};	