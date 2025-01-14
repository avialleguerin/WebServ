#ifndef COOKIE_HPP
#define COOKIE_HPP

#include "Header.hpp"

typedef struct s_session {
	time_t								expireDate;
	int									is_valid;
	std::pair<std::string, std::string>	credentials;
} t_session;

class Cookie
{
	private:
		std::map<std::string, t_session>	session; 

	public:
		std::map<std::string, t_session>	get_session();
		std::pair<std::string, t_session>	get_session_id(std::string id);
		void								add_session(std::pair<std::string, t_session> session);
		bool								remove_session_id(std::string id);
};

#endif