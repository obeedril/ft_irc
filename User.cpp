#include "Library.hpp"

        User::User(){}

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

        std::string User::getPassword(){
            return password;
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

        void User::setPassword(std::string _password){
            password = _password;
        }

                t_command  User::getCommand() {
            return command;
        }

        void  User::setCommand(t_command _command) {
            command = _command;
        }

        std::string User::getMessage(){
            return(message);
        }

        void User::setMessage(std::string message){
            this->message.clear();
            this->message = message;
        }