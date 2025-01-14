#include "Header.hpp"

/* ================ */
/*		SETTER		*/
/* ================ */

void	Server::setIP(std::string ip)
{
	ip.erase(std::remove(ip.begin(), ip.end(), ' '), ip.end());
	ip.erase(std::remove(ip.begin(), ip.end(), ':'), ip.end());
	std::vector<std::string>	substr_ip = ft_split(ip, '.');

	if (substr_ip.size() != 4
		|| ip.find_first_not_of("0123456789.") != std::string::npos)
		throw Response::ConfigurationFileServer("Wrong IP format");
	for (size_t i = 0; i < substr_ip.size(); i++)
	{
		int result = 0;
		if (!ft_stoi(substr_ip[i], result))
			break ;
		if (!(result >= 0 && result <= 255))
			throw Response::ConfigurationFileServer("IP out of range");
	}
	_ip = ip;
}

void	Server::setListen(std::string line)
{
	std::string	port;
	int			int_port = 0;
	size_t		pos = line.find("listen");
	size_t		pos2 = line.find(":");
	size_t		len = strlen("listen");

	line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
	line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
	port = line.substr((pos2 != std::string::npos) ? pos2 : pos + len, line.length());
	if (pos2 != std::string::npos)
		setIP(line.substr(pos + len, pos2 - (pos + len)));
	if (port.empty())
		throw Response::ConfigurationFileServer("Port is empty");
	if (!ft_stoi(port.c_str(), int_port))
		throw Response::ConfigurationFileServer("Port is not a number");
	if (int_port < 0 || int_port > 65535)
		throw Response::ConfigurationFileServer("Port out of range");
	_port = port;
}

void	Server::setServerName(std::string line)
{
	size_t		pos = line.find("server_name");
	size_t		len = strlen("server_name");
	std::string	server_name = line.substr(pos + len, line.length() - (pos + len));
	server_name.erase(std::remove(server_name.begin(), server_name.end(), ' '), server_name.end());
	server_name.erase(std::remove(server_name.begin(), server_name.end(), '\t'), server_name.end());
	bool	is_ip = server_name.find_first_of("0123456789") != std::string::npos;
	_server_name = server_name;

	if (is_ip)
	{
		std::vector<std::string>	substr_ip = ft_split(_server_name, '.');
		if (!_ip.empty())
			throw Response::ConfigurationFileServer("Server name is an IP and IP is already set");
		if (substr_ip.size() != 4
			|| _server_name.find_first_not_of("0123456789.") != std::string::npos)
			throw Response::ConfigurationFileServer("Wrong IP format");
		for (size_t i = 0; i < substr_ip.size(); i++)
		{
			int result = 0;
			if (!ft_stoi(substr_ip[i], result))
				break ;
			if (!(result >= 0 && result <= 255))
				throw Response::ConfigurationFileServer("IP out of range");
		}
		_ip = _server_name;
		return ;
	}
}

void	Server::setRoot(std::string line)
{
	size_t		pos = line.find("root");
	size_t		len = strlen("root");
	std::string	root = line.substr(pos + len, line.length() - (pos + len));

	root.erase(std::remove(root.begin(), root.end(), ' '), root.end());
	root.erase(std::remove(root.begin(), root.end(), '\t'), root.end());
	if(root.at(root.size() - 1) != '/')
		root = root + "/";
	if(root.at(0) != '.')
		root = "." + root;
	_root = root;
}

void	Server::setMaxBody(std::string line)
{
	size_t		pos = line.find("client_max_body_size");
	size_t		len = strlen("client_max_body_size");
	std::string	maxBody = line.substr(pos + len, line.length() - (pos + len));

	maxBody.erase(std::remove(maxBody.begin(), maxBody.end(), ' '), maxBody.end());
	maxBody.erase(std::remove(maxBody.begin(), maxBody.end(), '\t'), maxBody.end());
	if(maxBody.find_first_not_of("0123456789kmKM") != std::string::npos)
		throw Response::ErrorMaxBody();
	size_t pos_character = maxBody.find_first_of("kKmM");
	if (pos_character != std::string::npos)
	{
		std::string caracter = maxBody.substr(pos_character, 1);
		if(caracter == "k" || caracter == "K")
			maxBody.replace(pos_character, 1, "000");
		else
			maxBody.replace(pos_character, 1, "000000");
	}
	_maxBody = maxBody;
}

void	Server::setIndex(std::string line)
{
	size_t		pos = line.find("index");
	size_t		len = strlen("index");
	std::string	index = line.substr(pos + len, line.length() - (pos + len));

	index.erase(std::remove(index.begin(), index.end(), ' '), index.end());
	index.erase(std::remove(index.begin(), index.end(), '\t'), index.end());
	_index = index;
}

