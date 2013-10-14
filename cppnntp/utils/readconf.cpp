#include "readconf.hpp"

namespace cppnntp {

	readconf::readconf() {
		checkconfig();
	}

	readconf::~readconf() {
	}

	std::string readconf::nntpuser() {
		return NNTPUSER;
	}

	std::string readconf::nntppass() {
		return NNTPPASS;
	}

	std::string readconf::nntphost() {
		return NNTPHOST;
	}

	std::string readconf::nntpport() {
		return NNTPPORT;
	}

	bool readconf::nntpssl() {
		return NNTPSSL;
	}

	void readconf::checkconfig() {
		// Check if configuration.cfg exists.
		std::ifstream configfile("configuration.cfg");
		bool make = false;

		// No file so make one.
		if (!configfile)
			make = true;
		// Check if it's empty.
		else {
			if (configfile.peek() == std::ifstream::traits_type::eof())
				make = true;
		}

		if (make)
			initiateconfig();
		else
			readconfig();
	}

	void readconf::readconfig() {
		std::string currentline;
		std::ifstream configfile;
		configfile.open("configuration.cfg");

		while(!configfile.eof()) {
			std::getline(configfile, currentline);

			try {
				boost::smatch match;
				boost::regex pattern
					("^(.+?)[[:space:]]*=[[:space:]]*(.*?)[[:space:]]*$",
					boost::regex_constants::icase);

				if (boost::regex_search (currentline, match, pattern)) {
					if (match[1] == "NNTP_USERNAME")
						NNTPUSER = match[2];
					else if (match[1] == "NNTP_PASSWORD")
						NNTPPASS = match[2];
					else if (match[1] == "NNTP_SERVER")
						NNTPHOST = match[2];
					else if (match[1] == "NNTP_PORT")
						NNTPPORT = match[2];
					else if (match[1] == "NNTP_SSL") {
						if (match[2] == "true")
							NNTPSSL = true;
					}
					else if (match[1] == "DB_HOSTNAME")
						DBHOST = match[2];
					else if (match[1] == "DB_PORT")
						DBPORT = match[2];
				}
			} catch (boost::regex_error& e) {
				boostRegexExceptions(e);
			}
			currentline = "";
		}
		configfile.close();
	}

	void readconf::initiateconfig() {
		std::ofstream configfile;
		configfile.open("configuration.cfg");
		if (!configfile.is_open())
			return;

		std::cout << "Initiating a new configuration file.\n";
		std::string currentsetting, confirmation = "";

		while (1) {
			std::cout << "Please enter your NNTP username without spaces, press enter when done.\n";
			std::cin >> currentsetting;
			std::cout << "You entered: '" << currentsetting << "' is this correct?\n"
			<< "Type yes if you are satisfied, or anything else to restart, then press enter.\n";
			std::cin >> confirmation;
			if (confirmation == "yes") {
				NNTPUSER = currentsetting;
				configfile << "#NNTP SETTINGS" << std::endl;
				configfile << "NNTP_USERNAME = " << currentsetting << std::endl;
				break;
			}
		}

		while (1) {
			std::cout << "Please enter your NNTP password without spaces, press enter when done.\n";
			std::cin >> currentsetting;
			std::cout << "You entered: '" << currentsetting << "' is this correct?\n"
			<< "Type yes if you are satisfied, or anything else to restart, then press enter.\n";
			std::cin >> confirmation;
			if (confirmation == "yes") {
				NNTPPASS = currentsetting;
				configfile << "NNTP_PASSWORD = " << currentsetting << std::endl;
				break;
			}
		}

		while (1) {
			std::cout << "Please enter your NNTP server address without spaces, press enter when done.\n";
			std::cin >> currentsetting;
			std::cout << "You entered: '" << currentsetting << "' is this correct?\n"
			<< "Type yes if you are satisfied, or anything else to restart, then press enter.\n";
			std::cin >> confirmation;
			if (confirmation == "yes") {
				NNTPHOST = currentsetting;
				configfile << "NNTP_SERVER = " << currentsetting << std::endl;
				break;
			}
		}

		while (1) {
			std::cout << "Please enter your NNTP server port without spaces, press enter when done.\n";
			std::cin >> currentsetting;
			std::cout << "You entered: '" << currentsetting << "' is this correct?\n"
			<< "Type yes if you are satisfied, or anything else to restart, then press enter.\n";
			std::cin >> confirmation;
			if (confirmation == "yes") {
				NNTPPORT = currentsetting;
				configfile << "NNTP_PORT = " << currentsetting << std::endl;
				break;
			}
		}

		while (1) {
			std::cout << "Does this port require SSL? (types true or false, without spaces then enter)\n";
			std::cin >> currentsetting;
			std::cout << "You entered: '" << currentsetting << "' is this correct?\n"
			<< "Type yes if you are satisfied, or anything else to restart, then press enter.\n";
			std::cin >> confirmation;
			if (confirmation == "yes") {
				if (currentsetting == "true")
					NNTPSSL = true;

				configfile << "NNTP_SSL = " << currentsetting << std::endl;
				break;
			}
		}

		while (1) {
			std::cout << "Please enter your MongoDB hostname (default is localhost) without spaces, press enter when done.\n";
			std::cin >> currentsetting;
			std::cout << "You entered: '" << currentsetting << "' is this correct?\n"
			<< "Type yes if you are satisfied, or anything else to restart, then press enter.\n";
			std::cin >> confirmation;
			if (confirmation == "yes") {
				DBHOST = currentsetting;
				configfile << std::endl << "#DB settings" << std::endl;
				configfile << "DB_HOSTNAME = " << currentsetting << std::endl;
				break;
			}
		}

		while (1) {
			std::cout << "Please enter your MongoDB port (default is 27017) without spaces, press enter when done.\n";
			std::cin >> currentsetting;
			std::cout << "You entered: '" << currentsetting << "' is this correct?\n"
			<< "Type yes if you are satisfied, or anything else to restart, then press enter.\n";
			std::cin >> confirmation;
			if (confirmation == "yes") {
				DBPORT = currentsetting;
				configfile << "DB_PORT = " << currentsetting << std::endl;
				break;
			}
		}
	}
}
