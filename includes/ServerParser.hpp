#ifndef SERVERPARSER_HPP
# define SERVERPARSER_HPP

# include "Header.hpp"

class Location;
class ConfigParser;

#define MAX_EVENTS 100

typedef struct s_cgi
{
	int		cgifd;
	int		cgipid;
	int		parentsocket;
	time_t	cgiTimeout;
}t_cgi;

typedef struct s_serverData
{
	int									sockfd;
	std::string 						port;
	std::string 						ip;
	std::string							server_name;
	std::string							path;
	std::string							maxBody;
	std::string							index;
	std::string							autoindex;
	std::string							buffer;
	std::string							header;
	std::string							body;
	t_cgi								*cgi;
	bool								isDownload;
	void								*isCgi;
	bool 								isHeader;
	int									contentLength;
	std::vector<std::string>			envCgi;
	std::map<std::string, std::string>	errorPage;
	std::map<std::string, std::string>	cgiPath;
	std::map<std::string, std::string>	redir;
	std::vector<Location>				location;
	std::vector<std::string>			requestAllow;
}	t_serverData;

/**
 * @brief	Class representing the main HTTP server.
 * @note	Responsible for listening on one or more ports for incoming connections.
 * 			Accepts connections from clients and redirects them to a request handler.
 * @details	Responsible for server configuration and management of virtual hosts.
*/
class Server
{
	private:

		std::string	_port;
		std::string	_ip;
		std::string	_server_name;
		std::string	_root;
		std::string	_maxBody;
		std::string	_autoindex;
		std::string	_index;

		std::map<std::string, std::string>	_redir;
		std::map<std::string, std::string>	_errorPage;
		std::map<std::string, std::string>	_cgiPath;
		std::vector<std::string>			_allowedMethods;
		std::vector<Location>				_location;
		std::set<int>						socketfd;
		t_serverData						*data;

		// std::vector<std::string>						_locKeywords[8];
		std::vector<std::string>						_locKeywords;
		std::vector<void (Location::*)(std::string)>	_locationFunctions;
		static int const								_locKeywordsSize = 8;
	
	public:

		void	setIP(std::string port);
		void	setServerName(std::string server_name);
		void	setListen(std::string line);
		void	setRoot(std::string line);
		void	setMaxBody(std::string line);
		void	setAutoIndex(std::string line);
		void	setIndex(std::string line);
		void	setRedir(std::string line);
		void	setErrorPage(std::string line);
		void 	setCgiPath(std::string line);
		void	setAllowedMethods(std::string line);
		void	setLocation(Location &location);
		void	setSocketFd(int sockfd);

		std::string							getPort() const;
		std::string							getIP() const;
		std::string							getServerName() const;
		std::string							getRoot() const;
		std::string							getMaxBody() const;
		std::string							getAutoIndex() const;
		std::string							getIndex() const;
		std::map<std::string,std::string>	&getRedir();
		std::map<std::string,std::string>	&getErrorPage();
		std::map<std::string,std::string>	&getCgiPath();
		std::vector<std::string>			&getAllowedMethods();
		std::vector<Location>				&getLocation();

		std::vector<std::string>						getKeywords(void);
		std::vector<void (Location::*)(std::string)>	getLocationFunctions(void);
		int												getKeywordsSize(void);

		void	fillLocation(std::ifstream &file, std::string line, std::vector<Location> &location);
	
		void				functionConfig(void);
		void				createListenAddr(ConfigParser &config);
		void				configuringNetwork(std::vector<Server>::iterator &itbeg, ConfigParser &config, int &epoll_fd);
		struct epoll_event	fillEpoolDataInfo(int &client_fd, t_serverData *info);
		struct epoll_event	fillEpoolDataIterator(int sockfd, std::vector<Server>::iterator itbeg, ConfigParser &config);
		void				setupSocket(int &sockfd, struct sockaddr_in &addr, std::vector<Server>::iterator itbeg);

		void	printConfig();
};

#endif