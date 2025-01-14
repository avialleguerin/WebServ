#include "Header.hpp"

void Response::sendResponse(std::string statusCode, std::string contentType, std::string content, t_serverData *data)
{
	std::string	response;
	response = "HTTP/1.1 " + statusCode + " \r\n";
	response += "Content-Type: " + contentType + " \r\n";
	response += "Content-Length: " + to_string(content.size()) + "\r\n";
	if (is_keep_alive(data->header))
		response += "Connection: keep-alive\r\n";
	else
		response += "Connection: close\r\n";
	response += "\r\n" + content;
	if(send(data->sockfd, response.c_str(), response.size(), 0) < 0)
		errorPage(std::string(strerror(errno)), "500", data);
	throw Response::responseOk();
}

std::string httpGetResponse(std::string code, std::string contentType, std::string content, t_serverData *data, std::string filePath)
{
	//make the header response
	std::string response = "HTTP/1.1 " + code + "\r\n" ;
	response += "Content-Length: " + to_string(content.size()) + "\r\n";
	if (is_keep_alive(data->header))
		response += "Connection: keep-alive\r\n";
	else
		response += "Connection: close\r\n";
	if(data->isDownload)
	{
		size_t pos = filePath.find_last_of("/");
		std::string file = filePath.substr(pos + 1, filePath.size());
		response += "Content-Disposition: attachment; filename=" + file  + "\r\n";
		response += "Content-Type: application/octet-stream\r\n";
		response += "Content-Transfer-Encoding: binary\r\n";
		data->isDownload = false;
	}
	else
		response += "Content-Type: " + contentType + "\r\n";
	response += "\r\n" + content;
	return (response);
}

std::string httpGetResponseDownload(std::string code, std::string contentType, std::string content, t_serverData *data)
{
	std::string response = "HTTP/1.1 " + code + " \r\n" ;
	response += "Content-Type: " + contentType + "\r\n";
	response += "Content-Length: " + to_string(content.size()) + "\r\n";
	response += "Content-Disposition: attachment\r\n";
	if (is_keep_alive(data->header))
		response += "Connection: keep-alive\r\n";
	else
		response += "Connection: close\r\n";
	response += "\r\n" + content;
	return (response);
}

void redirRequest(std::string location, int fd, t_serverData *data)
{
	std::string response = "HTTP/1.1 302 Found \r\n"
							"Location: " + location + "\r\n"
							"Content-Type: text/html\r\n"
							"Content-Length: 0 \r\n";
	if (is_keep_alive(data->header))
		response += "Connection: keep-alive\r\n\r\n";
	else
		response += "Connection: close\r\n\r\n";
	if(send(fd, response.c_str(), response.size(), 0) < 0)
		errorPage(std::string(strerror(errno)), "500", data);
}

void httpPostResponse(std::string code , std::string contentType, std::string content, t_serverData *data, Cookie &cookie, std::string id)
{
	std::string response = "HTTP/1.1 " + code + " \r\n"
							"Content-Type: " + contentType + "\r\n";

	std::pair<std::string, t_session> session = cookie.get_session_id(id);
	if(!session.first.empty())
	{
		response += "Set-Cookie: id=" + session.first + 
					"; Expires=" + manageDate(session.second.expireDate) +
					"; password=" + session.second.credentials.first +
					"; Path=/";
		response += "; email=" + session.second.credentials.second + "\r\n";
	}
	if (is_keep_alive(data->header))
		response += "Connection: keep-alive\r\n";
	else
		response += "Connection: close\r\n";
	response +=	"Content-Length: " + to_string(content.size()) + "\r\n"
				"\r\n" + content;
	if(send(data->sockfd, response.c_str(), response.size(), 0) < 0)
	{
		errorPage(std::string(strerror(errno)), "500", data);
	}
}

const char*	Response::ErrorOpeningFile::what() const throw()
{
	return _msg.c_str();
}

const char* Response::ErrorSendingResponse::what() const throw()
{
	return("Error sending response");
}

const char* Response::ConfigurationFileLocation::what() const throw()
{
	return _msg.c_str();
}

const char* Response::ConfigurationFileServer::what() const throw()
{
	return _msg.c_str();
}

const char* Response::ErrorRequest::what() const throw()
{
	return _msg.c_str();
}

const char* Response::ErrorSocket::what() const throw()
{
	return _msg.c_str();
}

const char* Response::ErrorCGI::what() const throw()
{
	return _msg.c_str();
}

const char* Response::ErrorEpoll::what() const throw()
{
	return _msg.c_str();
}

const char* Response::DisplayErrorPage::what() const throw()
{
	errorPage(NULL, _code, _data);
	return _msg.c_str();
}

const char* Response::ErrorBodyPostRequest::what() const throw()
{
	return("Error format http POST body");
}

const char* Response::ErrorMaxBody::what() const throw()
{
	return("Error in the syntax of the max body");
}

const char* Response::responseOk::what() const throw()
{
	return("");
}
