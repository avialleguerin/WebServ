#include "Header.hpp"


//=================
//getter and setter
//=================

std::map<std::string, t_session> Cookie::get_session()
{
	return (this->session);
}

std::pair<std::string, t_session> Cookie::get_session_id(std::string id)
{
	std::map<std::string, t_session>::iterator it = this->session.find(id);

	std::pair<std::string, t_session> m;

	if(it != this->session.end())
		return(*it);
	return m;
}

bool Cookie::remove_session_id(std::string id)
{
	std::map<std::string, t_session>::iterator it = this->session.find(id);
	if (it != this->session.end())
	{
		this->session.erase (it);
		return (true);
	}
	return(false);
}

void Cookie::add_session(std::pair<std::string, t_session> session)
{
	this->session.insert(session);
}


//=================
// other functions
//=================

std::string get_cookie_id(std::string buffer)
{
	size_t pos = buffer.find("Cookie: id=");
	
	if (pos != std::string::npos)
	{
		std::string id = buffer.substr(pos + 11, 8);
		return (id);
	}
	return("");
}

bool check_cookie_validity(Cookie &cookie, std::string id)
{
	time_t actualTime = time(NULL);

	if(!cookie.get_session().empty())
	{
		if(cookie.get_session_id(id).second.expireDate < actualTime)
			return (false);
	}
	else
		return(false);
	return(true);
}

std::string display_user_connection(Cookie &cookie, t_serverData *data, std::string response)
{
	std::string html;
	std::string id = get_cookie_id(data->buffer);
	if(id.size() && check_cookie_validity(cookie, id))
	{
		std::pair<std::string, t_session> session_id = cookie.get_session_id(id);
		if(!session_id.first.empty())
		{
			html =
				"\t<div class=\"test-section\">\n"
				"\t\t<h2>Session Tests</h2>\n"
				"\t\t<p><strong>Username / Email: </strong></br>" + session_id.second.credentials.second + " </p>\n"
				"\t\t<p><strong>Password: </strong></br>" + session_id.second.credentials.first + " </p>\n"
				"\t\t<a href=\"/pages/deconnexion\"><button>Log Out</button></a>\n"
				"\t</div>\n" ;

			size_t pos = response.find("</body>");
			if(pos != std::string::npos)
				response = response.insert(pos, html);
		}
	}
	else
	{
		html =
			"\t<div class=\"test-section\">\n"
			"\t\t<h2>Session Tests</h2>\n"
			"\t\t<a href=\"/pages/cookie/connexion.html\"><button>Log In</button></a>\n"
			"\t</div>\n" ;

		size_t pos = response.find("</body>");
		if(pos != std::string::npos)
			response = response.insert(pos, html);
	}
	return (response);
}

std::string gen_random(const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string temp;
	temp.reserve(len);

	for (int i = 0; i < len; ++i) {
		temp += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	return temp;
}

std::string manageDate(time_t current_time)
{
	struct tm *local_time = localtime(&current_time);
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S %Z", local_time);
	return (buffer);
}

std::string newSessionCookie(std::map<std::string, std::string> values, Cookie &cookie, t_serverData *data)
{
	// creating a cookie session and save client info and credentials
	std::pair<std::string, t_session> newSession;
	//add all atributes to my session
	newSession.first = gen_random(8);
	// cookie during 15sec
	newSession.second.expireDate = time(NULL) + 15;

	std::map<std::string, std::string>::iterator it = values.begin();
	if(it == values.end())
		errorPage("", "400", data);
	while(it != values.end())
	{
		if(!newSession.second.credentials.first.size())
			newSession.second.credentials.first = it->second;
		else
			newSession.second.credentials.second = it->second;
		it++;
	}
	cookie.add_session(newSession);
	return (newSession.first);
}
