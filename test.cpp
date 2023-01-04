
# include <sys/socket.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <iostream>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <netdb.h>
# include <fcntl.h>
# include <poll.h>
# include <vector>
# include <list>
# include <string>
# include <sstream>
# include <fstream>
# include <map>
# include <cstdio>
# include <cstring>
# include <arpa/inet.h>
# include <algorithm>
# include <csignal>
# include <ctime>
# include <iomanip>

#include <cmath> //для робота

int main(int argc, char **argv, char **env) {
	int fd;
	std::ofstream f("weather.html", std::ofstream::out | std::ofstream::trunc);
	//fd = f.;
	pid_t	pid;
	char	**cmd;
	std::string curl("/usr/bin/curl");
	std::string cmd_1("-X");
	std::string cmd_2("GET");
	std::string site("https://p.ya.ru/?via=f");
	std::string redir(">");
	std::string html("weather.html");

	cmd = new char*[6];
	for(int i = 0; i < 6; i++) {
		cmd[i] = NULL;
	}
	cmd[0] = strdup(curl.c_str());
	cmd[1] = strdup(cmd_1.c_str());
	cmd[2] = strdup(cmd_2.c_str());
	cmd[3] = strdup(site.c_str());
	cmd[4] = strdup(redir.c_str());
	cmd[5] = strdup(html.c_str());
	cmd[6] = NULL;
	pid = fork();
	if (pid != 0) {
		waitpid(pid, NULL, 0);
	}
	else if (pid == 0) {
		if (execve(curl.c_str(), cmd, NULL) == -1) {
			std::cout << "\x1b[1;70m" << "> ERROR: Execve fail." << "\n" << "\x1b[0m";
		}
		exit(0);
	}
	for(int i = 0; i < 6; i++) {
		free(cmd[i]);
	}
	delete cmd;
	std::cout << "\x1b[1;70m" << "> SUCCESS." << "\n" << "\x1b[0m";
	// char	tmp[512];
	// sprintf(tmp, "%s %s\n", "BOT",
	// 		": The weather is always Nice.\r\n");
	// 		my_client->setMessage(static_cast<std::string>(tmp));
	return(0);
}