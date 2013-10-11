#include <fstream>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include "boostRegexExceptions.hpp"
#include "nntp.hpp"

/**
 * Fetch the config options from the config file.
 */
bool readconf(std::string &hostname, std::string &port,
				std::string &username, std::string &password,
				std::string &group, std::string &xover,
				std::string &start, std::string &end,
				std::string &ssl, std::string &body,
				std::string &messageid, std::string &path) {

	// Create a string to hold the file.
	std::string line;

	// Create a file object to store the file contents.
	std::ifstream file;

	// Open the file.
	file.open("config.cfg");

	// Check if the file is open.
	if (!file.is_open())
		return false;

	// Loop through the file line by line until we get to the end.
	while(!file.eof()) {

		// Store the file's line into the string called line.
		std::getline(file, line);

		// Catch regex exceptions.
		try {

			// Create a regex array to store the found matches.
			boost::smatch match;

			// Create a regex to find the options and settings.
			boost::regex pattern
				("^(.+?)[[:space:]]*=[[:space:]]*(.*?)[[:space:]]*$",
				boost::regex_constants::icase);

			// Store the found settings according to the option.
			if (boost::regex_search (line, match, pattern)) {
				if (match[1] == "NNTP_SERVER")
					hostname = match[2];
				else if (match[1] == "NNTP_PORT")
					port = match[2];
				else if (match[1] == "NNTP_USERNAME")
					username = match[2];
				else if (match[1] == "NNTP_PASSWORD")
					password = match[2];
				else if (match[1] == "NNTP_SSL")
					ssl = match[2];
				else if (match[1] == "GROUP")
					group = match[2];
				else if (match[1] == "XOVER")
					xover = match[2];
				else if (match[1] == "START_HEADER")
					start = match[2];
				else if (match[1] == "END_HEADER")
					end = match[2];
				else if (match[1] == "BODY")
					body = match[2];
				else if (match[1] == "MESSAGE_ID")
					messageid = match[2];
				else if (match[1] == "PATH")
					path = match[2];
			}
		} catch (boost::regex_error& e) {
			boostRegexExceptions(e);
		}

		// Reset the line.
		line = "";
	}

	// Close the file.
	file.close();

	// Check if any of the required options were empty.
	if (username  == "" || password == "" || hostname == ""
		|| port == "" || group == "" || ssl == ""
		|| path == "" || xover == "")
		return false;
	else
		return true;
}

/**
 * Show some examples of using the library.
 */
int testusenet1(std::string &hostname, std::string &port,
				std::string &username, std::string &password,
				std::string &group, std::string &xover,
				std::string &start, std::string &end,
				std::string &ssl, std::string &body,
				std::string &messageid, std::string &path) {

	// Create instance of class nntp.
	nntp::nntp nntp;

	// Connect to the usenet server. Check if SSL was true.
	if (ssl == "true") {
		if (nntp.sslconnect(hostname, port))
			std::cout << "Succesfully connected to usenet.\n";
		else {
			std::cerr << "Problem connecting to usenet.\n";
			return 1;
		}
	}
	else {
		if (nntp.connect(hostname, port))
			std::cout << "Succesfully connected to usenet.\n";
		else {
			std::cerr << "Problem connecting to usenet.\n";
			return 1;
		}
	}

	/* This will log in to usenet using the supplied password
	 * and username.
	 */
	if (nntp.login(username, password))
		std::cout << "Succesfully logged in to usenet.\n";
	else {
		std::cerr << "Problem when trying to log in to usenet.\n";
		return 1;
	}

	/* Send the HELP command which displays a list of
	 * all the legal commands your usenet provider accepts.
	 */
	if (nntp.help())
		std::cout << "HELP messages succesfully retrieved.\n";
	else {
		std::cerr << "Problem passing the HELP command.\n";
		return 1;
	}

	/* Send the DATE command which displays the
	 * local time of your usenet provider.
	 */
	if (nntp.date())
		std::cout << "Retrieved the date from the usenet server.\n";
	else {
		std::cerr << "Problem passing the DATE command.\n";
		return 1;
	}
	 return 0;
	/* Since we go out of scope, the desctructors of both
	 * socket and nntp will handle closing the connection.
	 */
}

/**
 * Show some more examples of using the library.
 */
