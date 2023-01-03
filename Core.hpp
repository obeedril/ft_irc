#ifndef CORE_HPP
# define CORE_HPP

#include "Library.hpp"

class Core {
	private:

		int listen_sock;
		int id[64*1024];
		int port;
		char bufRead[42*4096], str[42*4096], bufWrite[42*4097];
		int max;
		fd_set read_, write_, active_;
		int count_cli;
		struct sockaddr_in servaddr;
		socklen_t len_adr;

		int length_message;

		std::map<int, User> map_users;
		
		Messenger *storage_messages;
		Message *message;



		//int flag_msg;

	public:
		Core(int port_);
		~Core();

		void	run();
		int		createNewSocket();
		int		writeToUser(int current_fd); 
		int		readFromUser(int user_fd);

		void	error(int err_type);

		void parser_message(int user_fd, char *bufRead);
		// void setStorage_messages(Messenger *_storage_messages);
		Messenger *getStorage_messages();
		// void check_message(int user_fd);
};

#endif
