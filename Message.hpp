#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Library.hpp"

class Message {

private:

    std::string rawMessage;
    std::string cmd;
    std::string ready_mess;

	std::string receiver;
	std::string restMess;
	std::string prefix;
   

public:
    Message();
    ~Message();

    void setRawMessage(std::string rawMessage);
    void setReadyMess(std::string ready_mess);
    void setCmd(std::string cmd);
    void setRestMess(std::string restMess);
    void setPrefix(std::string prefix);
    std::string getCmd();
    std::string getRestMess();
    std::string getPrefix();
    std::string getRawMessage();
    std::string getReadyMess();

};

#endif