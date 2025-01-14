#include "Header.hpp"

std::string pathLocation(std::string &content, std::string &uri, std::vector<Location>::iterator it, t_serverData *data, std::string root)
{
	std::string filePath;
	// std::string 
	if(!it->getIndex().empty())
	{
		filePath = root + uri + it->getIndex();
		content = readFile(filePath, data);
		return filePath;
	}
	else if(!it->getAutoIndex().empty() && it->getAutoIndex() == "on")
	{
		filePath = root + uri;
		std::vector<std::string> files = listDirectory(filePath);
		content = generateAutoIndexPage(uri, files);
		return root + uri;
	}
	else if(!data->index.empty())
	{
		filePath = root + uri + data->index;
		content = readFile(filePath, data);
		return filePath;
	}
	else if(!data->autoindex.empty() && data->autoindex == "on")
	{
		filePath = root + uri;
		std::vector<std::string> files = listDirectory(filePath);
		content = generateAutoIndexPage(uri, files);
		return (filePath);
	}
	else
	{
		errorPage("", "404", data);
		return("");
	}
}

std::string process_extension_location(std::string &content, std::string base, std::string &uri, t_serverData *data, std::map<int, t_serverData*> &fdEpollLink)
{
	std::string filePath;

	if(is_cgi_extension(uri))
	{
		filePath = base + uri;
		HandleCgiRequest(filePath.c_str(), data, fdEpollLink, "");
	}
	content = readFile(base + uri, data);
	return base + uri;
}

std::string check_location(std::string &uri, std::string &content, std::vector<Location> &location, t_serverData *data, std::map<int, t_serverData*> &fdEpollLink)
{
	std::vector<Location>::iterator it = location.begin();
	std::string filePath;
	if(it == location.end())
		return("");
	//iterate throught my different server location
	while(it != location.end())
	{
		//modify locationPath to remove the first '/'
		it->setPath1(it->getPath().substr(1));
		if(!it->getPath().empty() && uri.find(it->getPath()) != std::string::npos)
		{
			if(it->getRedir().size())
			{
				redirRequest(it->getRedir().begin()->second, data->sockfd, data);
				throw Response::ErrorRequest("Redirection");
			}
			else if(isExtension(uri) || is_cgi_extension(uri))
			{
				if(!it->getRoot().empty())
					return process_extension_location(content, it->getRoot(), uri, data, fdEpollLink);
				else if(!data->path.empty())
					return process_extension_location(content, data->path, uri, data, fdEpollLink);
				else
					errorPage("", "404", data);
			}
			else
			{
				if(!it->getRoot().empty())
					return pathLocation(content, uri, it, data, it->getRoot());
				else
				{
					if(!data->path.empty())
						return pathLocation(content, uri, it, data, data->path);
					else
						errorPage("", "404", data);
				}
			}
		}
		it++;
	}
	return ("");
}

void	checkAccessDir(std::string &code, std::string &dirPath, t_serverData *data)
{
	struct stat	pathStat;
	if(stat(dirPath.c_str(), &pathStat) != 0)
		errorPage("", "404", data);
	else if (!S_ISDIR(pathStat.st_mode))
		errorPage("", "404", data);
	else if (access(dirPath.c_str(), R_OK) != 0)
		errorPage("", "403", data);
	else
		code = "200 OK";
}

void	process_extension(std::string &filePath, std::string &code, std::string uri, std::string buffer, std::string &content, Cookie &cookie, t_serverData *data, std::map<int, t_serverData*> &fdEpollLink)
{
	if(isExtension(uri) || is_cgi_extension(uri))
	{
		filePath = data->path + uri;
		std::string path = data->path + "pages/cookie/connexion.html";
		if (is_cgi_extension(filePath))
		{
			HandleCgiRequest(filePath.c_str(), data, fdEpollLink, code);
		}
		else if(filePath  == path && check_cookie_validity(cookie, get_cookie_id(buffer)))
		{
			redirRequest("/", data->sockfd, data);
			throw Response::responseOk();
		}
		else
		{
			content = readFile(filePath, data);
		}
	}
}

