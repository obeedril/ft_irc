#ifndef USER_HPP
# define USER_HPP

# include "Library.hpp"

class User {
    private:
        int userFd;
        bool isAdminHere;
        std::string channelHere;
        std::string login; //nickname
        std::string userName; //username
        std::string realName; //realname
        std::string password;
        bool flagRegistred;

        t_bot_command botDialog; //for robot

    public:
        User();
        ~User();
        int getUserFd();
        bool getIsAdminHere();
        std::string getChannelHere();
        std::string getLogin(); //nickname
        std::string getUserName(); //username
        std::string getRealName(); //realname
        std::string getPassword();
        bool getRegistFlag();
        void setUserFd(int _userFd);
        void setIsAdminHere(bool _isAdminHere);
        void setChannelHere(std::string _channelHere);
        void setLogin(std::string _login);
        void setUserName(std::string _userName);
        void setRealName(std::string _realName);
        void setPassword(std::string _password);
        
        void setRegistFlag(bool);

        t_bot_command getBotDialog();
        void setBotDialog(t_bot_command botDialog);
};

#endif