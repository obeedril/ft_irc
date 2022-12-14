#include "Library.hpp"

User::User(Server *new_irc_serv){
    flagRegistred = false;
    _irc_serv = new_irc_serv;
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

std::string User::getServName() {
	return _irc_serv->getServName();
}

std::vector<std::string>	User::getMotdFromServer() {
    return _irc_serv->getMotd();
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

t_bot_command User::getBotDialog() {
    return(botDialog);
}

void User::setBotDialog(t_bot_command botDialog) {
    this->botDialog = botDialog;
}

void User::setRegistFlag(bool flag){
    flagRegistred = flag;
}

void User::setServ(Server *newServ) {
	_irc_serv = newServ;
}

