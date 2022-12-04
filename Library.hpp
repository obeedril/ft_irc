#ifndef LIBRARY_HPP
# define LIBRARY_HPP

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

// # include "Location.hpp"
// # include "ServerPairs.hpp"
// # include "ConfigTokens.hpp"
// # include "CGI.hpp"
// # include "Parser.hpp"
// # include "Response.hpp"
// # include "Request.hpp"
// # include "Http.hpp"
// # include "Server.hpp"
// # include "Core.hpp"

# define BUFSIZE 1024
// # define PORT 8080
# define ADDRESS INADDR_ANY
// # define DEFAULT_CONFIG "configs/default.conf"
// # define EXTENSION ".conf"
// # define PATH_PY "/usr/local/bin/python3"
// # define PATH_SH "/bin/bash"
// # define REQUEST_METHOD "" //The method used to make the request. The most common methods are GET and POST.
// # define QUERY_STRING "" // The URL-encoded information that is sent with GET method request.
// # define CONTENT_TYPE "" // The data type of the content. Used when the client is sending attached content to the server. For example, file upload.
// # define CONTENT_LENGTH "" // The length of the query information. It is available only for POST requests.
// # define HTTP_COOKIE "" // Returns the set cookies in the form of key & value pair.
# define COUNT_CMD 3
# define COUNT_ENV 7

#endif
