#ifndef CHANNELSSTORAGE_HPP
# define CHANNELSSTORAGE_HPP

# include "Library.hpp"

class ChannelsStorage {

private:
	std::map<std::string, t_channel> channels;

public:
	ChannelsStorage();
	~ChannelsStorage();

	void setChannels(std::map<std::string, t_channel> _channels);
	std::map<std::string, t_channel> getChannels();

	void addNewChannel(std::string channel_name, User *owner);
	std::string searchChannel(std::string channel_name);

	void setTopic(std::string channel_name, std::string topic);
	std::string getTopic(std::string channel_name);

	std::string joinToCannel(std::string msg, User *user);
	std::string	kickUser(std::string msg, User *user);

};

#endif