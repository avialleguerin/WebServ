#include "Header.hpp"

Node* GlobalLinkedList::head = NULL;
Node* GlobalLinkedList::tail = NULL;

int main(int ac, char **av)
{
	try
	{
		configureSignals();
		if (ac < 2)
			throw std::runtime_error("Usage: <program_name> <config_file>");
		
		ConfigParser	config(av[1]);
		Server			listenAddress;

		config.parseConfig(config.getServers());
		if (config.getServers().empty())
			throw std::runtime_error("No server configuration found in the file");
		config.printConfig();
		listenAddress.createListenAddr(config);
		GlobalLinkedList::cleanup();
	}
	catch (const std::exception& e)
	{
		GlobalLinkedList::cleanup();
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	}
	return (0);
}
