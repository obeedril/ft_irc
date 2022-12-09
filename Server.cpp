#include "Library.hpp"

Server::Server(int port, const std::string &password) {
	_port = port;
	_password = password;
	// _configTokens = {SERV_NAME, ADMIN_NAME, ADMIN_PASS, OPERATORS};
	_configTokens.push(SERV_NAME);
	_configTokens.push(ADMIN_NAME);
	_configTokens.push(ADMIN_PASS);
	_configTokens.push(OPERATORS);
};

void Server::parseConfig() {
	std::vector<std::string> config = splitLines(PATH_TO_CONFIG);

// checking

	std::vector<std::string>::iterator start = config.begin();
	std::vector<std::string>::iterator end = config.end();
	while (start < end) {
	std::cout << config[start]<< std::endl;
	start++;
	}
	//
	std::vector<std::string>::iterator end;
	std::vector<std::string>::iterator start;
	bool check = 0;
	end = config.begin();

	while (end < config.end()) {
		if (*end == OPERATORS) {
			start = end;
			end = findEndBrace(config, end);
			// _serverPairs.push_back(ServerPairs());
			// serverPairsInit(_serverPairs.size() - 1,  start, end);
			check = 1;
		}
		++end;
		if (check == 0 && end == config.end()) {
			throw std::runtime_error("Cannot find operators in config");
		}
	}
	config.clear();
	checkInfo();
}

std::vector<std::string> Server::splitLines(std::string path) {	
	// std::vector<std::string> Server::splitLines(char* path) {	
	std::ifstream file(path);
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
	bracesCounter(res);
	checkTokens(res);
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

	if (i <= (*str).size()) // строго меньше??
		*str = (*str).substr(0, i + 1);
}

//   *** проверка общего количества кавычек ***
void Server::bracesCounter(std::vector<std::string> res) {
	    size_t openBrace = 0;
		size_t closeBrace = 0;
		for (size_t i = 0; i < res.size(); ++i) {
		if (res.at(i) == "{")
			++openBrace;
		else if (res.at(i) == "}")
			++closeBrace;
    }
		if (openBrace != closeBrace)
			throw std::runtime_error("Unproper count of braces in config");
}

// // //   *** проверка токенов ***
// // void Server::checkTokens(std::vector<std::string> res) {
// // 	std::vector<std::string>::iterator it;
// // 	std::string str;

// // 	it = res.begin();
// // 	while (it < res.end()) {
// // 		size_t i = 0;
// // 		while ((*it)[i] && !isspace((*it)[i]))
// // 			++i;
// // 		str = (*it).substr(0, i);



// // 		std::cout << "str = " << str << std::endl;

		
// // 		// if (!str.empty() && _configTokens.find(str)->first != str) {
// // 		// 	// std::cout << "str = " << str << std::endl;
// // 		// 	throw std::runtime_error("Unproper token in config");
// // 		// }
// // 		++it;
// 	}
// }

