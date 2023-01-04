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

    void Message::setReadyMess(std::string mess) {
        ready_mess = mess;
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

    std::string Message::getReadyMess() {
        return ready_mess;
    }