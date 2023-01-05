#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Library.hpp"

class Message {

private:

    std::string rawMessage;
    std::string cmd;
    std::string readyMess;

	std::string receiver;
	std::string restMess;
	std::string prefix;
   

public:
    Message();
    ~Message();

    void setRawMessage(std::string _rawMessage);
    void setReadyMess(std::string _readyMess);
    void setCmd(std::string _cmd);
    void setRestMess(std::string _restMess);
    void setPrefix(std::string _prefix);
    void setReceiver(std::string _receiver);
    std::string getCmd();
    std::string getRestMess();
    std::string getPrefix();
    std::string getRawMessage();
    std::string getReadyMess();
    std::string getReceiver();

};

#endif