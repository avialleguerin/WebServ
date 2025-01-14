#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Header.hpp"

class Request
{
	private:
		std::string							_method;
		std::string							_path;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
	public:
		void	parseRequest();


};

# endif