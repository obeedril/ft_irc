#ifndef MESSENGER_HPP
# define MESSENGER_HPP

# include "Library.hpp"

class Messenger {

private:

    std::map<int, t_message> messages;

public:
    Messenger();
    ~Messenger();
   void parseBuffer(char* buf, User &user);


};

#endif