void	Server::setAutoIndex(std::string line)
{
	size_t		pos = line.find("autoindex"); 
	size_t		len = strlen("autoindex");
	std::string	autoindex = line.substr(pos + len, line.length() - (pos + len));

	autoindex.erase(std::remove(autoindex.begin(), autoindex.end(), ' '), autoindex.end());
	autoindex.erase(std::remove(autoindex.begin(), autoindex.end(), '\t'), autoindex.end());
	_autoindex = autoindex;
}

void	Server::setRedir(std::string line)
{
	size_t		pos = line.find("return");
	size_t		len = strlen("return");
	std::string	code = line.substr(pos + len, 4);
	std::string	domain = line.substr(pos + len + 4, line.length() - (pos + len));

	domain.erase(std::remove(domain.begin(), domain.end(), ' '), domain.end());
	domain.erase(std::remove(domain.begin(), domain.end(), '\t'), domain.end());
	code.erase(std::remove(code.begin(), code.end(), ' '), code.end());
	code.erase(std::remove(code.begin(), code.end(), '\t'), code.end());
	if(getRedir().size())
		_redir.erase(this->_redir.begin());
	_redir.insert(std::make_pair(code, domain));
}

void Server::setErrorPage(std::string line)
{
	line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
	line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
	size_t pos = line.find("error_page");
	size_t len = strlen("error_page");
	std::string code = line.substr(pos + len, 3).c_str();
	std::string errorFile = line.substr(pos + len + 4, line.length());

	_errorPage.insert(std::make_pair(code, errorFile));
}

void Server::setCgiPath(std::string line)
{
	size_t		pos = line.find("cgi_path ");
	size_t		len = strlen("cgi_path ");
	std::string	trimLine = line.substr(pos + len, line.length() - (pos + len));
	std::vector<std::string>	substr = ft_split(trimLine, ' ');

	for (size_t i = 0; i < substr.size(); i++)
	{
		std::vector<std::string>	map = ft_split(substr[i], ':');
		std::string					language = map[0];
		std::string					path = map[1];
		_cgiPath.insert(std::make_pair(language, path));
	}
}

void Server::setAllowedMethods(std::string line)
{
	size_t pos = line.find("allowed_methods");
	size_t len = strlen("allowed_methods");
	std::string methods = line.substr(pos + len, line.length() - (pos + len));

	methods.erase(std::remove(methods.begin(), methods.end(), ' '), methods.end());
	methods.erase(std::remove(methods.begin(), methods.end(), '\t'), methods.end());
	if (methods.find_first_not_of("GETPOSTDELETE") != std::string::npos)
		throw Response::ConfigurationFileServer("Allowed methods are not GET, POST or DELETE");
	if (methods.find("GET") != std::string::npos)
		_allowedMethods.push_back("GET");
	if (methods.find("POST") != std::string::npos)
		_allowedMethods.push_back("POST");
	if (methods.find("DELETE") != std::string::npos)
		_allowedMethods.push_back("DELETE");
}

void	Server::setLocation(Location &location)
{
	_location.push_back(location);
}

void	Server::setSocketFd(int sockfd)
{
	socketfd.insert(sockfd);
}

/* ================ */
/*		GETTER		*/
/* ================ */

std::string	Server::getPort() const { return (_port); }

std::string	Server::getIP() const { return (_ip); }

std::string	Server::getServerName() const { return (_server_name); }

std::string	Server::getRoot() const { return (_root); }

std::string	Server::getMaxBody() const { return (_maxBody); }

std::string	Server::getIndex() const { return (_index); }

std::string	Server::getAutoIndex() const { return (_autoindex); }

std::map<std::string,std::string>	&Server::getRedir() { return (_redir); }

std::map<std::string,std::string>	&Server::getErrorPage() { return (this->_errorPage); }

std::map<std::string,std::string>	&Server::getCgiPath() { return (this->_cgiPath); }

std::vector<std::string>			&Server::getAllowedMethods() { return (_allowedMethods); }

std::vector<Location>	&Server::getLocation() { return (_location); }

std::vector<void (Location::*)(std::string)>	Server::getLocationFunctions(void) { return (_locationFunctions); }

int	Server::getKeywordsSize(void) { return (_locKeywordsSize); }

std::vector<std::string>	Server::getKeywords(void)
{
	std::vector<std::string>	locKeywords;

	for (int i = 0; i < _locKeywordsSize; i++)
		locKeywords.push_back(_locKeywords[i]);
	return (locKeywords);
}

/* ================ */
/*		FILL		*/
/* ================ */

