#include "Header.hpp"

/**
 * @brief	Check if a path is a directory.
 * @param	path	The path to check.
*/
bool	isDirectory(const std::string& path)
{
	struct stat statbuf;
	std::string full = "./www/" + path;
	if (stat(full.c_str(), &statbuf) != 0)
		return false;
	return S_ISDIR(statbuf.st_mode);
}

/**
 * @brief	List the content of a directory.
 * @param	directory	The directory to list.
*/
std::vector<std::string> listDirectory(const std::string& directory)
{
	std::vector<std::string> files;
	DIR* dir = opendir(directory.c_str());
	if (dir != NULL)
	{
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL)
			files.push_back(entry->d_name);
		closedir(dir);
	}
	return files;
}

/**
 * @brief	Generate an autoindex page.
 * @param	directory	The directory to generate the page for.
 * @param	files		The list of files in the directory.
*/
std::string generateAutoIndexPage(const std::string directory, const std::vector<std::string>& files)
{
	std::string html;
	html = "\n<html>\n<head>\n\t<title>Index of " + directory + "</title>\n</head>\n<body>\n";
	html += "<h1>Index of " + directory + "</h1><ul>\n";

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		std::string relativePath = directory + *it;
		if(relativePath.at(0) != '/')
			relativePath.insert(0, 1, '/');
		html += "\t<li><a href=\"" + relativePath + "\">" + *it + (isDirectory(relativePath) ? "/" : "") + "</a></li>\n";
	}
	html += "</ul></body></html>";
	return html;
}
