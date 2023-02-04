#ifndef CHANNELSSTORAGE_HPP
# define CHANNELSSTORAGE_HPP

# include "Library.hpp"

class ChannelsStorage {

private:
  std::map<std::string, t_channel> channels;

public:
	ChannelsStorage();
	~ChannelsStorage();
	//Out func
	std::map<std::string, t_channel> getChannels();
	std::string joinToCannel(std::string msg, User *user, int flags);
	std::string	kickUser(std::string msg, User *user, int flags);
	std::string	addTopicToCannel(std::string msg, User *user);
	std::string	partChannel(std::string msg, User *user);
	//Topic
	void		setTopic(std::string channel_name, std::string topic);
	std::string	getTopic(std::string channel_name);
	//Users
	bool		addUserToChannel(std::string channel_name, User *user);
	bool		checkIfThisUserBanned(std::string channel_name, std::string user_name);
	bool		foundUserInThisChannel(std::string name_channel, std::string user_name);
	bool		bannedUserInThisChannel(std::string channel_name, User *user);
	std::string	getOwnerChannel(std::string channel_name);
	User*		getUserInThisChannel(std::string name_channel, std::string user_name);
	//Channel
	bool		addNewChannel(std::string channel_name);
	t_channel*	getChannelByName(std::string channel_name);
	//Update
	std::string	updateChannels(User *user, std::string name_channel, int command);
	//Deque
	std::vector<int>	getDequeByChannel(std::string name_channel, User *user);
	std::vector<int>	getDequeByAllInChannel(std::string name_channel, User *user);
	std::string			parserChannelInMsg(std::string msg);

	//std::string	modeChannel(std::string msg, User *user);
	//std::string	whoIsInChannel(std::string msg, User *user);
};

#endif