/*


#include "Parser.hpp"
#define EXTENSION ".conf"

FtParser::FtParser(const char *argv) : _configTokens(), _config(argv) {
}

FtParser::~FtParser() {
}

std::vector<ServerPairs>& FtParser::getServers() {
	return _serverPairs;
}

std::vector<u_short>& FtParser::getPorts() {
	return _allPorts;
};
    

//   *** основной метод парсинга ***

void FtParser::parse(std::string argv) {

	if (strlen(EXTENSION) > argv.size() || argv.substr((argv.size() - strlen(EXTENSION)), 
			strlen(EXTENSION)) != EXTENSION)
		throw std::runtime_error("Wrong config file");

	std::vector<std::string> config = splitLines(_config);
	std::vector<std::string>::iterator end;
	std::vector<std::string>::iterator start;
	bool check = 0;
	end = config.begin();

	while (end < config.end()) {
		if (*end == "server") {
			start = end;
			end = findEndBrace(config, end);
			_serverPairs.push_back(ServerPairs());
			serverPairsInit(_serverPairs.size() - 1,  start, end);
			check = 1;
		}
		++end;
		if (check == 0 && end == config.end()) {
			throw std::runtime_error("Cannot find server in config");
		}
	}
	config.clear();
	checkInfo();

/* УДАЛИТЬ! Проверка
	for (std::vector<ServerPairs>::iterator it = _serverPairs.begin() ; it!=_serverPairs.end() ; ++it) {
        std::cout<< "test server name:  " << (*it).getServName() <<  std::endl;
		std::cout<< "test server host:  " << (*it).getHost() <<  std::endl;
		std::cout<< "test server port:  " << (*it).getPort() <<  std::endl;
		std::cout<< "test server root:  " << (*it).getRoot() <<  std::endl;


		std::vector<Location> a = (*it).getLocations();
		for (std::vector<Location>::iterator at = a.begin() ; at!=a.end() ; ++at) {
		std::cout<< "test location name:  " << (*at).getLocationName() <<  std::endl;
		std::cout<< "test location pathcgi:  " << (*at).getLocationPathCgi() <<  std::endl;
		std::cout<< "test location is cgi:  " << (*at).getCgiInLocation() <<  std::endl;
		std::cout<< "test location index:  " << (*at).getLocationIndex() <<  std::endl;
		std::cout<< "test location redir:  " << (*at).getLocationRedirection() <<  std::endl;
		}
		}
}



//   *** поиск закрывающей фигурной скобки***

std::vector<std::string>::iterator FtParser::findEndBrace(std::vector<std::string> &config, 
		std::vector<std::string>::iterator it) {

	if (*(++it) != "{")
		throw std::runtime_error("Invalid syntax in config");

	size_t openBrace = 1;
	size_t closeBrace = 0;

	while (++it < config.end()) {
		if (*it == "}" && (openBrace - closeBrace == 1)) {
			return it;
		}
		else if (*it == "}") {
			++closeBrace;
		}

		else if (*it == "{") {
			++openBrace;
		}
	}
	return it;
}

//   *** проверка данных: чтобы был порт и не было одновременно cgi и редирекшн***

void FtParser::checkInfo(void) {
		for (size_t i = 0; i < _serverPairs.size(); ++i) {
		if (_serverPairs[i].getPort() ==  0) {
			throw std::runtime_error("Invalid parameters: no port");
		}
		for (size_t j = 0; j < _serverPairs[i].getLocations().size(); ++j) {
			if (_serverPairs[i].getLocations()[j].getCgiInLocation() == 1 && 
						_serverPairs[i].getLocations()[j].getLocationRedirection() == 1) {
				throw std::runtime_error("Invalid parameters: location & redirection");
			}
			// if (_serverPairs[i].getLocations()[j].getCgiInLocation() == 0 && _serverPairs[i].getLocations()[j].getIsRedirect() == 0) {
			// 	_serverPairs[i].getLocations()[j].setIsFolder(true);
			// }
		}
}
}

//   ***разделение строчки в конфиге по словам***

std::vector<std::string> FtParser::splitLineOfConfig(std::string token, std::string str) {
	std::vector<std::string> vector;
	size_t i = 0;
	size_t start = str.find(token) + token.size();
	i = start;
	while (i < str.size()) {
		while (isspace(str[i])) {
			++i;
	}
		start = i;
			while (str[i] && !isspace(str[i]))
				++i;
			vector.push_back(str.substr(start, i - start));
	}
	return vector;
}

//   *** инициализация серверов: ищем токены и локейшн***

void FtParser::serverPairsInit(size_t index,  
	std::vector<std::string>::iterator start, std::vector<std::string>::iterator end) {
	
	ConfigTokens data;

	while (++start < end) {
		for (size_t i = 0; i < data.serverData.size(); ++i) {
			if (!(*start).find(data.serverData[i])) {
					chooseTokenInConfig(data.serverData[i], *start, index, i);
			}
			else if (!(*start).find("location")) {
				start = locationInit(index,  start, end);
			}
		}
	}
}

//   *** инициализация локейшнов ***
std::vector<std::string>::iterator FtParser::locationInit(size_t index,  
	std::vector<std::string>::iterator start, std::vector<std::string>::iterator end) {

	ConfigTokens data;
	// std::vector<std::string> vector;
	std::vector<std::string>::iterator beginLocation = start;

	_serverPairs[index].getLocations().push_back(Location());
	size_t iLocation = _serverPairs[index].getLocations().size() - 1;

	while (start < end) {
		size_t i = 0;
		while ((*start)[i]) {
			if ((*start)[i] == '}') {
				end = start;
				break ;
			}
			++i;
		}
			++start;
	}

	while (beginLocation < end) {
		for (size_t i = 0; i < data.locationData.size(); ++i) {
			if (!(*beginLocation).find(data.locationData[i])) {
				chooseTokenInLocation(data.locationData[i], *beginLocation, 
						_serverPairs[index].getLocations()[iLocation], i);
			}
		}
		++beginLocation;
	} 
	return beginLocation; 
}


//   *** разводящая функция для локейшнов ***

void FtParser::chooseTokenInLocation(std::string token, std::string str, Location& location, size_t num) {
		switch (num) {
			case 0:
				findLocationName(str, token, location);
				break;
			case 1:
				findLocationMethod(str, token, location);
				break;
			case 2:
				findLocationRoot(str, token, location);
				break;
			case 3:
				findLocationRedirection(str, token, location);
				break;
			case 4:
				findLocationError(str, token, location);
				break;
			case 5:
				findLocationBinPath(str, token, location);
				break;
			case 6:
				findLocationIndex(str, token, location);
				break;
			case 7:
				findLocationUpload(str, token, location);
				break;
			case 8:
				findLocationAutoIndex(str, token, location);
				break;
		}
}
			

//   *** если есть локейшн (url) ***
	void FtParser::findLocationName(std::string str, std::string token, Location& location) {

	std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);

	if (vector.size() != 1 || !location.getLocationName().empty())
		throw std::runtime_error("Invalid syntax of location name");
	location.setLocationName(vector[0]);
	if (vector[0] == "/cgi-bin/")
		location.setIsCgi(1);
	}


//   *** если есть локейшн autoindex ***

	void FtParser::findLocationAutoIndex(std::string str, std::string token, Location& location) {
	
	std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);
	if (vector.size() != 1 || (vector[0] != "on" && vector[0] != "off"))
		throw std::runtime_error("Invalid syntax of autoindex (location)");
	if (vector[0] == "on")
	location.setLocationAutoIndex(1);
	}

//   *** если есть локейшн index ***

	void FtParser::findLocationIndex(std::string str, std::string token, Location& location) {

	std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);
	if (vector.size() != 1 || !location.getLocationIndex().empty())
		throw std::runtime_error("Invalid syntax of index (location)");
	location.setLocationIndex(vector[0]);
	}

//   *** если есть локейшн root ***

	void FtParser::findLocationRoot(std::string str, std::string token, Location& location) {

	std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);
	if (vector.size() != 1 || !location.getLocationRoot().empty())
		throw std::runtime_error("Invalid syntax of root (location)");
	location.setLocationRoot(vector[0]);
	}

//   *** если есть локейшн upload ***

	void FtParser::findLocationUpload(std::string str, std::string token, Location& location) {

	std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);
	if (vector.size() != 1 || !location.getLocationUpload().empty())
		throw std::runtime_error("Invalid syntax of upload (location)");
	location.setLocationUploadPath(vector[0]);
	}

//   *** если есть локейшн redirection ***
	void FtParser::findLocationRedirection(std::string str, std::string token, Location& location) {

	std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);
	if (vector.size() != 2 || location.getCgiInLocation() == 1 || location.getLocationRedirection() == 1)
		throw std::runtime_error("Invalid syntax of redirection (location)");
	location.setLocationRedirection(1);
	location.setLocationIndex(vector[1]);
	int redirectionCode = static_cast<int>(strtod(vector[1].c_str(), 0));
	if ( redirectionCode!= 302)
		throw std::runtime_error("Wrong redirection code (302 only)"); //нужны ли другие коды???
	location.setLocationRedirection(redirectionCode);
	}

//   *** если есть локейшн error ***
	void FtParser::findLocationError(std::string str, std::string token, Location& location) {
		
		std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);
		if (vector.size() < 2)
			throw std::runtime_error("Invalid syntax of error page (location)");
		int code = static_cast<int>(strtod(vector[0].c_str(), 0));
		std::string description = vector[1];
		if (code == 0 || description.empty())
			throw std::runtime_error("Invalid syntax of error page (location)");
		location.setLocationError(code, description);
	}

//   *** если есть локейшн BinPath ***
	void FtParser::findLocationBinPath(std::string str, std::string token, Location& location) {

	std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);
	if (vector.size() != 1 || location.getCgiInLocation() == 0 || !location.getLocationBinPath().empty())
		throw std::runtime_error("Invalid syntax of binpath (location)");
	location.setLocationBinPath(vector[0]);
	}

//   *** если есть локейшн methods ***
	void FtParser::findLocationMethod(std::string str, std::string token, Location& location) {

		std::vector<std::string> vector = FtParser::splitLineOfConfig(token, str);
		if (vector.size() < 1 || vector.size() > 3)
			throw std::runtime_error("Unproper count of methods (location)");
		for (size_t i = 0; i < vector.size(); ++i) {
			if (vector[i] == "GET")
				location.setLocationMethod("GET");
			else if (vector[i] == "POST")
			location.setLocationMethod("POST");
			else if (vector[i] == "DELETE")
				location.setLocationMethod("DELETE");
			else
				throw std::runtime_error("Invalid syntax of method (location)");	
		}
}

//   *** разводящая функция для серверов ***

	void FtParser::chooseTokenInConfig(std::string token, std::string str, size_t index, size_t num) {

		switch (num) {
			case 0:
				findListen(str, token, index);
				break;
			case 1:
				findServerName(str, token, index);
				break;
			case 2:
				findAutoIndex(str, token, index);
				break;
			case 3:
				findRoot(str, token, index);
				break;
			case 4:
				findIndex(str, token, index);
				break;
			case 5:
				findMethod(str, token, index);
				break;
			case 6:
				findBodySize(str, token, index);
				break;
			case 7:
				findError(str, token, index);
				break;
			case 8:
				findUpload(str, token, index);
				break;
		}		
}

//   *** нашли строчку listen: делим на хост и порт ***

	std::vector<std::string> FtParser::splitListen(std::string str) {

	std::vector<std::string> vector;
	size_t i = 0;
	size_t dot = 0;

	while(str[i]) {	
		if (isdigit(str[i])) {
			++i;
		}
		else if (str[i] == '.') {
			++dot;
			if (dot > 3)
				throw std::runtime_error("Invalid syntax in \"listen\": wrong host");
			++i;
		}
		else if (str[i] == ':' && i) {	
			vector.push_back(str.substr(0, i));
			vector.push_back(str.substr(i + 1, str.size()));
			return vector;
		}
		else
			throw std::runtime_error("Invalid syntax in \"listen\": no host");
	}
		vector.push_back(str.substr(0, i));
	return vector;
}

//   *** если есть сервер listen ***
void FtParser::findListen(std::string str, std::string token, size_t index) {

	std::vector<std::string> vector = splitLineOfConfig(token, str);

	if (vector.size() != 1 || _serverPairs[index].getHost() != 0 || _serverPairs[index].getPort() != 0)
		throw std::runtime_error("Invalid syntax of host or port");
	vector = splitListen(vector[0]);
	if (vector.size() > 1) {
		_serverPairs[index].setHost(vector[0]);
		_serverPairs[index].setPort(static_cast<int>(strtod(vector[1].c_str(), 0))); // нужна ли проверка порта на валидность?
	}
	else {
		if (vector[0].find(".") != std::string::npos)
			_serverPairs[index].setPort(PORT);
		else
			_serverPairs[index].setPort(static_cast<int>(strtod(vector[0].c_str(), 0)));
	}
		_allPorts.push_back(_serverPairs[index].getPort());
}

//   *** если есть сервер server_name ***
void FtParser::findServerName(std::string str, std::string token, size_t index) {

		std::vector<std::string> vector = splitLineOfConfig(token, str);

		if (vector.size() != 1 || !_serverPairs[index].getServName().empty())
			throw std::runtime_error("Invalid syntax of server name");
		_serverPairs[index].setServName(vector[0]);
		}

//   *** если есть сервер autoindex ***
	void FtParser::findAutoIndex(std::string str, std::string token, size_t index) {

	std::vector<std::string> vector = splitLineOfConfig(token, str);
	
	if (vector.size() != 1 || (vector[0] != "on" && vector[0] != "off"))
		throw std::runtime_error("Invalid syntax of autoindex");
	if (vector[0] == "on")
	_serverPairs[index].setAutoIndex(1);
	}

//   *** если есть сервер index ***
	void FtParser::findIndex(std::string str, std::string token, size_t index) {

	std::vector<std::string> vector = splitLineOfConfig(token, str);

	if (vector.size() != 1 || !_serverPairs[index].getIndex().empty())
		throw std::runtime_error("Invalid syntax of index");
	_serverPairs[index].setIndex(vector[0]);
	}

//   *** если есть сервер root ***
	void FtParser::findRoot(std::string str, std::string token, size_t index) {

	std::vector<std::string> vector = splitLineOfConfig(token, str);

	if (vector.size() != 1 || !_serverPairs[index].getRoot().empty())
		throw std::runtime_error("Invalid syntax of root");
	_serverPairs[index].setRoot(vector[0]);
	}

//   *** если есть сервер upload ***
	void FtParser::findUpload(std::string str, std::string token, size_t index) {

	std::vector<std::string> vector = splitLineOfConfig(token, str);

	if (vector.size() != 1 || !_serverPairs[index].getUpload().empty())
		throw std::runtime_error("Invalid syntax of upload");
	_serverPairs[index].setUpload(vector[0]);
	}

//   *** если есть сервер bodysize ***
	void FtParser::findBodySize(std::string str, std::string token, size_t index) {

	std::vector<std::string> vector = splitLineOfConfig(token, str);

	if (vector.size() != 1 || _serverPairs[index].getBodySize() != -1)
		throw std::runtime_error("Invalid syntax of max body size");
	if (vector[0][vector[0].size() - 1] == 'M') {
		vector[0] = vector[0].substr(0, vector[0].size() - 1);
		_serverPairs[index].setMaxBodySize(static_cast<int>(strtod(vector[0].c_str(),
				 0)) * 1024 * 1024);
	} else if (isdigit(vector[0][vector[0].size() - 1])) { // только последний?
		_serverPairs[index].setMaxBodySize(static_cast<int>(strtod(vector[0].c_str(),
				 0)) * 1024);
	} else {
		throw std::invalid_argument("Invalid syntax of max body size");
		}
	}

//   *** если есть сервер error ***
	void FtParser::findError(std::string str, std::string token, size_t index) {

	std::vector<std::string> vector = splitLineOfConfig(token, str);

	if (vector.size() < 2)
		throw std::runtime_error("Invalid syntax of error page");
	int code = static_cast<int>(strtod(vector[0].c_str(), 0));
	std::string description = vector[1];
	if (code == 0 || description.empty())
		throw std::runtime_error("Invalid syntax of error page");
	_serverPairs[index].setError(code, description);
	}

//   *** если есть сервер methods ***
	void FtParser::findMethod(std::string str, std::string token, size_t index) {
	std::vector<std::string> vector = splitLineOfConfig(token, str);
	if (vector.size() < 1 || vector.size() > 3)
		throw std::runtime_error("Unproper count of methods");
	for (size_t i = 0; i < vector.size(); ++i) {
		if (vector[i] == "GET")
			_serverPairs[index].setMethod("GET");
		else if (vector[i] == "POST")
			_serverPairs[index].setMethod("POST");
		else if (vector[i] == "DELETE")
			_serverPairs[index].setMethod("DELETE");
		else
			throw std::runtime_error("Invalid syntax of method");
	}
	}

//   *** проверка на пустоту - только пробелы ***
int FtParser::onlySpaces(std::string buf) {

	size_t i = 0;
	while (buf[i]) {
		if (!isspace(buf[i++]))
			return 1;
	}
	return 0;
}

//   *** проверка на пустоту - только пробелы и комменты ***
int FtParser::emptyOrComments(std::string buf) {

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
void FtParser::deleteSpaces(std::string *str) {
	
	size_t i = 0;
	while (isspace((*str)[i])) {
			++i;
	}
	*str = (*str).substr(i, (*str).size());
	i = (*str).size() - 1;
	
	while (isspace((*str)[i]) || (*str)[i] == ';') {
			--i;
	}

	if (i <= (*str).size()) // строго меньше??
		*str = (*str).substr(0, i + 1);
}

//   *** вырезание по фигурным скобкам ***
std::vector<std::string> FtParser::checkBraces(std::string buf) {

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

//   *** проверка токенов ***
void FtParser::checkTokens(std::vector<std::string> res) {
	std::vector<std::string>::iterator it;
	std::string str;

	it = res.begin();
	while (it < res.end()) {
		size_t i = 0;
		while ((*it)[i] && !isspace((*it)[i]))
			++i;
		str = (*it).substr(0, i);
		if (!str.empty() && _configTokens.serverTokens.find(str)->first != str) {
			// std::cout << "str = " << str << std::endl;
			throw std::runtime_error("Unproper token in config");
		}
		++it;
	}
}


//   *** проверка общего количества кавычек ***
void FtParser::bracesCounter(std::vector<std::string> res) {
	    size_t openBrace = 0;
		size_t closeBrace = 0;
		for (size_t i = 0; i < res.size(); ++i) {
		if (res.at(i) == "{")
			++openBrace;
		else if (res.at(i) == "}")
			++closeBrace;
    }
		if (openBrace != closeBrace)
			throw std::runtime_error("Unproper count of braces in config");
}

//   *** проверка открытия файла, построчное считывание и запись в вектор ***
std::vector<std::string> FtParser::splitLines(std::string argv) {	
	std::ifstream file(argv);
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
	bracesCounter(res);
	checkTokens(res);
	return res;
}
*/