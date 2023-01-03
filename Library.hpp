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

#include <cmath> //для робота

#define PATH_TO_CONFIG "config.conf"
#define SERV_NAME "serverName"
#define ADMIN_NAME "adminName"
#define ADMIN_PASS "adminPass"
#define OPERATORS "operators"
#define COUNT_TOKENS 4

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
	KICK,
	BOT
};

enum t_bot_command
{
	HELLO_BOT,
    PLAY,
    WEATHER,
    BYE_BOT,
    READY,
    NO_COMM,
	YES,
	NO,
	FINISH
};

// # include "ChannelsStorage.hpp"
# include "User.hpp"
# include "Server.hpp"
# include "Bot.hpp"
# include "Core.hpp"


#endif
