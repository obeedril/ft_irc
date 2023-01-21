#ifndef ERRORS_HPP
# define ERRORS_HPP

#include "Library.hpp"

#define ERR_NOSUCHCHANNEL		403
#define ERR_UNKNOWNCOMMAND		421
#define ERR_NOMOTD				422
#define ERR_NONICKNAMEGIVEN		431
#define ERR_NICKNAMEINUSE		433
#define ERR_NOTONCHANNEL		442
#define ERR_NEEDMOREPARAMS		461
#define ERR_ALREADYREGISTRED	462
#define ERR_PASSWDMISMATCH		464
#define ERR_BANNEDFROMCHAN		474
#define ERR_CHANOPRIVSNEEDED	482
#define RLP_REGIST_OK		    001
#define RPL_CLOSING 			362
#define RPL_MOTDSTART			375
#define RPL_MOTD				372
#define RPL_ENDOFMOTD			376

#endif