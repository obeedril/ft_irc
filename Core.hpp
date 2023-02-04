#ifndef CORE_HPP
# define CORE_HPP

#include "Library.hpp"

class Core {
	private:

		int					listen_sock;
		int					port;
		int					max;
		struct sockaddr_in	servaddr;
		fd_set				read_, write_, active_;
		socklen_t			len_adr;
		Server*				_irc_serv;

		int			length_message;
		Messenger*	storage_messages;

	public:
		Core(int port_, Server *serv);
		~Core();

		void		run();
		void		error(int err_type);
		void		quitUser(int user_fd);
		int			createNewSocket();
		int			writeToUser(int current_fd); 
		int			readFromUser(int user_fd);

		Messenger*	getStorage_messages();
		void		setServ(Server *newServ);
		Server*		getServ();
};

#endif