bool is_download(t_serverData *data, std::string uri)
{
	std::string downloadPath = data->path + "upload";
	std::string path = data->path + uri;

	size_t pos = path.find_last_of("/?");

	if(pos !=  std::string::npos)
	{
		//check if i have a filename at the right of the path
		size_t pos1 = path.substr(pos + 1).find("fileName=");
		if(pos1 != std::string::npos)
		{
			return(true);
		}
		else if(downloadPath == path.substr(0, pos))
		{
			return(true);
		}
	}
	return (false);	
}

bool check_download(t_serverData *data, std::string &filePath, std::string uri)
{
	std::string downloadPath = data->path + "upload";
	std::string path = data->path + uri;

	size_t pos = path.find_last_of("/?");

	if(pos !=  std::string::npos)
	{
		//check if i have a filename at the right of the path
		size_t pos1 = path.substr(pos + 1).find("fileName=");
		if(pos1 != std::string::npos)
		{
			pos1 += pos + 10;
			filePath = downloadPath + "/" + path.substr(pos1);
			data->isDownload = true;
			return(true);
		}
		else if(downloadPath == path.substr(0, pos))
		{
			filePath = path;
			data->isDownload = true;
			return(true);
		}
	}
	return (false);
}

void getRequest(std::string &uri, t_serverData *&data, Cookie &cookie, std::string buffer, std::map<int, t_serverData*> &fdEpollLink)
{
	std::vector<Location>location = data->location;
	std::string	content;
	std::string code;
	std::string contentType = getContentType(uri, "GET", data);
	bool autoindex = false;
	// check if I have a location block that match the query
	std::string filePath = check_location(uri, content, data->location, data, fdEpollLink);

	if(filePath.empty())
	{
		if(data->path.empty())
			errorPage("", "403", data);
		// if i have a file to download
		else if(isExtensionDownload(uri) || check_download(data, filePath, uri))
		{
			if(isExtensionDownload(uri))
			{
				filePath = data->path + uri;
			}
			content = readFile(filePath, data);
		}
		else if(isExtension(uri) || is_cgi_extension(uri))
		{
			process_extension(filePath, code, uri, buffer, content, cookie, data, fdEpollLink);
		}
		else if(uri == "pages/deconnexion/")
		{
			std::string id = get_cookie_id(buffer);
			if(!id.empty() && check_cookie_validity(cookie, id))
				cookie.remove_session_id(id);
			redirRequest("/", data->sockfd, data);
			throw Response::responseOk();
		}
		else if (!data->index.empty())
		{
			filePath = data->path + uri + data->index;
			content = readFile(filePath, data);
		}
		else if(!data->autoindex.empty() && data->autoindex == "on")
		{
			autoindex = true;
			filePath = data->path + uri;
			std::vector<std::string> files = listDirectory(filePath);
			content = generateAutoIndexPage(uri, files);
		}
		else
			errorPage("", "403", data);
	}
	if (filePath.find("pages/delete/delete.html") != std::string::npos)
		displayDeletePage("pages/delete/delete.html", data);
	if (filePath.find("pages/post/post.html") != std::string::npos)
		display_message("pages/post/post.html", data);
	if (isDirectory(filePath))
		checkAccessDir(code, filePath, data);
	checkAccessFile(code, filePath, data);
	// add user cookie connection at the end
	if(!autoindex)
		content = display_user_connection(cookie, data, content);
	//get the response
	std::string response = httpGetResponse(code, contentType, content, data, filePath);
	if(send(data->sockfd, response.c_str(), response.size(), 0) < 0)
		errorPage(std::string(strerror(errno)), "500", data);
}

void parseAndGetRequest(std::string buffer, t_serverData *&data, Cookie &cookie, std::map<int, t_serverData*> &fdEpollLink)
{
	std::string path = buffer.substr(buffer.find('/') + 1, buffer.size() - buffer.find('/'));
	path = path.substr(0, path.find(' '));

	if(path.find("favicon.ico") != std::string::npos)
	{
		return notFoundFavicon(data);
	}
	else
		getRequest(path, data, cookie, buffer, fdEpollLink);
}

