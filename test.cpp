#include "Library.hpp"

int main(int argc, const char* argv[]) {
    std::map<std::string, std::string> channels;
    channels.insert(std::pair<std::string, std::string> ("1", "del")).second;
    channels.insert(std::pair<std::string, std::string> ("2", "da")).second;
    channels.insert(std::pair<std::string, std::string> ("3", "nat")).second;
    channels.insert(std::pair<std::string, std::string> ("4", "del")).second;
    channels.insert(std::pair<std::string, std::string> ("5", "hoho")).second;
    std::cout << "before"  << std::endl;
    for (std::map<std::string, std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		std::cout << it->second << ", ";
    }
    std::cout << std::endl;
    
    for (std::map<std::string, std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
			if (it->second == "del") {
				channels.erase(it->first);
			}
    }
    std::cout << "after"  << std::endl;
    for (std::map<std::string, std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		std::cout << it->second << ", ";
    }
    std::cout << std::endl;
}

