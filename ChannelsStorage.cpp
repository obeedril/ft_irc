#include "Library.hpp"

ChannelsStorage::ChannelsStorage() {
}

ChannelsStorage::~ChannelsStorage() {
}

std::map<std::string, t_channel>* ChannelsStorage::getChannels() {
		return channels;
}

bool ChannelsStorage::addNewChannel(std::string channel_name) {
		t_channel tmp;
		tmp.name = channel_name;
		tmp.topic = "";
	
		return(channels->insert(std::pair<std::string, t_channel> (channel_name, tmp)).second);
}

bool ChannelsStorage::addUserToChannel(std::string channel_name, User *user) {
	std::map<std::string, t_channel>::iterator it_ch = channels->find(channel_name);
	if (it_ch != channels->end()) {
			it_ch->second.list_users.push_back(user);
			return(true);
	}
	return(false);
}

std::string ChannelsStorage::getOwnerChannel(std::string channel_name) {
		std::map<std::string, t_channel>::iterator it_ch = channels->find(channel_name);
		if (it_ch != channels->end()) {
				return(it_ch->second.list_users.front()->getLogin());
		}
		return("");
}

t_channel *ChannelsStorage::getChannelByName(std::string channel_name) {
		
	
		std::map<std::string, t_channel>::iterator it_ch = channels->find(channel_name);
		if (it_ch != channels->end()) {
				return(&it_ch->second);
		}
		t_channel *channel_empty = new t_channel;
		channel_empty->name = "";
		channel_empty->topic = "No topic is set";
		return(channel_empty);
}

std::string ChannelsStorage::getTopic(std::string channel_name) {
		std::map<std::string, t_channel>::iterator it_ch = channels->find(channel_name);
		if (it_ch != channels->end()) {
				return(it_ch->second.topic);
		}
		return("No topic is set");
}

void ChannelsStorage::setTopic(std::string channel_name, std::string topic) {
		std::map<std::string, t_channel>::iterator it_ch = channels->find(channel_name);
		if (it_ch != channels->end()) {
			 it_ch->second.topic = topic;
		}
}

bool ChannelsStorage::checkIfThisUserBanned(std::string name_channel, std::string user) {
		std::list<std::string> banned_users;
	
		banned_users = getChannelByName(name_channel)->banned_users;
		if (std::find(banned_users.begin(), banned_users.end(), user) != banned_users.end()) {
			return(true);
		}
	return(false);
}

bool ChannelsStorage::foundUserInThisChannel(std::string name_channel, std::string user_name) {
		std::list<User*> list_users;
		t_channel *channel;
		channel = getChannelByName(name_channel);
		if (channel->name == "") {
			delete channel;
			return(false);
		}
		list_users = channel->list_users;
		for(std::list<User*>::iterator it = list_users.begin(); it != list_users.end(); it++) {
			 if((*it)->getLogin() == user_name) {
				return(true);
			 }
		}
	return(false);
}

User *ChannelsStorage::getUserInThisChannel(std::string name_channel, std::string user_name) {
		std::list<User*> list_users;
	
		list_users = getChannelByName(name_channel)->list_users;
		for(std::list<User*>::iterator it = list_users.begin(); it != list_users.end(); it++) {
			 if((*it)->getLogin() == user_name) {
				return((*it));
			 }
		}
	write(2, "getUserInThisChannel ERROR\n", 28);
	exit(-1);
}

