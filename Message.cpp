#include "Library.hpp"

    Message::Message(){
        rawMessage = "";
        cmd = "";
	    receiver = "";
        restMess = "";
	    prefix = "";
    }
    Message::~Message(){}

    void Message::setRawMessage(std::string _rawMessage){
        rawMessage.clear();
        rawMessage = "";
        rawMessage = _rawMessage;
    }

    void Message::setCmd(std::string _cmd){
        cmd = _cmd;
    }
    void Message::setRestMess(std::string _restMess){
        // restMess.clear();
        // restMess = "";
        restMess = _restMess;
    }
    void Message::setPrefix(std::string _prefix){
        prefix = _prefix;
    }
    std::string Message::getCmd(){
        return cmd;
    }
    std::string Message::getRestMess(){
        return restMess;
    }
    std::string Message::getPrefix(){
        return prefix;
    }

    std::string Message::getRawMessage(){
        return rawMessage;
    }

    void Message::setReadyMess(std::string _readyMess){
        readyMess = _readyMess;
    }

    std::string Message::getReadyMess(){
        return readyMess;
    }

    void Message::setReceiver(std::string _receiver){
        receiver = _receiver;
    }

    std::string Message::getReceiver(){
        return receiver;
    }

