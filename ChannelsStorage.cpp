#include "Library.hpp"

ChannelsStorage::ChannelsStorage(){}

ChannelsStorage::~ChannelsStorage(){}

void ChannelsStorage::setChannels(std::map<std::string, t_channel> _channels){
    channels = _channels;
}

std::map<std::string, t_channel> ChannelsStorage::getChannels() {
    return channels;
}

void ChannelsStorage::addNewChannel(std::string channel_name, User *owner) {
    t_channel tmp;
    tmp.name = channel_name;
    tmp.owner = owner->getUserName();
    tmp.topic = "";

    channels.insert(std::pair<std::string, t_channel> (channel_name, tmp));
}

std::string ChannelsStorage::searchChannel(std::string channel_name) {
    std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
    if (it_ch != channels.end()) {
        return(it_ch->second.owner);
    }
    return("");
}

std::string ChannelsStorage::getTopic(std::string channel_name) {
    std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
    if (it_ch != channels.end()) {
        return(it_ch->second.topic);
    }
    return(":No topic is set");
}

void ChannelsStorage::setTopic(std::string channel_name, std::string topic) {
    std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
    if (it_ch != channels.end()) {
       it_ch->second.topic = topic;
    }
}

std::string	ChannelsStorage::joinToCannel(std::string msg, User *user) {
	std::string		str = "";
	std::string		owner = "";
	std::string		topic = "";
	std::string		name_channel = "";
	std::vector<std::string> vector_string = splitString(msg, ' ');

	for(int i = 0; i < static_cast<int>(vector_string.size()); i++) {
		std::cout << "'" << vector_string[i] << "' ";
	}
	char t = vector_string[1][0];
	if (t == '#') {
		name_channel = vector_string[1];
		topic = getTopic(name_channel);
		owner = searchChannel(name_channel);
		if (owner == "") {
			addNewChannel(name_channel, user);
			std::cout << "searchChannel: '" << user->getUserName() << "' ";
			owner = "Dlana";
		}
		str.append(":" + owner + "!" + owner + "@127.0.0.1 ");
		str.append(vector_string[0] + " :" + vector_string[1] + "\n");
		//
		str.append(":IRC-kitty 331 ");
		str.append(user->getUserName() +  " " + name_channel +  " " + topic + "\n");
		str.append(":IRC-kitty 353 ");
		str.append(user->getUserName() + " = " + name_channel + " :@" + owner +  " " + user->getUserName() + "\n");
		str.append(":IRC-kitty 366 " + user->getUserName() +  " " + name_channel +  " " + ":End of /NAMES list\n");
		//:Dlana!Dlana@127.0.0.1 JOIN :#ggg
		//str = str_1 + str_2 + str_3;
		//SEND: :IRCat 331 B #SS :No topic is set
		//SEND: :IRCat 353 B = #SS :@B
		//SEND: :IRCat 366 B #SS :End of /NAMES list
	}	
	else {
		str = toString(ERR_NOSUCHCHANNEL) + " Channel!\n";
	}
	std::cout << "joinToCannel--> " << str << std::endl;
	std::cout << "name_channel: " << name_channel << std::endl;
	std::cout << "user->getUserName(): " << user->getUserName() << std::endl;
	return(str);
}

std::string	ChannelsStorage::kickUser(std::string msg, User *user) {
	size_t 			start_channel = msg.find("#");
	size_t 			end_channel;
	std::string		str = "";
	std::string		owner = "";
    std::string		name_channel = "";
    std::string		name = "";// = user->getUserName();
	std::vector<std::string> vector_string = splitString(msg, ' ');
	for(int i = 0; i < static_cast<int>(vector_string.size()); i++) {
		std::cout << "'" << vector_string[i] << "' ";
	}
	std::cout << "\n";
	name = vector_string[2];
	std::cout << "kickUser--> " << name << std::endl;
	
    if (start_channel != std::string::npos) {
		end_channel = msg.substr(start_channel).find(" ");
		if (end_channel != std::string::npos) {
			name_channel = msg.substr(start_channel, end_channel - start_channel - 1);
			std::cout << "name_channel: " << name_channel << std::endl;
			name = msg.substr(start_channel, msg.length() - start_channel - 2);
		}
		else {
			str.append(toString(ERR_NEEDMOREPARAMS) +  " " + name_channel + " not Found!\n");
			return(str);
		}
    }
	owner = "Test";//searchChannel(name_channel);
    if (owner != "" && owner != user->getUserName()) {
		str.append(":" + owner + "!" + owner);
		str.append("@127.0.0.1 ");
		str.append(msg);
		str.append(" :" + owner + "\n");
		//:456!456@127.0.0.1 KICK #WE 123 :456
    }
    else {
        str.append((ERR_NOSUCHCHANNEL) +  " " + name_channel + " not Found!\n");
    }
	std::cout << "kickUser--> " << str << std::endl;
	return(str);
}

std::vector<std::string> ChannelsStorage::getUsersNamesInThisChannel(std::string msg, std::map<int, User> *map_users) {
	std::vector<std::string>	new_deque;
	std::vector<std::string>	vector_string = splitString(msg, ' ');
	std::string					owner;

	owner = searchChannel(vector_string[1]);
	for(std::map<int, User>::iterator it = map_users.begin(); it != map_users.end(); it++) {
		if (it->second.getChannelHere() == vector_string[1]) {
			new_deque.push_back(it->second.getUserName());
		}
	}
	return new_deque;
}














//### KICK <a name="kick"></a>

//**Параметры:** `<channel>` `<user>` `[<comment>]`

// :Dlana!Dlana@127.0.0.1 JOIN :#www
// :irc.ircnet.su 353 Dlana = #www :@Dlana
// :irc.ircnet.su 366 Dlana #www :End of /NAMES list.

// :irc.ircnet.su 001 Dlana :Welcome to the IrcNet.ru IRC Network Dlana!Dlana@127.0.0.1
//https://github.com/clokep/irc-docs/blob/main/draft-mitchell-irc-capabilities-02.html

//
//nc -l 6667
//nc irc.ircnet.ru 6667