#include "examples.hpp"

// Get the config options.
cppnntp::readconf config;

int main() {
	start();
}

void start() {
	std::cout
	<< "Welcome to cppnntp, type help for a list of options.\n"
	<< "If you want to exit, type exit.\n";

	std::string userinput;
	while (1) {
		std::cout << ">";
		std::cin >> userinput;
		if (userinput == "help")
			help();
		else if (userinput == "exit")
			break;
		else if (userinput == "nntp") {
			examples();
			std::cout
			<< "Welcome to cppnntp, type help for a list of options.\n"
			<< "If you want to exit, type exit.\n";
		}
	}
}

void help() {
	std::cout
	<< "\nList of valid commands follows:\n"
	<< "help        : Displays this.\n"
	<< "exit        : Exits the program.\n"
	<< "nntp        : Run the NNTP examples.\n";
}

void examples() {
	std::cout << "Attempting to connect to " << config.nntphost()
	<< ":" << config.nntpport() << (config.nntpssl() ? " using SSL" : "")
	<< " with username: " << config.nntpuser() << "." << std::endl;

	cppnntp::nntp nntp;
	nntp.connect(config.nntphost(), config.nntpport(), config.nntpssl());
	nntp.login(config.nntpuser(), config.nntppass());

	std::cout << "If you are finished, type done,"
	<< " else type in a group name. (ex: alt.binaries.teevee)\n";
	std::string command;
	while (1) {
		std::cout << ">";
		std::cin >> command;
		if (command == "done")
			return;
		else {
			if (nntp.group(command)) {
				std::cout
				<< "Group " << nntp.group_name()
				<< "'s oldest article is " << nntp.group_oldest()
				<< ", newest article is " << nntp.group_newest()
				<< ", and it has a total of " << nntp.group_total()
				<< " articles." << std::endl;
				break;
			}
			else
				continue;
		}
	}

	std::cout << "If you are finished, type done,"
	<< " else type in a single article number. (ex: 56319488)\n";
		while (1) {
		std::cout << ">";
		std::cin >> command;
		if (command == "done")
			return;
		else {
			if (nntp.head(command)) {
				break;
			}
			else
				continue;
		}
	}

	std::cout << "Do you want to turn on header compression? Type yes or no.\n>";
	std::cin >> command;
	if (command == "yes")
		nntp.xfeaturegzip();

	std::cout << "If you are finished, type done,"
	<< " else type continue and we will download an header using XOVER.\n";
		while (1) {
		std::cout << ">";
		std::cin >> command;
		if (command == "done")
			return;
		else {
			if (nntp.xover(std::to_string(nntp.group_newest() - 2), 
				std::to_string(nntp.group_newest())))
				break;
			else
				continue;
		}
	}
}
