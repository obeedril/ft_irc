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
    return("");
}

void ChannelsStorage::setTopic(std::string channel_name, std::string topic) {
    std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
    if (it_ch != channels.end()) {
       it_ch->second.topic = topic;
    }
}

std::string	ChannelsStorage::joinToCannel(std::string msg, User *user) {
	size_t 			start_channel = msg.find("#");
	size_t 			end_channel;
	std::string		str = "";
	//std::string		str_1 = "";
	//std::string		str_2 = "";
	//std::string		str_3 = "";
	std::string		owner = "";
	std::string		topic = ":No topic is set";
	std::string		name_channel = "";
	if (start_channel != std::string::npos) {
		end_channel = msg.substr(start_channel).find(" ");
		if (end_channel != std::string::npos) {
			name_channel = msg.substr(start_channel, end_channel - start_channel - 1);
		}
		else {
			name_channel = msg.substr(start_channel, msg.length() - start_channel - 2);
		}
		topic = getTopic(name_channel);
		owner = searchChannel(name_channel);
		str.append(":IRC-kitty 331 ");
		str.append(user->getUserName() +  " " + name_channel +  " " + topic + "\n");
		if (owner == "") {
			addNewChannel(name_channel, user);
			str.append(":IRC-kitty 353 ");
			str.append(user->getUserName() + " = " + name_channel + " :@" + user->getUserName() + "\n");
		}
		else {
			str.append(":IRC-kitty 353 ");
			str.append(user->getUserName() + " = " + name_channel + " :@" + owner +  " " + user->getUserName() + "\n");
		}
		str.append(":IRC-kitty 366 " + user->getUserName() +  " " + name_channel +  " " + ":End of /NAMES list");
		//str = str_1 + str_2 + str_3;
    }
	//SEND: :IRCat 331 B #SS :No topic is set
	//SEND: :IRCat 353 B = #SS :@B
	//SEND: :IRCat 366 B #SS :End of /NAMES list
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
	std::string		owner = user->getUserName();
    std::string		name_channel = "";
    std::string		name = "";// = user->getUserName();
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
	owner = searchChannel(name_channel);
    if (owner != "") {
        str.append("KICK from Channel " + name_channel + "\n");
    }
    else {
        str.append((ERR_NOSUCHCHANNEL) +  " " + name_channel + " not Found!\n");
    }
	std::cout << "kickUser--> " << str << std::endl;
	return(str);
}

//### KICK <a name="kick"></a>

//**Параметры:** `<channel>` `<user>` `[<comment>]`