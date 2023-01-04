#ifndef BOT_HPP
# define BOT_HPP

# include "Library.hpp"

class Bot {
    private:
        User *my_client;
        int UserFd;
        int startGame;
        int gameNum;
        int bad_dialog;
        

        std::string message;
        t_bot_command bot_com;

    public:

        Bot(User *my_client);
        ~Bot();

        int callBot(std::string msg);
        int playGame(std::string msg);
        int getWeather();
        int parserWeather();
        void exitBot();

        std::string getMsg();
        void setMsg(std::string msg);

};

#endif
