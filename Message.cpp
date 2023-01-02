#include "Library.hpp"

    Message::Message(){}
    Message::~Message(){}

    void Message::setRawMessage(std::string _rawMessage){
        rawMessage = _rawMessage;
    }

    void Message::setCmd(std::string _cmd){
        cmd = _cmd;
    }
    void Message::setRestMess(std::string _restMess){
        restMess = _restMess;
    }
    void Message::setPrefix(std::string _restMess){
        restMess = _restMess;
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