int testusenet2(std::string &hostname, std::string &port,
				std::string &username, std::string &password,
				std::string &group, std::string &xover,
				std::string &start, std::string &end,
				std::string &ssl, std::string &body,
				std::string &messageid, std::string &path) {

	// Create instance of class nntp.
	nntp::nntp nntp;

	// Connect to the usenet server. Check if SSL was true.
	if (ssl == "true") {
		if (nntp.sslconnect(hostname, port))
			std::cout << "Succesfully connected to usenet.\n";
		else {
			std::cerr << "Problem connecting to usenet.\n";
			return 1;
		}
	}
	else {
		if (nntp.connect(hostname, port))
			std::cout << "Succesfully connected to usenet.\n";
		else {
			std::cerr << "Problem connecting to usenet.\n";
			return 1;
		}
	}

	/* This will log in to usenet using the supplied password
	 * and username.
	 */
	if (nntp.login(username, password))
		std::cout << "Succesfully logged in to usenet.\n";
	else {
		std::cerr << "Problem when trying to log in to usenet.\n";
		return 1;
	}

	/* Send the GROUP command to select a group. This displays information
	 * on the articles stored in the group.
	 */
	if (nntp.group(group))
		std::cout << "Succesfully passed the GROUP command.\n";
	else {
		std::cerr << "Problem selecting the GROUP.\n";
		return 1;
	}

	if (xover == "true") {
		/* Send the XOVER command which displays an header with
		 * basic info.
		 *
		 * Right now, this is parsed by the nntp class, but in the
		 * future I will make an overloaded function with
		 * the parsed header as an array.
		 */
		if (nntp.xover(start, end))
			std::cout << "Succesfully downloaded headers with XOVER command.\n";
		else {
			std::cerr << "Problem downloading the header with XOVER.\n";
			return 1;
		}
	}

	/* Send an ARTICLE command, this displays the header + body of
	 * an article.
	 *
	 * I commented this out because the output is quite large and
	 * the body + header commands will display overall the same
	 * information.
	 */
	/*if (nntp.article(start))
		std::cout << "Succesfully downloaded an article.\n";
	else {
		std::cerr << "Problem downloading the article.\n";
		return 1;
	}*/

	if (body == "true") {
		/* Send an BODY command and store the decoded yEnc on disk.
		 * In this example it fetches a rar file (the message-id in
		 * config.cfg links to part of a rar file on usenet).
		 * The extension is not important, you can even create the file
		 * without an extension.
		 *
		 * The decoded data is also in this string, if you need to use it,
		 * storing on drive is optional
		 * (don't pass a 3rd arg if you don't want to store on drive).
		 *
		 * The string can be useful if you have a library that can read
		 * rar files from string. Or if you want to do something else
		 * with it.
		 *
		 * You can also call the body function with only the messageid
		 * or article number, this will print the output from usenet
		 * on the screen.
		 */
		std::string decodeddata;
		if (nntp.body(messageid, decodeddata, path)) {
			std::cout
			<< "Succesfully downloaded an article body and stored it here: "
			<< path << std::endl;
		}
		else {
			std::cerr << "Problem downloading the article body.\n";
			return 1;
		}
	}

	/* This sends the HEAD command which displays a full article header,
	 * unlike XOVER which only displays the more useful lines
	 * of the article.
	 */
	if (nntp.head(start))
		std::cout << "Succesfully downloaded an article header with HEAD command.\n";
	else {
		std::cerr << "Problem downloading the article header with HEAD command.\n";
		return 1;
	}

	/* This is just an example to show we can keep the class instance
	 * and disconnect from usenet, then reconnect at a later time.
	 */
	nntp.disconnect();
	if (nntp.sslconnect(hostname, port))
		std::cout << "Connected to usenet.\n";
	else {
		std::cerr << "Problem connecting to usenet.\n";
		return 1;
	}

	return 0;
	/* Again we go out of scope, the destructors take care of
	 * the connection to usenet (so disconnect is not needed if you
	 * go out of scope). However if you need require a connection,
	 * then you do nothing with that connection for a long period of
	 * time, it is a good idea to disconnect.
	 */
}

int main() {

	// Create strings to store the settings in.
	std::string hostname,port,username,password,group,
		xover,start,end,ssl,body,messageid,path;

	// Read the config file and store the settings in the strings.
	if (!readconf(hostname, port, username, password, group, xover,
		start, end, ssl, body, messageid, path)) {
		std::cerr << "Error getting options from config.cfg\n";
		return 1;
	}

	// Run the first set of tests.
	if (testusenet1(hostname, port, username, password, group, xover,
					start, end, ssl, body, messageid, path) != 0)
		return 1;

	// Sleep for 2 seconds.
	auto seconds(boost::posix_time::milliseconds(2000));
	boost::this_thread::sleep(seconds);

	// Run the second set of tests.
	if (testusenet2(hostname, port, username, password, group, xover,
					start, end, ssl, body, messageid, path) != 0)
		return 1;

	return 0;
}
