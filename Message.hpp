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

    std::vector<int> deque;
    std::string msgForSender;

    std::vector<std::string> listOfRecievers;

public:
    Message();
    ~Message();

    void setRawMessage(std::string _rawMessage);
    void setReadyMess(std::string _readyMess);
    void setCmd(std::string _cmd);
    void setRestMess(std::string _restMess);
    void setPrefix(std::string _prefix);
    void setReceiver(std::string _receiver);
    void setListOfRecievers(std::vector<std::string> _listOfRecievers);
    std::vector<std::string> getListOfRecievers();
    std::string getCmd();
    std::string getRestMess();
    std::string getPrefix();
    std::string getRawMessage();
    std::string getReadyMess();
    std::string getReceiver();

    std::vector<int> getDeque();
    void setDeque(std::vector<int> deque);
    // void clearDeque(std::vector<int> deque);
    void setMessForSender(std::string msgForSender); // for system messages
    std::string getMessForSender();
    // void addReciever();
};

#endif