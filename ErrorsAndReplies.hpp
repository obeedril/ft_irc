#ifndef ERRORS_HPP
# define ERRORS_HPP

#include "Library.hpp"

// #define ERR_NOSUCHNICK			401
// #define ERR_NOSUCHSERVER		402
#define ERR_NOSUCHCHANNEL		403
// #define ERR_CANNOTSENDTOCHAN	404
// #define ERR_TOOMANYCHANNELS		405
// #define ERR_WASNOSUCHNICK		406
// #define ERR_TOOMANYTARGETS		407
// #define ERR_NOORIGIN			409
// #define ERR_NORECIPIENT			411
// #define ERR_NOTEXTTOSEND		412
// #define ERR_NOTOPLEVEL			413
// #define ERR_WILDTOPLEVEL		414
#define ERR_UNKNOWNCOMMAND		421
#define ERR_NOMOTD				422
// #define ERR_NOADMININFO			423
// #define ERR_FILEERROR			424
#define ERR_NONICKNAMEGIVEN		431
// #define ERR_ERRONEUSNICKNAME	432
#define ERR_NICKNAMEINUSE		433
// #define ERR_NICKCOLLISION		436
// #define ERR_USERNOTINCHANNEL	441
#define ERR_NOTONCHANNEL		442
// #define ERR_USERONCHANNEL		443
// #define ERR_NOLOGIN				444
// #define ERR_SUMMONDISABLED		445
// #define ERR_USERSDISABLED		446
// #define ERR_NOTREGISTERED		451
#define ERR_NEEDMOREPARAMS		461
#define ERR_ALREADYREGISTRED	462
// #define ERR_NOPERMFORHOST		463
// #define ERR_PASSWDMISMATCH		464
// #define ERR_YOUREBANNEDCREEP	465
// #define ERR_KEYSET				467
// #define ERR_CHANNELISFULL		471
// #define ERR_UNKNOWNMODE			472
// #define ERR_INVITEONLYCHAN		473
#define ERR_BANNEDFROMCHAN		474
// #define ERR_BADCHANNELKEY		475
// #define ERR_NOPRIVILEGES		481
#define ERR_CHANOPRIVSNEEDED	482
// #define ERR_CANTKILLSERVER		483
// #define ERR_NOOPERHOST			491
// #define ERR_UMODEUNKNOWNFLAG	501
// #define ERR_USERSDONTMATCH		502
#define RLP_REGIST_OK		001

// #define RPL_NONE				300
// #define RPL_USERHOST			302
// #define RPL_ISON				303
// #define RPL_AWAY				301
// #define RPL_UNAWAY				305
// #define RPL_NOWAWAY				306
// #define RPL_WHOISUSER			311
// #define RPL_WHOISSERVER			312
// #define RPL_WHOISOPERATOR		313
// #define RPL_WHOISIDLE			317
// #define RPL_ENDOFWHOIS			318
// #define RPL_WHOISCHANNELS		319
// #define RPL_WHOWASUSER			314
// #define RPL_ENDOFWHOWAS			369
// #define RPL_LISTSTART			321
// #define RPL_LIST				322
// #define RPL_LISTEND				323
// #define RPL_CHANNELMODEIS		324
// #define RPL_NOTOPIC				331
// #define RPL_TOPIC				332
// #define RPL_INVITING			341
// #define RPL_SUMMONING			342
// #define RPL_VERSION				351
// #define RPL_WHOREPLY			352
// #define RPL_ENDOFWHO			315
// #define RPL_NAMREPLY			353
// #define RPL_ENDOFNAMES			366
// #define RPL_LINKS				364
// #define RPL_ENDOFLINKS			365
// #define RPL_BANLIST				367
// #define RPL_ENDOFBANLIST		368
// #define RPL_INFO				371
// #define RPL_ENDOFINFO			374
#define RPL_MOTDSTART			375
#define RPL_MOTD				372
#define RPL_ENDOFMOTD			376
// #define RPL_YOUREOPER			381
// #define RPL_REHASHING			382
// #define RPL_TIME				391
// #define RPL_USERSSTART			392
// #define RPL_USERS				393
// #define RPL_ENDOFUSERS			394
// #define RPL_NOUSERS				395
// #define RPL_TRACELINK			200
// #define RPL_TRACECONNECTING		201
// #define RPL_TRACEHANDSHAKE		202
// #define RPL_TRACEUNKNOWN		203
// #define RPL_TRACEOPERATOR		204
// #define RPL_TRACEUSER			205
// #define RPL_TRACESERVER			206
// #define RPL_TRACENEWTYPE		208
// #define RPL_TRACELOG			261
// #define RPL_STATSLINKINFO		211
// #define RPL_STATSCOMMANDS		212
// #define RPL_STATSCLINE			213
// #define RPL_STATSNLINE			214
// #define RPL_STATSILINE			215
// #define RPL_STATSKLINE			216
// #define RPL_STATSYLINE			218
// #define RPL_ENDOFSTATS			219
// #define RPL_UMODEIS				221
// #define RPL_STATSLLINE			241
// #define RPL_STATSUPTIME			242
// #define RPL_STATSOLINE			243
// #define RPL_STATSHLINE			244
// #define RPL_LUSERCLIENT			251
// #define RPL_LUSEROP				252
// #define RPL_LUSERUNKNOWN		253
// #define RPL_LUSERCHANNELS		254
// #define RPL_LUSERME				255
// #define RPL_ADMINME				256
// #define RPL_ADMINLOC1			257
// #define RPL_ADMINLOC2			258
// #define RPL_ADMINEMAIL			259

