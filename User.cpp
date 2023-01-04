#include "Library.hpp"

User::User(){
    flagRegistred = false;
}

User::~User(){}

int User::getUserFd(){
    return userFd;
}

bool User::getIsAdminHere(){
    return isAdminHere;
}

std::string User::getChannelHere(){
    return channelHere;
}

std::string User::getLogin(){
    return login;
}

std::string User::getUserName(){
    return userName;
}

std::string User::getRealName(){
    return realName;
}

std::string User::getPassword(){
    return password;
}

bool User::getRegistFlag(){
    return flagRegistred;
}

void User::setUserFd(int _userFd){
    userFd = _userFd;
}

void User::setIsAdminHere(bool _isAdminHere){
    isAdminHere = _isAdminHere;
}

void User::setChannelHere(std::string _channelHere){
    channelHere = _channelHere;
}

void User::setLogin(std::string _login){
    login = _login;
}

void User::setUserName(std::string _userName){
    userName = _userName;
}

void User::setRealName(std::string _realName){
    realName = _realName;
}

void User::setPassword(std::string _password){
    password = _password;
}

void User::setRegistFlag(bool flag){
    flagRegistred = flag;
}