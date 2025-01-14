#include "Header.hpp"

std::string httpDeleteResponse(std::string code, std::string contentType, std::string content)
{
	return ("HTTP/1.1 " + code + " \r\n"
			"Content-Type: " + contentType + "\r\n"
			"Content-Length: " + to_string(content.size()) + "\r\n"
			"Connection: close\r\n"
			"\r\n" + content);
}

bool deleteFile(std::string &filePath)
{
	if(!remove(filePath.c_str()))
		return (true);
	return (false);
}

void displayDeletePage(std::string path, t_serverData *data)
{
	std::string filePath = data->path + path;
	std::string content = readFile(filePath, data);
	std::string contentType = getContentType(filePath, "DELETE", data);
	std::string pathToUpload = "./www/upload";
	std::string html = "<ul style=\"text-align:center\">\n";
	std::string response;

	std::vector<std::string> files = listDirectory(pathToUpload);
	std::vector<std::string>::iterator it = files.begin();
	while(it != files.end())
	{
		if(*it != "." && *it != "..")
			html += "\t<li>" + *it + "</li>\n";
		it++;
	}
	html += "</ul>\n</div><a href=\"../../index.html\" id=\"return\">Go Back Home</a>\n";
	html += "</body>\n</html>";
	content += html;
	response = httpGetResponse("200 Ok", contentType, content, data, "");
	if(send(data->sockfd, response.c_str(), response.size(), 0) < 0)
	{
		std::cout << strerror(errno) << std::endl;
		errorPage("", "500", data);
	}
}


void deleteRequest(std::string &uri, t_serverData *data, std::string typeRequest, std::map<int, t_serverData*> &fdEpollLink)
{
	std::string code;
	std::string content;
	std::string filePath = check_location(uri, content, data->location, data, fdEpollLink);
	std::string response;
	std::string contentType = getContentType(uri, typeRequest, data);
	//if i have a location
	if(filePath.empty())
		filePath = data->path + uri;
	checkAccessFile(code, filePath, data);
	content = readFile(filePath, data);
	deleteFile(filePath);

	response = httpGetResponse("200", "text/html", "", data, "");
	//send response
	if(send(data->sockfd, response.c_str(), response.size(), 0) < 0)
	{
		std::cout << strerror(errno) << std::endl;
		errorPage("", "500", data);
	}
}

void parseAndDeleteRequest(std::string buffer, t_serverData *data, std::string typeRequest, std::map<int, t_serverData*> &fdEpollLink)
{
	std::string path = buffer.substr(buffer.find('/') + 1, buffer.size() - buffer.find('/'));
	path = path.substr(0, path.find(' '));
	if(path.find("favicon.ico") != std::string::npos)
		close(data->sockfd);
	else if(path.find("?") != std::string::npos)
		errorPage("", "501", data);
	deleteRequest(path, data, typeRequest, fdEpollLink);
}