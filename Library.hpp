#ifndef LIBRARY_HPP
# define LIBRARY_HPP

# include <sys/socket.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <iostream>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <netdb.h>
# include <fcntl.h>
# include <poll.h>
# include <vector>
# include <list>
# include <string>
# include <sstream>
# include <fstream>
# include <map>
# include <cstdio>
# include <cstring>
# include <arpa/inet.h>
# include <algorithm>
# include <csignal>
# include <ctime>
# include <iomanip>

#define PATH_TO_CONFIG "config.conf"
#define SERV_NAME "serverName"
#define ADMIN_NAME "adminName"
#define ADMIN_PASS "adminPass"
#define OPERATORS "operators"
// #define COUNT_TOKENS 4

enum t_command
{
	NO_COMMAND = -1,
    USER,
	NICK,
	PASS,
    QIUT,
	PRIVMSG,
	NOTICE,
	JOIN,
	KICK
};

typedef struct s_message{
	std::string cmd;
	std::string receiver;
	std::string restMess;

} t_message;

// # include "ChannelsStorage.hpp"
# include "Errors.hpp"
# include "User.hpp"
# include "Messenger.hpp"
# include "Server.hpp"
# include "Core.hpp"



#endif