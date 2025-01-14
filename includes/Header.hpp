#ifndef HEADER_HPP
# define HEADER_HPP

/* ================ */
/*	   INCLUDES 	*/
/* ================ */

// standard library C++
#include <string>				// Chain manipulation
#include <vector>				// Vector container
#include <map>
# include <set>					// Map Container
#include <exception>			// Exceptions handler
#include <stdexcept>			// Standard exceptions
#include <utility>				// Utilitaries
#include <cstddef>				// Size type

#include <iostream>				// for cout
#include <sstream>				// for les flux de chaînes
#include <fstream>				// std::ifstream
#include <algorithm>			// for erase()

// System library / C
#include <dirent.h>
#include <limits.h>				// For auto indexing
#include <sys/stat.h>			// For auto indexing
#include <sys/socket.h>			// for socket functions
#include <sys/wait.h>
#include <netinet/in.h>			// for sockaddr_in
#include <cstdlib>				// for exit() et EXIT_FAILURE
#include <csignal>				// for exit() et EXIT_FAILURE
#include <errno.h>				// for errno
#include <string.h>				// for strerror
#include <unistd.h>				// for close
#include <arpa/inet.h>			// for inet_ntoa
#include <sys/epoll.h>
#include <fcntl.h>
#include <netdb.h>

// Class header
#include "ServerParser.hpp"		// Class for Server
#include "ConfigParser.hpp"		// Class for Parsing
#include "Location.hpp"			// Class for Locations
#include "Request.hpp"			// Classe for Request
#include "Response.hpp"			// Class for Response
#include "Cookie.hpp"
#include "GlobalLinkList.hpp"


/* ================ */
/*		COLORS		*/
/* ================ */

// Reset
#define RESET		"\e[0m"

// Regular Colors
#define BLACK		"\e[0;30m"
#define RED			"\e[0;31m"
#define GREEN		"\e[0;32m"
#define YELLOW		"\e[0;33m"
#define BLUE		"\e[0;34m"
#define MAGENTA		"\e[0;35m"
#define CYAN		"\e[0;36m"
#define WHITE		"\e[0;37m"

// Bold Colors
#define BBLACK		"\e[1;30m"
#define BRED		"\e[1;31m"
#define BGREEN		"\e[1;32m"
#define BYELLOW		"\e[1;33m"
#define BBLUE		"\e[1;34m"
#define BMAGENTA	"\e[1;35m"
#define BCYAN		"\e[1;36m"
#define BWHITE		"\e[1;37m"

// Background Colors
#define ON_BLACK	"\e[40m"
#define ON_RED		"\e[41m"
#define ON_GREEN	"\e[42m"
#define ON_YELLOW	"\e[43m"
#define ON_BLUE		"\e[44m"
#define ON_MAGENTA	"\e[45m"
#define ON_CYAN		"\e[46m"
#define ON_WHITE	"\e[47m"

// Values
#define TIME_OUT_CGI_MS 10000


/* ================ */
/*		CLASS		*/
/* ================ */

// Class Prototypes
class Client;
class ConfigParser;
class location;
class Request;
class RequestHandler;
class Response;
class Server;
class Cookie;

/* ================ */
/*	  FUNCTIONS 	*/
/* ================ */

// main.cpp
int				main(int ac, char **av);

// autoindex.cpp
bool						isDirectory(const std::string& path);
std::vector<std::string>	listDirectory(const std::string& directory);
std::string					generateAutoIndexPage(const std::string directory, const std::vector<std::string>& files);

// CGIHandler.cpp
void check_timeout_cgi(t_serverData *info, std::map<int, t_serverData*> &fdEpollLink, struct epoll_event *events, int i, int epoll_fd);
void read_cgi(t_serverData *data, struct epoll_event *events, int i, int epoll_fd);
void HandleCgiRequest(std::string uri, t_serverData *&data, std::map<int, t_serverData*> &fdEpollLink, std::string code);


// cookie.cpp
std::string	newSessionCookie(std::map<std::string, std::string> values,Cookie &cookie, t_serverData *data);
std::string	manageDate(time_t current_time);
bool		check_cookie_validity(Cookie &cookie, std::string id);
std::string	get_cookie_id(std::string buffer);

// delete.cpp
void	deleteRequest(std::string &path, t_serverData *data);
void	displayDeletePage(std::string path, t_serverData *data);
void	parseAndDeleteRequest(std::string buffer, t_serverData *data, std::string typeRequest, std::map<int, t_serverData*> &fdEpollLink);

// error.cpp
void	closeAllFileDescriptors(); 
void	errorCloseEpollFd(int &epoll_fd, int errCode);
void	notFoundFavicon(t_serverData *data);
void	errorPage(std::string msg, std::string error, t_serverData *data);

// get.cpp
void		redirRequest(std::string location, int fd, t_serverData *data);
std::string	check_location(std::string &uri, std::string &content, std::vector<Location> &location, t_serverData *data, std::map<int, t_serverData*> &fdEpollLink);
std::string	getContentType(std::string &path, std::string typeRequest, t_serverData * data);
void		checkAccessFile(std::string &code, std::string &filePath, t_serverData *data);
void		parseAndGetRequest(std::string buffer, t_serverData *&data, Cookie &cookie, std::map<int, t_serverData*> &fdEpollLink);

// ParsingUtils.cpp
void		checkLocationPath(Location &location, std::vector<Location> &locations);
bool 		is_download(t_serverData *data, std::string uri);

// post.cpp
void		display_message(std::string path, t_serverData *data);
int			getContentLength(std::string header, t_serverData *data);
void		postRequest(t_serverData *data, Cookie &cookie);

// cookie.cpp
std::string	newSessionCookie(std::map<std::string, std::string> values,Cookie &cookie, t_serverData *data);
std::string	manageDate(time_t current_time);
bool		check_cookie_validity(Cookie &cookie, std::string id);
std::string	get_cookie_id(std::string buffer);
std::string display_user_connection(Cookie &cookie, t_serverData *data, std::string response);

// Response.cpp
std::string	httpGetResponse(std::string code, std::string contentType, std::string content, t_serverData *data, std::string filePath);
std::string	httpGetResponseDownload(std::string code, std::string contentType, std::string content, t_serverData *data);
void		httpPostResponse(std::string code , std::string contentType, std::string content, t_serverData *data, Cookie &cookie, std::string id);

// signals.cpp
void	configureSignals();

// utils.cpp
bool						ft_stoi(const std::string &str, int &result);
std::vector<std::string>	ft_split(const std::string& str, char delimiter);
std::string					readFile(std::string path, t_serverData *data);
std::string					read_error_file(std::string path, t_serverData *data);
bool						isExtension(std::string path);
bool						isExtensionDownload(std::string path);
void						truncate_file(std::string &file, t_serverData *data);
bool						is_keep_alive(std::string &header);
bool						request_allowed(std::string typeRequest, t_serverData *data);
std::string					CGIExtension(std::string path);
bool						is_cgi_extension(std::string path);
void						checkLocationPath(Location &location, std::vector<Location> &locations);
void						removeChar(std::string& str, char c);


template <typename T>
std::string to_string(T value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

#endif