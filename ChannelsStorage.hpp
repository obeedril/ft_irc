#ifndef CHANNELSSTORAGE_HPP
# define CHANNELSSTORAGE_HPP

# include "Library.hpp"

class ChannelsStorage {

private:
    std::map<std::string, std::list<User>> channels;

public:
    ChannelsStorage();
    ~ChannelsStorage();

    void setChannels(std::map<std::string, std::list<User>> _channels);
    std::map<std::string, std::list<User>> getChannels();

};

#endif