static void findKeywordIndex(const std::string& line, Location	&location, const std::vector<std::string>& keywords, const std::vector<void (Location::*)(std::string)> &functions)
{
	int	kSzie = static_cast<int> (keywords.size());
	int	i = 0;

	while (i < kSzie && line.find(keywords[i]) == std::string::npos)
		i++;
	if (i < kSzie)
		(location.*functions[i])(line);
	else if (i > (kSzie - 1) && line.find("}") == std::string::npos && !line.empty())
		throw Response::ConfigurationFileServer("Unknown attribute: " + line);
}

/**
 * @brief	Gonna Fill all Location data info detected in a location instance
 * 			It will get the path of the location first, and then get all the information.
 * 			In last add my Location to my Server.
 * @note	It will erase spaces between key and value.
 * @author	Ozan, if you need to ask something...
*/
void	Server::fillLocation(std::ifstream &file, std::string line, std::vector<Location> &locations)
{
	Location	location;
	bool		bracket = false;

	if (line.find("{") != std::string::npos)
		bracket = true;
	location.setPath(line);
	checkLocationPath(location, locations);
	while(getline(file, line) && ConfigParser::parseLine(line))
	{
		if (line.find("{") != std::string::npos)
		{
			bracket = true;
			continue;
		}
		findKeywordIndex(line, location, getKeywords(), getLocationFunctions());
		if (line.find("}") != std::string::npos)
		{
			if (bracket == false)
				throw Response::ConfigurationFileLocation("Missing '{' in location block");
			location.checkNotEmptys();
			return setLocation(location);
		}
	}
	throw Response::ConfigurationFileLocation("Missing '}'");
}

void	Server::functionConfig(void)
{
	_locKeywords.push_back("root");
	_locKeywords.push_back("client_max_body_size");
	_locKeywords.push_back("autoindex");
	_locKeywords.push_back("index");
	_locKeywords.push_back("return");
	_locKeywords.push_back("error_page");
	_locKeywords.push_back("cgi_path");
	_locKeywords.push_back("allowed_methods");

	_locationFunctions.push_back(&Location::setRoot);
	_locationFunctions.push_back(&Location::setMaxBody);
	_locationFunctions.push_back(&Location::setAutoIndex);
	_locationFunctions.push_back(&Location::setIndex);
	_locationFunctions.push_back(&Location::setRedir);
	_locationFunctions.push_back(&Location::setErrorPage);
	_locationFunctions.push_back(&Location::setCgiPath);
	_locationFunctions.push_back(&Location::setAllowedMethods);
}

/* ================ */
/*		DEBUG		*/
/* ================ */

void	Server::printConfig()
{
	std::vector<Location>::iterator	itbeg = _location.begin();
	std::vector<Location>::iterator	itend = _location.end();

	if (!getPort().empty())
		std::cout << "listen\t\t" YELLOW << getPort() << RESET << std::endl;
	if (!getIP().empty())
		std::cout << "ip\t\t" YELLOW << getIP() << RESET << std::endl;
	if (!getServerName().empty())
		std::cout << "server_name\t" YELLOW << getServerName() << RESET << std::endl;
	if (!getRoot().empty())
		std::cout << "root\t\t" YELLOW <<getRoot() << RESET << std::endl;
	if (!getMaxBody().empty())
		std::cout << "maxBody\t\t" YELLOW << getMaxBody() << RESET << std::endl;
	if (!getIndex().empty())
		std::cout << "index\t\t" YELLOW << getIndex() << RESET << std::endl;
	if (!getAutoIndex().empty())
		std::cout << "autoindex\t" YELLOW << getAutoIndex() << RESET << std::endl;
	if (getErrorPage().size())
	{
		std::map<std::string, std::string>::iterator it = getErrorPage().begin();
		std::cout << "error_page\t" YELLOW;
		for (; it != getErrorPage().end(); it++)
			std::cout << it->first << " " << it->second << " ";
		std::cout << RESET << std::endl;
	}
	if (getCgiPath().size())
	{
		std::map<std::string, std::string>::iterator it = getCgiPath().begin();
		std::cout << "cgi_path\t" YELLOW;
		for (; it != getCgiPath().end(); it++)
			std::cout << it->first << ":" << it->second << " ";
		std::cout << RESET << std::endl;
	}
	if (getRedir().size()) 
		std::cout << "return\t\t" YELLOW << getRedir().begin()->first << " " << getRedir().begin()->second << RESET << std::endl;
	if (_allowedMethods.size())
	{
		std::cout << "allowed_methods\t";
		for (size_t i = 0; i < _allowedMethods.size(); i++)
			std::cout << YELLOW << _allowedMethods[i] << " " << RESET;
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (int i = 1; itbeg != itend; ++itbeg, ++i)
		std::cout << BWHITE "\tLocation " << i << RESET << std::endl, (itbeg)->printConfig();

}

