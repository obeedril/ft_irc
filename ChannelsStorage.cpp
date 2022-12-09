#include "Library.hpp"

ChannelsStorage::ChannelsStorage(){}

ChannelsStorage::~ChannelsStorage(){}

void ChannelsStorage::setChannels(std::map<std::string, std::list<User>> _channels){
    channels = _channels;
}

std::map<std::string, std::list<User>> ChannelsStorage::getChannels(){
    return channels;
}