std::string	ChannelsStorage::joinToCannel(std::string msg, User *user, int flags) {
	std::list<User*> users_in;
	std::string		owner = "";
	std::string		str = "";
	std::string		topic = "";
	std::string		name_channel = "";

	std::string		user_n = user->getLogin();
	std::cout << "\n------user->getLogin() JOIN-----:\n" << str << std::endl;
	std::vector<std::string> vec_msg = splitString(msg, ' ');
	std::cout << "\n------splitString JOIN-----:\n" << str << std::endl;
	// for(int i = 0; i < static_cast<int>(vec_msg.size()); i++) {
	// 	std::cout << "<" << vec_msg[i] << "> ";
	// }
	if (vec_msg.size() != 2 && flags == SYSTEM_MSG) {
		str.append(":IRC-kitty " + toString(ERR_NEEDMOREPARAMS) + " " + vec_msg[0] + " :Not enough parameters\n");
	}
	else if (vec_msg[1][0] != '#' && flags == SYSTEM_MSG) {
		str.append(":IRC-kitty " + toString(ERR_NOSUCHCHANNEL) + " " + vec_msg[1] + " :No such channel\n");
	}
	else if (checkIfThisUserBanned(vec_msg[1], user_n) == true && flags == SYSTEM_MSG) {
		str.append(":IRC-kitty " + toString(ERR_BANNEDFROMCHAN) + " " + vec_msg[1] + " :Cannot join channel (+b)\n");
	}
	else {
		name_channel = vec_msg[1];
		std::cout << "\n------nnel JOIN false-----:\n" << str << std::endl;
		if (foundUserInThisChannel(name_channel, user->getLogin()) == false) {
		 std::cout << "\n------foundUserInThisChannel JOIN false-----:\n" << str << std::endl;
			addNewChannel(name_channel);
			addUserToChannel(name_channel, user);
		}
		std::cout << "\n------nnel JOIN ater-----:\n" << str << std::endl;
		//Join to channel Success
		str.append(":" + user_n + "!" + user_n + "@127.0.0.1 ");
		str.append("JOIN :" + name_channel + "\n");
		
		if (flags == SYSTEM_MSG) {
			//Get Topic
			topic = getTopic(name_channel);
			str.append(":IRC-kitty 331 " + user_n +  " " + name_channel +  " :" + topic + "\n");
			//Get NAMES list in this channel
			users_in = getChannelByName(name_channel)->list_users;
			str.append(":IRC-kitty 353 " + user_n + " = " + name_channel + " :@");
			for (std::list<User*>::iterator it = users_in.begin(); it != users_in.end(); ++it) {
				str.append((*it)->getLogin() + " ");
			}
			//End of /NAMES list
			str.append("\n:IRC-kitty 366 " + user_n +  " " + name_channel + " :End of /NAMES list\n");
		}
	}
	std::cout << "\n------ReadyMess JOIN-----:\n" << str << std::endl;
	std::cout << "name_channel: " << name_channel << std::endl;
	std::cout << "user->getUserName(): " << user_n << std::endl;
	return(str);
}

std::string	ChannelsStorage::kickUser(std::string msg, User *user, int flags) {
	//Params: KICK <channel> <user> [<comment>]
	std::string		str = "";
	std::list<User*> users_in;
	//std::string		name_channel = "";
	std::string		user_n = user->getLogin();
	std::vector<std::string> vec_msg = splitString(msg, ' ');

	for(int i = 0; i < static_cast<int>(vec_msg.size()); i++) {
		std::cout << "<" << vec_msg[i] << "> ";
	}
	std::cout << "\n";
	std::cout << "\n------Check KICK-----:\n" << str << std::endl;
	std::cout << getOwnerChannel(vec_msg[1]) << std::endl;
	std::cout << user->getIsAdminServer() << std::endl;
	std::cout << user->getIsOperatorServer() << std::endl;
	 std::cout << "\n------Check KICK-----:\n" << str << std::endl;
	if (vec_msg.size() < 3 && flags == SYSTEM_MSG) {
		str.append(":IRC-kitty " + toString(ERR_NEEDMOREPARAMS) + " " + vec_msg[0] + " :Not enough parameters\n");
	}
	else if (foundUserInThisChannel(vec_msg[1], user->getLogin()) == false && flags == SYSTEM_MSG) {
		str.append(":IRC-kitty " + toString(ERR_NOTONCHANNEL) + " " + vec_msg[1] + " :You're not on that channel\n");
	}
	else if (user->getIsAdminServer() == false 
						&& user->getIsOperatorServer() == false 
						&& getOwnerChannel(vec_msg[1]) != user_n 
						&& flags == SYSTEM_MSG) {
		str.append(":IRC-kitty " + toString(ERR_CHANOPRIVSNEEDED) + " " + vec_msg[1]
			+ " :You're not channel operator\n");
	}
	else if (user->getLogin() == vec_msg[2] && flags == SYSTEM_MSG) {
		str.append(":IRC-kitty " + toString(ERR_NEEDMOREPARAMS) + " " + vec_msg[0] + " :Not enough parameters\n");
	}
	else {
		if (foundUserInThisChannel(vec_msg[1], vec_msg[2]) == true) {
				bannedUserInThisChannel(vec_msg[1], getUserInThisChannel(vec_msg[1], vec_msg[2]));
		}
		str.append(":" + user_n + "!" + user_n + "@127.0.0.1 ");
		str.append(msg + " :" + user_n + "\n");
		
		 if (flags == SYSTEM_MSG) {
		//Get NAMES list in this channel
		users_in = getChannelByName(vec_msg[1])->list_users;
		str.append(":IRC-kitty 353 " + user_n + " = " + vec_msg[1] + " :@");
		for (std::list<User*>::iterator it = users_in.begin(); it != users_in.end(); ++it) {
			str.append((*it)->getLogin() + " ");
		}
		//End of /NAMES list
		str.append("\n:IRC-kitty 366 " + user_n +  " " + vec_msg[1] + " :End of /NAMES list\n");
		//:456!456@127.0.0.1 KICK #WE 123 :456
		 }
	}
	std::cout << "\n------ReadyMess KICK-----:\n" << str << std::endl;
	std::cout << getOwnerChannel(vec_msg[1]) << std::endl;
	std::cout << user_n << std::endl;
	return(str);
}

