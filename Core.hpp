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

		//t_command command;

	public:
		Core(int port_);
		~Core();

		void	run();
		int		createNewSocket();
		int		writeToUser(int current_fd); 
		int		readFromUser(int user_fd);

		void	error(int err_type);
};

#endif

