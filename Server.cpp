#include "Library.hpp"

Server::Server(int port, const std::string &password) {
	_port = port;
	_argvPass = password;
	_configTokens.push_back(SERV_NAME);
	_configTokens.push_back(ADMIN_NAME);
	_configTokens.push_back(OPERATORS);


	std::string		line;
	std::ifstream	motdFile(PATH_TO_MOTD);
	if (motdFile.is_open())
	{
		while (getline(motdFile, line))
			_motd.push_back(line);
		motdFile.close();
	}

};

std::string	Server::getServName() {
	return _serverName;
}

std::vector<std::string>	Server::getMotd() {
	return _motd;
}

//   *** удаление пробелов, кавычек и ; ***
void Server::newDeleteSpaces(std::string *str) {
	
	size_t i = 0;
	while (isspace((*str)[i])) {
			++i;
	}
	if ((*str)[i] == '\"')
		++i;
	*str = (*str).substr(i, (*str).size());
	i = (*str).size() - 1;
	
	while (isspace((*str)[i]) || (*str)[i] == ';' || (*str)[i] == '\"') {
			--i;
	}

	if (i <= (*str).size())
		*str = (*str).substr(0, i + 1);
}

void Server::parseConfig() {
	std::vector<std::string> config = splitLines(PATH_TO_CONFIG);
	std::vector<std::string>::iterator end;
	std::vector<std::string>::iterator start;
	end = config.begin();

	while (end < config.end()) {
		size_t index = (*end).find(":");
		if (index != std::string::npos) {
			std::string sub = (*end).substr(1, index - 2);

		if (sub == SERV_NAME) {
			_serverName = (*end).substr(index + 1);
			newDeleteSpaces(&_serverName);
		}
		else if (sub == ADMIN_NAME) {
			_admin = (*end).substr(index + 1);
			newDeleteSpaces(&_admin);
		}
		else if (sub == OPERATORS)
			while (*(++end) != "}") {
				size_t indexMap = (*end).find(":");
				if (indexMap != std::string::npos) {
				std::string tmp = (*end).substr(indexMap + 1);
				newDeleteSpaces(&tmp);
				if (indexMap != std::string::npos) {
					_operators.insert(std::pair<std::string,std::string>((*end).substr(1, indexMap - 2), tmp));
				}
			}
		}
		}
		end++;
		}

	config.clear();
}

std::vector<std::string> Server::splitLines(std::string path) {	
	std::ifstream file(path.c_str());
	if (!(file.is_open()))
		throw std::runtime_error("Cannot open config file");

	std::string buf;
	std::vector<std::string> split;
	std::vector<std::string> res;

	while (!file.eof()) {
		getline(file, buf);
		if (emptyOrComments(buf))
			continue;
		split = checkBraces(buf);
		if (!split.empty()) {
			for (size_t i = 0; i < split.size(); ++i) {
				res.push_back(split[i]);
				split[i].clear();		
			}
		}
		else {
			deleteSpaces(&buf);
			res.push_back(buf);
		}
		buf.clear();
	}
	res.push_back("");
	file.close();
	return res;
}

//   *** вырезание по фигурным скобкам ***
std::vector<std::string> Server::checkBraces(std::string buf) {

	std::vector<std::string> vector;
	std::string lineBeforeBraces;
	std::string lineAfterBraces;
	std::string braces;
	std::string tmp;
	size_t i = 0;

	while (buf[i]) {
		if (buf[i] == '}' || buf[i] == '{') {
			lineBeforeBraces = buf.substr(0, i);
			deleteSpaces(&lineBeforeBraces);
			if (!lineBeforeBraces.empty())
				vector.push_back(lineBeforeBraces);
			lineBeforeBraces.clear();

			tmp = buf.substr(i, buf.size());
			braces = tmp.substr(0, 1);
			vector.push_back(braces);
			braces.clear();

			lineAfterBraces = tmp.substr(1, tmp.size());
			deleteSpaces(&lineAfterBraces);

			if (!lineAfterBraces.empty())
				vector.push_back(lineAfterBraces);
			lineAfterBraces.clear();
			tmp.clear();
		}
		++i;
	}
	return vector;
}



//   *** проверка на пустоту - только пробелы ***
int Server::onlySpaces(std::string buf) {

	size_t i = 0;
	while (buf[i]) {
		if (!isspace(buf[i++]))
			return 1;
	}
	return 0;
}

//   *** проверка на пустоту - только пробелы и комменты ***
int Server::emptyOrComments(std::string buf) {

	if (buf[0] == '#' || buf.empty()) {
		return 1;
	}
		if (buf.find('#')) {
		buf = buf.substr(0, buf.find('#'));
		if (!onlySpaces(buf)) {
			return 1;
		}
	}
	return 0;
}

//   *** удаление пробелов и ; ***
void Server::deleteSpaces(std::string *str) {
	
	size_t i = 0;
	while (isspace((*str)[i])) {
			++i;
	}
	*str = (*str).substr(i, (*str).size());
	i = (*str).size() - 1;
	
	while (isspace((*str)[i]) || (*str)[i] == ';') {
			--i;
	}

	if (i <= (*str).size())
		*str = (*str).substr(0, i + 1);
}

std::string	Server::getArgPass() {
	return _argvPass;
}

std::string	Server::getConfigPass() {
	return _configPass;
}

std::string	Server::getAdminLogin() {
	return _admin;

}
const std::map<std::string, std::string>&	Server::getOperatorsMap() {
	return _operators;
}