std::string	ChannelsStorage::addTopicToCannel(std::string msg, User *user) {
	
	std::list<std::string> users_in;
	std::string		owner = "";
	std::string		str = "";
	std::string		topic = "";
	std::string		num = "";
	std::string		user_n = user->getLogin();
	std::vector<std::string> vec_msg = splitString(msg, ' ');
	
	for(int i = 0; i < static_cast<int>(vec_msg.size()); i++) {
		std::cout << "<" << vec_msg[i] << "> ";
	}
	std::cout << "\n";
	
	if (vec_msg.size() != 2 && vec_msg.size() != 3) {
		str.append(":IRC-kitty " + toString(ERR_NEEDMOREPARAMS) + " " + vec_msg[0] + " :Not enough parameters\n");
	}
	else if (foundUserInThisChannel(vec_msg[1], user->getLogin()) == false) {
		str.append(":IRC-kitty " + toString(ERR_NOTONCHANNEL) + " " + vec_msg[1] + " :You're not on that channel\n");
	}
	else if (getChannelByName(vec_msg[1])->name == "") {
		str.append(":IRC-kitty " + toString(ERR_NOSUCHCHANNEL) + " " + vec_msg[1] + " :No such channel\n");
	}
	else {
		if (vec_msg.size() == 3) {
			setTopic(vec_msg[1], vec_msg[2]);
		}
		//Get Topic
		topic = getTopic(vec_msg[1]);
		if (topic == "No topic is set") {
			num.append("331 ");
		}
		else {
			num.append("332 ");
		}
		str.append(":IRC-kitty " + num + user_n +  " " + vec_msg[1] +  " :" + topic + "\n");
	}
	std::cout << "\n------ReadyMess TOPIC-----:\n" << str << std::endl;
	return(str);
}

bool	ChannelsStorage::bannedUserInThisChannel(std::string channel_name, User *user) {
	std::map<std::string, t_channel>::iterator it_ch = channels->find(channel_name);
	if (it_ch != channels->end()) {
			it_ch->second.list_users.remove(user);
			it_ch->second.banned_users.push_back(user->getLogin());
			return(true);
		}
	return(false);
}


std::string ChannelsStorage::updateChannels(User *user, std::string new_user_name, int command) {
	new_user_name = "";
	std::string		str = "";
	std::string	user_n	= user->getLogin();
	if (command == DELETE_USER) {
		for (std::map<std::string, t_channel>::iterator it = channels->begin(); it != channels->end(); it++) {
			if (foundUserInThisChannel(it->second.name, user_n) == true) {
				str.append(":" + user_n + "!" + user_n + "@127.0.0.1 ");
				str.append("PART " + it->second.name + "\n");
			}
			it->second.list_users.remove(user);
			it->second.banned_users.remove(user_n);
		}
	}
	return(str);
}

//sendPartCmd

std::vector<int> ChannelsStorage::getDequeByChannel(std::string name_channel, User *user) {
	std::list<User*> list_users;
	std::vector<int> vector;
	if(foundUserInThisChannel(name_channel, user->getLogin()) == true) {
		list_users = getChannelByName(name_channel)->list_users;
		for(std::list<User*>::iterator it = list_users.begin(); it != list_users.end(); it++) {
				if(user->getUserFd() != (*it)->getUserFd()) {
					vector.push_back((*it)->getUserFd());
				}
		}
	}
	return(vector);
}

std::vector<int> ChannelsStorage::getDequeByAllInChannel(std::string name_channel, User *user) {
	std::list<User*> list_users;
	std::vector<int> vector;
	if(foundUserInThisChannel(name_channel, user->getLogin()) == true) {
		std::cout << "\x1b[1;95m" << "getDeque: " << "\x1b[0m";
		list_users = getChannelByName(name_channel)->list_users;
		for(std::list<User*>::iterator it = list_users.begin(); it != list_users.end(); it++) {
					vector.push_back((*it)->getUserFd());
					std::cout << "\x1b[1;95m" << "getDequeByAllInChannel: " << (*it)->getUserFd() << "\x1b[0m";
		}
	}
	std::cout << std::endl;
	return(vector);
}


std::string ChannelsStorage::parserChannelInMsg(std::string msg) {
	std::vector<std::string> vec_msg = splitString(msg, ' ');
	if (vec_msg.size() >= 2 && vec_msg[1].length() >= 2 && vec_msg[1][0] == '#' ) {
			return(vec_msg[1]);
	}
	return("");
}
