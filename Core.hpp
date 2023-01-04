#ifndef CORE_HPP
# define CORE_HPP

#include "Library.hpp"

class Core {
	private:

		int listen_sock;
		int id[64*1024];
		int port;
		// char bufRead[42*4096];
		// char str[42*4096];
		char bufWrite[42*4097];
		int max;
		fd_set read_, write_, active_;
		int count_cli;
		struct sockaddr_in servaddr;
		socklen_t len_adr;
		Server *_irc_serv;

		int length_message;
		// size_t errorCode;

		std::map<int, User> map_users;
		
		Messenger *storage_messages;
		// Message *message;

		// Commands

		// int										passCmd(const Messenger &msg, User &user);
		// int										nickCmd(const Messenger &msg, User &user);
		// int										userCmd(const Messenger &msg, User &user);
		int										userCmd(const std::string &msg, int user_fd);
		// void	setError(size_t code);
		// size_t	getError();
		int		replyError(User &user, int err, const std::string &str1, const std::string &str2);
		// int										quitCmd(const Messenger &msg, User &user);
		// int										privmsgCmd(const Messenger &msg, User &user);
		// int										noticeCmd(const Messenger &msg, User &user);
		// int										joinCmd(const Messenger &msg, User &user);
		// int										kickCmd(const Messenger &msg, User &user);
		std::string tostring(std::vector<std::string> &v);


		//int flag_msg;

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
		// void check_message(int user_fd);
};

#endif

