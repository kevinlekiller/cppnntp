#include "examples.hpp"

cppnntp::nntp nntp;

int main() {
	start();
}

void start() {
	if (!readconf()) {
		std::cerr << "Problem getting settings from config.cfg\n";
		return;
	}

	std::cout << "Welcome to cppnntp!\n";
	std::string in;
	while (1) {
		std::cout
		<< "Type help for a list of options, "
		<< " to exit, type exit.\n> ";

		std::getline (std::cin, in);
		if (in.empty())
			continue;

		try {
			if      (in == "help")
				help();
			else if (in == "exit")
				break;

			else if (in == "connect") {
				if (nntp.connect(NNTP_HOST, NNTP_PORT, NNTP_SSL))
					std::cout << "Connected to usenet, now please login.\n";
				else {
					std::cerr << "Problem connecting to usenet, check host/port.\n";
					break;
				}
			}
			else if (in == "login") {
				if (!nntp.login(NNTP_USER, NNTP_PASS))
					break;
			}
			else if (in == "disconnect")
				nntp.disconnect();

			else if (in == "capabilities")
				nntp.capabilities();
			else if (in == "date")
				nntp.date();
			else if (in == "helplist")
				nntp.help();
			else if (in == "compression")
				nntp.xfeaturegzip();
			else if (in == "group") {
				std::string group;
				std::cout << "Please enter the name of a group.\n> ";
				std::getline (std::cin, group);
				if (!nntp.group(group))
					continue;
			}

			else if (in == "article") {
				std::string article;
				std::cout << "Please enter an article number or message-id.\n> ";
				std::getline (std::cin, article);
				if (!nntp.article(article))
					continue;
			}
			else if (in == "body") {
				std::string body;
				std::cout << "Please enter an article number or message-id.\n> ";
				std::getline (std::cin, body);
				if (!nntp.body(body))
					continue;
			}
			else if (in == "grouptotal")
				std::cout << nntp.group_total() << std::endl;
			else if (in == "groupnewest")
				std::cout << nntp.group_newest() << std::endl;
			else if (in == "groupoldest")
				std::cout << nntp.group_oldest() << std::endl;
			else if (in == "groupname")
				std::cout << nntp.group_name() << std::endl;
			else if (in == "head") {
				std::string head;
				std::cout << "Please enter an article number or message-id.\n> ";
				std::getline (std::cin, head);
				if (!nntp.head(head))
					continue;
			}
			else if (in == "xover") {
				std::string xover;
				std::cout << "Please enter an article number or message-id.\n> ";
				std::getline (std::cin, xover);
				if (!nntp.head(xover))
					continue;
			}
			else
				std::cout << "Command not understood, type help.\n";
		} catch (const std::runtime_error &e) {
			std::cerr << e.what();
		}
	}
}

void help() {
	std::cout
	<< "\nList of valid commands follows:\n"
	
	<< "\nGeneral commands:\n"
	<< "help           : Displays this.\n"
	<< "exit           : Exits the program.\n"
	
	<< "\nInitial NNTP commands:\n"
	<< "connect        : Connect to usenet.\n"
	<< "login          : Login to usenet.\n"
	<< "disconnect     : Disconnect from usenet.\n"

	<< "\nGeneral NNTP commands:\n"
	<< "capabilities   : Displays supported capabilities by the NNTP server.\n"
	<< "date           : Displays the UTC time sent back from the NNTP server.\n"
	<< "helplist       : Displays the NNTP servers commands list.\n"
	<< "compression    : Turn on compression for xover headers.\n"
	<< "group          : Select a group.\n"

	<< "\nCommands that require a selected group:\n"
	<< "grouptotal     : The total amount of articles for the selected group.\n"
	<< "groupnewest    : The newest article number for the selected group.\n"
	<< "groupoldest    : The oldest article number for the selected group.\n"
	<< "groupname      : The name of the currently selected group.\n"
	<< "article        : Display an article.\n"
	<< "body           : Display the body of an article.\n"
	<< "head           : Display the header of an article.\n"
	<< "xover          : Display the header of an article.\n";
}

bool readconf() {
	std::ifstream file;

	file.open("config.cfg");

	if (!file.is_open())
		return false;

	std::string line;

	while(!file.eof()) {
		std::getline(file, line);

		try {
			boost::smatch match;
			boost::regex pattern
				("^(.+?)[[:space:]]*=[[:space:]]*(.*?)[[:space:]]*$",
				boost::regex_constants::icase);

			if (boost::regex_search (line, match, pattern)) {
				if (match[1] == "NNTP_USER")
					NNTP_USER = match[2];
				else if (match[1] == "NNTP_PASS")
					NNTP_PASS = match[2];
				else if (match[1] == "NNTP_HOST")
					NNTP_HOST = match[2];
				else if (match[1] == "NNTP_PORT")
					NNTP_PORT = match[2];
				else if (match[1] == "NNTP_SSL") {
					if (match[2] == "true")
						NNTP_SSL = true;
				}
			}
		} catch (boost::regex_error& e) {
			std::cerr << e.what() << std::endl;
			return false;
		}
		line = "";
	}
	file.close();
	if (NNTP_USER == "" || NNTP_PASS == "" || NNTP_HOST == "" || NNTP_PORT == "")
		return false;

	return true;
}
