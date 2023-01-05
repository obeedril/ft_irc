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
#define PATH_TO_MOTD "welcome.motd"
#define SERV_NAME "serverName"
#define ADMIN_NAME "adminName"
#define ADMIN_PASS "adminPass"
#define OPERATORS "operators"
#define ONE_USER 1
#define TO_ALL_BUT_NO_THIS_USER 2
#define TO_ALL 3
#define TO_CHANNEL_BUT_NO_THIS_USER 4
#define TO_CHANNEL 5

// typedef struct s_message{
// 	std::string cmd;
// 	std::string receiver;
// 	std::string restMess;
// 	std::string prefix;

// } t_message;

typedef struct s_message{
	std::string cmd;
	std::string receiver;
	std::string restMess;

} t_message;

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
# include "Server.hpp"
# include "ErrorsAndReplies.hpp"
# include "User.hpp"
# include "Message.hpp"
# include "Bot.hpp"
# include "Messenger.hpp"
# include "Core.hpp"

#endif