/*

	switch (err)
	{
	// case ERR_NOSUCHNICK:
	// 	msg += " " + str + " :No such nick/channel\n";
	// 	break;
	// case ERR_NOSUCHSERVER:
	// 	msg += " " + str + " :No such server\n";
	// 	break;
	// case ERR_NOSUCHCHANNEL:
	// 	msg += " " + str + " :No such channel\n";
	// 	break;
	// case ERR_CANNOTSENDTOCHAN:
	// 	msg += " " + str + " :Cannot send to channel\n";
	// 	break;
	// case ERR_TOOMANYCHANNELS:
	// 	msg += " " + str + " :You have joined too many channels\n";
	// 	break;
	// case ERR_WASNOSUCHNICK:
	// 	msg += " " + str + " :There was no such nickname\n";
	// 	break;
	// case ERR_TOOMANYTARGETS:
	// 	msg += " " + str + " :Duplicate recipients. No str delivered\n";
	// 	break;
	// case ERR_NOORIGIN:
	// 	msg += " :No origin specified\n";
	// 	break;
	// case ERR_NORECIPIENT:
	// 	msg += " :No recipient given (" + str + ")\n";
	// 	break;
	// case ERR_NOTEXTTOSEND:
	// 	msg += " :No text to send\n";
	// 	break;
	// case ERR_NOTOPLEVEL:
	// 	msg += " " + str + " :No toplevel domain specified\n";
	// 	break;
	// case ERR_WILDTOPLEVEL:
	// 	msg += " " + str + " :Wildcard in toplevel domain\n";
	// 	break;
	case ERR_UNKNOWNCOMMAND:
		msg += " " + str + " :Unknown command\n";
		break;
	case ERR_NOMOTD:
		msg += " :MOTD File is missing\n";
		break;
	// case ERR_NOADMININFO:
	// 	msg += " " + str + " :No administrative info available\n";
	// 	break;
	// case ERR_FILEERROR:
	// 	msg += " :File error doing \n" + str + " on " + arg + "\n";
	// 	break;
	case ERR_NONICKNAMEGIVEN:
		msg += " :No nickname given\n";
		break;
	// case ERR_ERRONEUSNICKNAME:
	// 	msg += ss.str() + " " + user->getLogin();
	// 	msg += " " + str + " :Erroneus nickname\n";
	// 	// msg = ":" + user->getServName() + " 432" + " default " +  user->getLogin() + "\r\r\r\r\r\r :Erroneus nickname\n";
	// 	// std::cout << ">> >>>>>> msg = " << msg << std::endl;

	// 	break;
	case ERR_NICKNAMEINUSE:
		msg += " " + str + " :Nickname is already in use\n";
		break;
	// case ERR_NICKCOLLISION:
	// 	msg += " " + str + " :Nickname collision KILL\n";
	// 	break;
	// case ERR_USERNOTINCHANNEL:
	// 	msg += " " + str + " " + arg + " :They aren't on that channel\n";
	// 	break;
	// case ERR_NOTONCHANNEL:
	// 	msg += " " + str + " :You're not on that channel\n";
	// 	break;
	// case ERR_USERONCHANNEL:
	// 	msg += " " + str + " " + arg + " :is already on channel\n";
	// 	break;
	// case ERR_NOLOGIN:
	// 	msg += " " + str + " :User not logged in\n";
	// 	break;
	// case ERR_SUMMONDISABLED:
	// 	msg += " :SUMMON has been disabled\n";
	// 	break;
	// case ERR_USERSDISABLED:
	// 	msg += " :USERS has been disabled\n";
	// 	break;
	// case ERR_NOTREGISTERED:
	// 	msg += " :You have not registered\n";
	// 	break;
	case ERR_NEEDMOREPARAMS:
		msg += " " + str + " :Not enough parameters\n";
		break;
	case ERR_ALREADYREGISTRED:
		msg += " :You may not reregister\n";
		break;
	// case ERR_NOPERMFORHOST:
	// 	msg += " :Your host isn't among the privileged\n";
	// 	break;
	// case ERR_PASSWDMISMATCH:
	// 	msg += " :Password incorrect\n";
	// 	break;
	// case ERR_YOUREBANNEDCREEP:
	// 	msg += " :You are banned from this server\n";
	// 	break;
	// case ERR_KEYSET:
	// 	msg += " " + str + " :Channel key already set\n";
	// 	break;
	// case ERR_CHANNELISFULL:
	// 	msg += " " + str + " :Cannot join channel (+l)\n";
	// 	break;
	// case ERR_UNKNOWNMODE:
	// 	msg += " " + str + " :is unknown mode char to me\n";
	// 	break;
	// case ERR_INVITEONLYCHAN:
	// 	msg += " " + str + " :Cannot join channel (+i)\n";
	// 	break;
	// case ERR_BANNEDFROMCHAN:
	// 	msg += " " + str + " :Cannot join channel (+b)\n";
	// 	break;
	// case ERR_BADCHANNELKEY:
	// 	msg += " " + str + " :Cannot join channel (+k)\n";
	// 	break;
	// case ERR_NOPRIVILEGES:
	// 	msg += " :Permission Denied- You're not an IRC operator\n";
	// 	break;
	// case ERR_CHANOPRIVSNEEDED:
	// 	msg += " " + str + " :You're not channel operator\n";
	// 	break;
	// case ERR_CANTKILLSERVER:
	// 	msg += " :You cant kill a server!\n";
	// 	break;
	// case ERR_NOOPERHOST:
	// 	msg += " :No O-lines for your host\n";
	// 	break;
	// case ERR_UMODEUNKNOWNFLAG:
	// 	msg += " :Unknown MODE flag\n";
	// 	break;
	// case ERR_USERSDONTMATCH:
	// 	msg += " :Cant change mode for other users\n";
	// 	break;

	// default:
	// 	msg += "UNKNOWN ERROR\n";
	// 	break;
	}
*/

#endif