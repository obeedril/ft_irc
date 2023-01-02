#ifndef BOT_HPP
# define BOT_HPP

# include "Library.hpp"

enum t_bot_comm
{
	HELLO_BOT,
    PLAY,
    WEATHER,
    BYE_BOT,
    READY,
    NO_COMM
};

class Bot {
    private:
        User *my_client;
        int UserFd;
        int startGame;
        int gameNum;
        

        std::string message;
        t_bot_comm bot_com;

    public:

        Bot(User *my_client);
        ~Bot();

        int callBot(std::string msg);
        int playGame(std::string msg);
        int getWeather();
        void exitBot();

};

#endif
