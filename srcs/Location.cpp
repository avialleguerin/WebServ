#include "Header.hpp"

Location::Location() {}

Location::~Location() {}

/* ================ */
/*		SETTER		*/
/* ================ */
void	Location::setPath1(std::string path)
{
	path.erase(std::remove(path.begin(), path.end(), ' '), path.end());
	this->_path = path;
}

void	Location::setPath(std::string line)
{
	size_t len = strlen("location");
	size_t pos = line.find("location") + len;
	size_t pos2 = line.find("{");

	std::string path = line.substr(pos, pos2 - pos);

	path.erase(std::remove(path.begin(), path.end(), ' '), path.end());
	path.erase(std::remove(path.begin(), path.end(), '\t'), path.end());
	if (path.empty())
		throw Response::ConfigurationFileLocation("path is empty");
	if (path.at(path.size() - 1) != '/')
		path = path + "/";
	_path = path;
}

void	Location::setRoot(std::string line)
{
	size_t		pos = line.find("root");
	size_t		len = strlen("root");
	std::string	path = line.substr(pos + len, line.length() - (pos + len));

	path.erase(std::remove(path.begin(), path.end(), ' '), path.end());
	if(path.at(path.size() - 1) != '/')
		path = path + "/";
	if(path.at(0) != '.')
		path = "." + path;
	_root = path;
}

void	Location::setMaxBody(std::string line)
{
	size_t		pos = line.find("client_max_body_size");
	size_t		len = strlen("client_max_body_size");
	std::string	maxBody = line.substr(pos + len, line.length() - (pos + len));

	maxBody.erase(std::remove(maxBody.begin(), maxBody.end(), ' '), maxBody.end());
	if(maxBody.find_first_not_of("0123456789kmKM") != std::string::npos)
		throw Response::ConfigurationFileLocation("Error in client_max_body_size");
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

void	Location::setAutoIndex(std::string line)
{
	size_t		pos = line.find("autoindex");
	size_t		len = strlen("autoindex");
	std::string	autoindex = line.substr(pos + len, line.length() - (pos + len));

	autoindex.erase(std::remove(autoindex.begin(), autoindex.end(), ' '), autoindex.end());
	_autoindex = autoindex;
}

void	Location::setIndex(std::string line)
{
	size_t		pos = line.find("index");
	size_t		len = strlen("index");
	std::string	index = line.substr(pos + len, line.length() - (pos + len));

	index.erase(std::remove(index.begin(), index.end(), ' '), index.end());
	_index = index;
}

void	Location::setRedir(std::string line)
{
	size_t		pos = line.find("return");
	size_t		len = strlen("return");
	std::string	code = line.substr(pos + len, 4);
	std::string	domain = line.substr(pos + len + 4, line.length() - (pos + len));

	domain.erase(std::remove(domain.begin(), domain.end(), ' '), domain.end());
	code.erase(std::remove(code.begin(), code.end(), ' '), code.end());
	if(getRedir().size())
		_redir.erase(this->_redir.begin());
	_redir.insert(std::make_pair(code, domain));
}

void	Location::setErrorPage(std::string line)
{
	line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
	size_t pos = line.find("error_page");
	size_t len = strlen("error_page");
	std::string code = line.substr(pos + len, 3).c_str();
	std::string errorFile = line.substr(pos + len + 4, line.length());

	_errorPage.insert(std::make_pair(code, errorFile));
}

void	Location::setCgiPath(std::string line)
{
	size_t		pos = line.find("cgi_path ");
	size_t		len = strlen("cgi_path ");
	std::string	trimLine = line.substr(pos + len, line.length() - (pos + len));
	std::vector<std::string>	substr = ft_split(trimLine, ' ');

	for (size_t i = 0; i < substr.size(); i++)
	{
		std::vector<std::string>	map = ft_split(substr[i], ':');
		if (map.size() != 2)
			throw Response::ConfigurationFileLocation("bad format of cgi_path");
		std::string					language = map[0];
		std::string					path = map[1];
		_cgiPath.insert(std::make_pair(language, path));
	}
}

void	Location::setAllowedMethods(std::string line)
{
	size_t		pos = line.find("allowed_methods");
	size_t		len = strlen("allowed_methods");
	std::string	methods = line.substr(pos + len, line.length() - (pos + len));

	methods.erase(std::remove(methods.begin(), methods.end(), ' '), methods.end());
	if (methods.find_first_not_of("GETPOSTDELETE") != std::string::npos)
		throw Response::ConfigurationFileLocation("Allowed methods are not GET, POST or DELETE");
	if (methods.find("GET") != std::string::npos)
		_allowedMethods.push_back("GET");
	if (methods.find("POST") != std::string::npos)
		_allowedMethods.push_back("POST");
	if (methods.find("DELETE") != std::string::npos)
		_allowedMethods.push_back("DELETE");
}

/* ================ */
/*		GETTER		*/
/* ================ */

std::string Location::getPath() const { return (this->_path); }

std::string Location::getRoot() const { return(this->_root); }

std::string Location::getIndex() const { return (this->_index); }

std::string Location::getMaxBody() const { return (this->_maxBody); }

std::string Location::getAutoIndex() const { return (this->_autoindex); }

std::map<std::string,std::string> &Location::getRedir() { return (this->_redir); }

std::map<std::string,std::string> &Location::getCgiPath() { return (this->_cgiPath); }

std::map<std::string,std::string> &Location::getErrorPage() { return (this->_errorPage); }

std::vector<std::string> &Location::getAllowedMethods() { return (this->_allowedMethods); }

/* ================ */
/*		DEBUG		*/
/* ================ */

void Location::checkNotEmptys(void)
{
	if (getRoot().empty()
		&& getMaxBody().empty()
		&& getAutoIndex().empty()
		&& getIndex().empty()
		&& getRedir().empty()
		&& getErrorPage().empty()
		&& getCgiPath().empty()
		&& getAllowedMethods().empty())
		throw Response::ConfigurationFileLocation("Empty location");
}

void	Location::printConfig()
{
	if (!getPath().empty())
		std::cout << "\tPath\t\t" YELLOW << getPath() << RESET << std::endl;
	if (!getMaxBody().empty())
		std::cout << "\tclient_max_body_size\t\t" YELLOW << getMaxBody() << RESET << std::endl;
	if (!getAutoIndex().empty())
		std::cout << "\tautoindex\t" YELLOW << getAutoIndex() << RESET << std::endl;
	if (!getIndex().empty())
		std::cout << "\tindex\t\t" YELLOW << getIndex() << RESET << std::endl;
	if (getRedir().size())
		std::cout << "\treturn\t\t" YELLOW << getRedir().begin()->first << " " << getRedir().begin()->second << RESET << std::endl;
	if (getErrorPage().size())
	{
		std::map<std::string, std::string>::iterator it = getErrorPage().begin();
		std::cout << "\terror_page\t" YELLOW;
		for (; it != getErrorPage().end(); it++)
			std::cout << it->first << " " << it->second << " ";
		std::cout << RESET << std::endl;
	}
	if (getCgiPath().size())
	{
		std::map<std::string, std::string>::iterator it = getCgiPath().begin();
		std::cout << "\tcgi_path\t" YELLOW;
		for (; it != getCgiPath().end(); it++)
			std::cout << it->first << ":" << it->second << " ";
		std::cout << RESET << std::endl;
	}
	if (getAllowedMethods().size())
	{
		std::cout << "\tallowed_methods\t";
		for (size_t i = 0; i < getAllowedMethods().size(); i++)
			std::cout << YELLOW << getAllowedMethods()[i] << " " << RESET;
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
