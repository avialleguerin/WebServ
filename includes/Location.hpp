#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "Header.hpp"

class Server;

class Location
{
	private:
		std::string	_path;
		std::string	_root;
		std::string	_maxBody;
		std::string	_autoindex;
		std::string	_index;

		std::map<std::string,std::string>	_redir;
		std::map<std::string, std::string>	_errorPage;
		std::map<std::string, std::string>	_cgiPath;
		std::vector<std::string>			_allowedMethods;

	public:
		Location(void);
		~Location(void);

		void	setPath(std::string line);
        void	setPath1(std::string path);
		void	setRoot(std::string line);
		void	setMaxBody(std::string line);
		void	setAutoIndex(std::string line);
		void	setIndex(std::string line);
		void	setRedir(std::string line);
		void	setErrorPage(std::string line);
		void	setCgiPath(std::string line);
		void	setAllowedMethods(std::string line);

		std::string							getPath() const;
		std::string							getRoot() const;
		std::string							getMaxBody() const;
		std::string							getAutoIndex() const;
		std::string							getIndex() const;
		std::map<std::string, std::string>	&getRedir();
		std::map<std::string,std::string>	&getCgiPath();
		std::map<std::string,std::string>	&getErrorPage();
		std::vector<std::string>			&getAllowedMethods();

		void	checkNotEmptys(void);
		void	printConfig(void);
		
};

#endif /* LOCATION_HPP */