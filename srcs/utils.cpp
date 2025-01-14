#include "Header.hpp"

std::string	CGIExtensions[] = {
	".py",
	".php",
	".pl",
	".sh",
	".rb",
	".tcl",
	".js"
};

int	nbCGIExtensions = 7;

bool is_cgi_extension(std::string path)
{
	int	i = 0;

	while (i < nbCGIExtensions)
	{
		size_t	pos = path.find(CGIExtensions[i]);
		if (pos != std::string::npos)
			return (true);
		i++;
	}
	return (false);
}

std::string CGIExtension(std::string path)
{
	int	i = 0;

	while (i < nbCGIExtensions)
	{
		size_t	pos = path.find(CGIExtensions[i]);
		std::string extension = CGIExtensions[i];
		if (pos != std::string::npos)
			return extension;
		i++;
	}
	return ("");
}

void removeChar(std::string& str, char c)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == c) {
			str.erase(i, 1);
			--i;
		}
	}
}

bool	ft_stoi(const std::string &str, int &result)
{
	bool isNegative = false;
	size_t i = 0;

	while (i < str.length() && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (i < str.length() && (str[i] == '-' || str[i] == '+')) {
		if (str[i] == '-')
			isNegative = true;
		i++;
	}
	while (i < str.length() && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (i < str.length() && (str[i] < '0' || str[i] > '9'))
		return false;
	result = isNegative ? -result : result;
	return true;
}

std::vector<std::string>	ft_split(const std::string& str, char delimiter)
{
	std::vector<std::string> result;
	std::string tmp;

	for (std::string::size_type i = 0; i < str.size(); ++i) {
		if (str[i] == delimiter) {
			result.push_back(tmp);
			tmp.clear();
		} else
			tmp += str[i];
	}
	if (!tmp.empty())
		result.push_back(tmp);

	return result;
}

std::string getContentType(std::string &path, std::string typeRequest, t_serverData * data) 
{
	std::map<std::string, std::string> contentTypes;

	contentTypes.insert(std::pair<std::string, std::string>(".html", "text/html"));
	contentTypes.insert(std::pair<std::string, std::string>(".css", "text/css"));
	contentTypes.insert(std::pair<std::string, std::string>(".js", "application/javascript"));
	contentTypes.insert(std::pair<std::string, std::string>(".png", "image/png"));
	contentTypes.insert(std::pair<std::string, std::string>(".jpg", "image/jpeg"));
	contentTypes.insert(std::pair<std::string, std::string>(".gif", "image/gif"));
	contentTypes.insert(std::pair<std::string, std::string>(".svg", "image/svg+xml"));
	contentTypes.insert(std::pair<std::string, std::string>(".webp", "image/webp"));
	contentTypes.insert(std::pair<std::string, std::string>(".ico", "image/x-icon"));
	contentTypes.insert(std::pair<std::string, std::string>(".pdf", "application/pdf"));
	contentTypes.insert(std::pair<std::string, std::string>(".mp3", "video/mpeg"));
	contentTypes.insert(std::pair<std::string, std::string>(".mp4", "video/mp4"));
	contentTypes.insert(std::pair<std::string, std::string>(".webm", "video/webm"));
	contentTypes.insert(std::pair<std::string, std::string>(".ogg", "video/ogg"));
	contentTypes.insert(std::pair<std::string, std::string>(".doc", "application/msword"));
	contentTypes.insert(std::pair<std::string, std::string>(".docx", "application/msword"));
	contentTypes.insert(std::pair<std::string, std::string>(".xls", "application/vnd.ms-excel"));
	contentTypes.insert(std::pair<std::string, std::string>(".xlsx", "application/vnd.ms-excel"));
	contentTypes.insert(std::pair<std::string, std::string>(".ppt", "application/vnd.ms-powerpoint"));
	contentTypes.insert(std::pair<std::string, std::string>(".pptx", "application/vnd.ms-powerpoint"));
	contentTypes.insert(std::pair<std::string, std::string>(".ppt", "application/vnd.ms-powerpoint"));
	contentTypes.insert(std::pair<std::string, std::string>(".zip", "application/zip"));
	contentTypes.insert(std::pair<std::string, std::string>(".rar", "application/vnd.rar"));
	contentTypes.insert(std::pair<std::string, std::string>(".tar", "application/x-tar"));
	contentTypes.insert(std::pair<std::string, std::string>(".gz", "application/gzip"));
	contentTypes.insert(std::pair<std::string, std::string>(".7z", "application/x-7z-compressed"));
	contentTypes.insert(std::pair<std::string, std::string>(".txt", "text/plain"));
	contentTypes.insert(std::pair<std::string, std::string>(".xml", "application/xml"));
	contentTypes.insert(std::pair<std::string, std::string>(".json", "application/json"));
	contentTypes.insert(std::pair<std::string, std::string>(".csv", "text/csv"));
	contentTypes.insert(std::pair<std::string, std::string>(".py", "text/html"));

	size_t dotPos = path.find_last_of(".");
	if (dotPos != std::string::npos) {
		std::string extension = path.substr(dotPos);
		if (contentTypes.find(extension) != contentTypes.end())
			return contentTypes[extension];
	}
	else
		if((path.size() == 0 || path.at(path.size() - 1) != '/')  && typeRequest != "DELETE" && !is_download(data, path))
			path += "/";
	return "text/html";
}

int getContentLength(std::string header, t_serverData *data)
{
	(void)data;
	std::string content = "Content-Length: ";
	size_t pos = header.find(content);

	if(pos == std::string::npos)
	{
		return (header.size());
	}
	std::string size = header.substr(pos + content.size(), header.size());
	pos = size.find("\n");
	size = size.substr(0, pos);
	int intSize = atoi(size.c_str());
	return(intSize);
}

bool isExtension(std::string path)
{
	if(path.find(".html") == std::string::npos &&
		path.find(".css") == std::string::npos &&
		path.find(".js") == std::string::npos &&
		path.find(".png") == std::string::npos &&
		path.find(".jpg") == std::string::npos &&
		path.find(".gif") == std::string::npos &&
		path.find(".svg") == std::string::npos &&
		path.find(".webp") == std::string::npos &&
		path.find(".ico") == std::string::npos &&
		path.find(".py") == std::string::npos &&
		path.find(".pdf") == std::string::npos &&
		path.find(".mp3") == std::string::npos &&
		path.find(".mp4") == std::string::npos &&
		path.find(".webm") == std::string::npos &&
		path.find(".ogg") == std::string::npos)
		return (false);
	return (true);
}

bool isExtensionDownload(std::string path)
{
	if(path.find(".doc") == std::string::npos &&
		path.find(".docx") == std::string::npos &&
		path.find(".xls") == std::string::npos &&
		path.find(".xlsx") == std::string::npos &&
		path.find(".ppt") == std::string::npos &&
		path.find(".pptx") == std::string::npos &&
		path.find(".zip") == std::string::npos &&
		path.find(".rar") == std::string::npos &&
		path.find(".tar") == std::string::npos &&
		path.find(".gz") == std::string::npos &&
		path.find(".7z") == std::string::npos &&
		path.find(".txt") == std::string::npos &&
		path.find(".xml") == std::string::npos &&
		path.find(".json") == std::string::npos &&
		path.find(".csv") == std::string::npos)
		return (false);
	return (true);
}

std::string readFile(std::string filePath, t_serverData *data)
{
	std::ifstream inputFile(filePath.c_str(), std::ios::binary);
	std::string code;

	checkAccessFile(code, filePath, data);
	if (!inputFile.is_open())
		errorPage("Error read file", "404", data);
	std::stringstream buffer;
	buffer << inputFile.rdbuf();
	return (buffer.str());
}

std::string read_error_file(std::string path, t_serverData *data)
{
	std::ifstream file(path.c_str(), std::ios::binary);
	if(!file.is_open()){
		Response::sendResponse("500", "text/html", "<h1>500 Internal Server Error</h1>", data);
	}
	return std::string(
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	);
}

bool is_keep_alive(std::string &header)
{
	if(header.find("keep-alive") != std::string::npos)
		return (true);
	return (false);
}

bool request_allowed(std::string typeRequest, t_serverData *data)
{
	std::vector<std::string>::iterator it = std::find(data->requestAllow.begin(), data->requestAllow.end(), typeRequest);
	if(it != data->requestAllow.end() || !data->requestAllow.size())
		return(true);
	errorPage("", "405", data);
	return (false);
}

void truncate_file(std::string &file, t_serverData *data)
{
	size_t pos = data->header.find("boundary=");
	std::string boundary = data->header.substr(pos + 9, data->header.size() - pos);
	pos = boundary.find("\r\n");
	boundary = boundary.substr(0, pos);
	pos = file.find("\n");
	file.erase(0, pos + 1);
	pos = file.find("\n");
	file.erase(0, pos + 1);
	pos = file.find("\n");
	file.erase(0, pos + 1);
	pos = file.find("\n");
	file.erase(0, pos + 1);
	pos = file.find(boundary + "--");
	if(pos != std::string::npos)
		file.erase(pos - 1, (boundary + "--").size());
}

void	signal_handler(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
		GlobalLinkedList::cleanup();
}

void	configureSignals()
{
	std::signal(SIGINT, signal_handler);
	std::signal(SIGQUIT, signal_handler);
}

void	checkLocationPath(Location &location, std::vector<Location> &locations)
{
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getPath() == location.getPath())
			throw Response::ConfigurationFileLocation("Path already exist");
	}
}

void	checkAccessFile(std::string &code, std::string &filePath, t_serverData *data)
{
	if(access(filePath.c_str(), F_OK) != 0)
	{
		errorPage("", "404", data);
	}
	else if (access(filePath.c_str(), R_OK) != 0)
		errorPage("", "403", data);
	else
		code = "200 OK";
}
