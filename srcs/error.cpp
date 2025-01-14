#include "Header.hpp"

/**
 * @brief Close all file descriptors
 * @note Except 0, 1 and 2. dir is used to read the /proc/self/fd directory.
 * @details a return was deleted..
*/
void	closeAllFileDescriptors(void)
{
	int				i = 2;
	DIR				*dir = opendir("/proc/self/fd");
	struct dirent	*entry;

	while(++i < 1024)
		close(i);
	if (dir == NULL)
	{
		int maxFd = sysconf(_SC_OPEN_MAX);
		for (int fd = 0; fd < maxFd; ++fd)
			close(fd);
		return;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.') 
			continue;
		int fd = atoi(entry->d_name);
		if (fd > 2)
			close(fd);
	}
	closedir(dir);
}

void errorCloseEpollFd(int &epoll_fd, int errCode)
{
	if(errCode != 7)
		close(epoll_fd);
	if(errCode == 1)
		std::cout << "Error epoll waiting: ";
	else if(errCode == 2)
		std::cout << "Error to create socket: ";
	else if(errCode == 3)
		std::cout << "Error adding option to socket: ";
	else if(errCode == 4)
		std::cout << "Error controlling socket: ";
	else if(errCode == 5)
		std::cout << "Error receiving data: ";
	else if(errCode == 6)
		std::cout << "Error accepting new client: ";
	else if(errCode == 7)
		std::cout << "Error creating epoll instance: ";
	closeAllFileDescriptors();
	throw Response::ErrorEpoll("close epoll fd" + std::string(strerror(errno)));
}

void	notFoundFavicon(t_serverData *data)
{
	std::string contentFile = readFile(data->path + "error/404.html", data);

	std::string response = "HTTP/1.1 404 Not Found\r\n"
							"Content-Type: text/html\r\n"
							"Content-Length: " + to_string(contentFile.size()) + "\r\n"
							"Connection: keep-alive\r\n\r\n"
							+ contentFile;
	//send response
	if(send(data->sockfd, response.c_str(), response.size(), 0) < 0)
		errorPage(std::string(strerror(errno)), "500", data);
	throw Response::ErrorRequest("favicon.ico not found");
}

/**
 * @brief Read an error file
 * @param path The path to the error file
 * @param data The server data
 * @return The content of the error file
*/
static void	choose_error_page(std::string error, t_serverData *data, std::string path)
{
	if (error == "400") Response::sendResponse("400 Bad Request", "text/html", read_error_file(path, data), data);
	else if (error == "403") Response::sendResponse("403 Forbidden", "text/html", read_error_file(path, data), data);
	else if (error == "404") Response::sendResponse("404 Not Found", "text/html", read_error_file(path, data), data);
	else if (error == "405") Response::sendResponse("405 Method Not Allowed", "text/html", read_error_file(path, data), data);
	else if (error == "413") Response::sendResponse("413 Content Too Large", "text/html", read_error_file(path, data), data);
	else if (error == "500") Response::sendResponse("500 Internal Server Error", "text/html", read_error_file(path, data), data);
	else if (error == "501") Response::sendResponse("501 Not Implemented", "text/html", read_error_file(path, data), data);
}

/**
 * @brief Will find the correct error page to display for a location
 * @param error The error code
 * @param data The server data
 * @param loc The location
*/
void chooseErrorPageForLocation(const std::string &error, t_serverData *data, Location &loc)
{
	std::string uri = data->buffer.substr(data->buffer.find('/') + 1, data->buffer.find(" HTTP") - data->buffer.find('/'));
	if (uri.find(loc.getPath()) != std::string::npos && loc.getErrorPage().find(error) != loc.getErrorPage().end())
	{
		std::string second = loc.getErrorPage().find(error)->second;
		std::string path = !loc.getRoot().empty() ? loc.getRoot() + second : data->path + second;
		choose_error_page(error, data, path);
	}
}

/**
 * @brief Will find the correct default error page to display
 * @param error The error code
 * @param data The server data
 * @details For 413 error code, it will call contentTooLarge function.
*/
void useDefaultErrorPage(const std::string &error, t_serverData *data)
{
	if (error == "400") Response::sendResponse("400 Bad Request", "text/html", read_error_file("./www/error/400.html", data), data);
	else if (error == "403") Response::sendResponse("403 Forbidden", "text/html", read_error_file("./www/error/403.html", data), data);
	else if (error == "404") Response::sendResponse("404 Not Found", "text/html", read_error_file("./www/error/404.html", data), data);
	else if (error == "405") Response::sendResponse("405 Method Not Allowed", "text/html", read_error_file("./www/error/405.html", data), data);
	else if (error == "413") Response::sendResponse("413 Content too large", "text/html", read_error_file("./www/error/413.html", data), data);
	else if (error == "500") Response::sendResponse("500 Internal Server Error", "text/html", read_error_file("./www/error/500.html", data), data);
	else if (error == "501") Response::sendResponse("501 Not Implemented", "text/html", readFile("./www/error/501.html", data), data);
	else if (error == "504") Response::sendResponse("504 Gateway Timeout", "text/html", readFile("./www/error/504.html", data), data);
}

/**
 * @brief Display an error page
 * @note location error pages are checked first, then server error pages, then default error pages.
 * @param error The error code
 * @param data The server data
 * @details This function will display an error page based on the error code.
*/
void errorPage(std::string msg, std::string error, t_serverData *data)
{
	if (!msg.empty())
		std::cout << msg << std::endl;
	// Check location error pages first
	for (std::vector<Location>::iterator it = data->location.begin(); it != data->location.end(); ++it)
	{
		// it->setPath1(it->getPath().substr(1));	// Strip leading '/'
		if (!it->getPath().empty())
			chooseErrorPageForLocation(error, data, *it);
	}
	// Check server error pages
	if(data->errorPage.size())
	{
		std::map<std::string, std::string>::iterator it = data->errorPage.find(error);
		if(it != data->errorPage.end())
		{
			std::string path = data->path + it->second;
			choose_error_page(error, data, path);
		}
	}
	// check default error pages
	useDefaultErrorPage(error, data);
}
