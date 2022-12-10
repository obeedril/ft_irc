#ifndef USER_HPP
# define USER_HPP

# include "Library.hpp"

class User {
    private:
        int userFd;
        bool isAdminHere;
        std::string channelHere;
        std::string login;
        std::string userName;
        std::string password;

    public:

        int count_cli; ////!!!!

        User();
        ~User();
        int getUserFd();
        bool getIsAdminHere();
        std::string getChannelHere();
        std::string getLogin();
        std::string getUserName();
        std::string getPassword();
        void setUserFd(int _userFd);
        void setIsAdminHere(bool _isAdminHere);
        void setChannelHere(std::string _channelHere);
        void setLogin(std::string _login);
        void setUserName(std::string _userName);
        void setPassword(std::string _password);

};


#endif