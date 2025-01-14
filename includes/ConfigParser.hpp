#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include "Header.hpp"

/**
 * @brief	Class responsible for parsing the configuration file for the server.
 * @note	This class reads and stores configuration parameters such as server port, root directory,
 * 			and virtual server (host) settings. It allows for flexible server configuration and 
 * 			manages the setup of multiple virtual hosts.
 * @details	Similar functionality to "httpConfig.hpp", designed to handle HTTP server configurations.
 */
class ConfigParser
{
	private:
		std::vector<Server>	_servers;
		std::string			_path;

		std::vector<std::string>					_keywords;
		std::vector<void (Server::*)(std::string)>	_serverFunctions;
		static int const							_keywordsSize = 10;
		std::vector<t_serverData *>					listData;

	public:
		ConfigParser(void);
		ConfigParser(char *path);
		~ConfigParser(void);

		void		addServer(Server &server);
		void		setListData(t_serverData *data);

		std::vector<Server>			&getServers(void);
		std::vector<std::string>	getKeywords(void);
		std::vector<t_serverData *>	getListData(void) const;

		static void	rmComments(std::string &line);
		static void	checkSemicolon(std::string &line);
		static bool	parseLine(std::string &line);
		void		handleNewServer(std::string& line, std::ifstream& file, std::vector<Server>& servers, bool httpBlock);
		void		parseConfig(std::vector<Server> &servers);
		void		getServerAttributs(std::ifstream& file, Server &server, bool bracket);
		void		functionConfig(void);
		bool		isFileEmpty(const std::string &filePath);
		void		checkServerAttributs(Server &server, std::vector<Server> &servers);
		void		printConfig(void);
};

# endif /* CONFIGPARSER_HPP */