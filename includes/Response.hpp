#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Header.hpp"

class Response
{
	public:
		static void	sendResponse(std::string statusCode, std::string contentType, std::string content, t_serverData *data);

		class ErrorOpeningFile : public std::exception{
			private:
				std::string _msg;
			public:
				explicit ErrorOpeningFile(const std::string &msg):
					_msg("Error opening File: " + msg) {}
				virtual const char* what() const throw();
				virtual ~ErrorOpeningFile() throw() {}
		};

		class ErrorSendingResponse : public std::exception{
			public:
				virtual const char* what() const throw();
		};

		class ConfigurationFileLocation : public std::exception{ // Config Location
			private:
				std::string _msg;
			public:
				explicit ConfigurationFileLocation(const std::string &msg):
					_msg("Location configuration: " + msg) {}
				virtual const char* what() const throw();
				virtual ~ConfigurationFileLocation() throw() {}
		};

		class ConfigurationFileServer : public std::exception{ // Config Server
			private:
				std::string _msg;
			public:
				explicit ConfigurationFileServer(const std::string &msg):
					_msg("Server configuration: " + msg) {}
				virtual const char* what() const throw();
				virtual ~ConfigurationFileServer() throw() {}
		};

		class ErrorRequest : public std::exception{
			private:
				std::string _msg;
			public:
				explicit ErrorRequest(const std::string &msg):
					_msg("Request failed: " + msg) {}
				virtual const char* what() const throw();
				virtual ~ErrorRequest() throw() {}
		};

		class ErrorEpoll : public std::exception{
			private:
				std::string _msg;
			public:
				explicit ErrorEpoll(const std::string &msg):
					_msg("Epoll: " + msg) {}
				virtual const char* what() const throw();
				virtual ~ErrorEpoll() throw() {}
		};

		class DisplayErrorPage : public std::exception{
			private:
				std::string _msg;
				std::string _code;
				t_serverData *_data;
			public:
				explicit DisplayErrorPage(const std::string &msg, const std::string &code, t_serverData *data):
					_msg("Display Page: " + msg), _code(code), _data(data) {}
				virtual const char* what() const throw();
				virtual ~DisplayErrorPage() throw() {}
		};

		class ErrorCGI : public std::exception{
			private:
				std::string _msg;
			public:
				explicit ErrorCGI(const std::string &msg):
					_msg("CGI: " + msg) {}
				virtual const char* what() const throw();
				virtual ~ErrorCGI() throw() {}
		};

		class ErrorSocket : public std::exception{
			private:
				std::string _msg;
			public:
				explicit ErrorSocket(const std::string &msg):
					_msg("Creating socket: " + msg) {}
				virtual const char* what() const throw();
				virtual ~ErrorSocket() throw() {}
		};

		class ErrorBodyPostRequest : public std::exception{
			public:
				virtual const char* what() const throw();
		};

		class ErrorMaxBody : public std::exception{
			public:
				virtual const char* what() const throw();
		};

		class responseOk : public std::exception{
			public:
				virtual const char* what() const throw();
		};
};

#endif /* RESPONSE_HPP */