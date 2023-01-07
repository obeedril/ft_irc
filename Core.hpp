#ifndef CORE_HPP
# define CORE_HPP

#include "Library.hpp"

class Core {
	private:

		int listen_sock;
		int id[64*1024];
		int port;
		char bufWrite[42*4097];

		int max;
		fd_set read_, write_, active_;
		int count_cli;
		struct sockaddr_in servaddr;
		socklen_t len_adr;
		Server *_irc_serv;

		int length_message;

		Messenger *storage_messages;
		std::string tostring(std::vector<std::string> &v);

		int count_mess;
		std::vector<std::string> vec_mess;

	public:
		Core(int port_);
		~Core();

		void	run();
		int		createNewSocket();
		int		writeToUser(int current_fd); 
		int		readFromUser(int user_fd);

		void	error(int err_type);

		Messenger *getStorage_messages();
		void setServ(Server *newServ);
		std::string getServName();
		void readFromVectorMessage(int user_fd);
		// void check_message(int user_fd);
};

#endif

