#include <fstream>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include "cppnntplib/boostRegexExceptions.hpp"
#include "cppnntplib/nntp.hpp"

// Fetch the config options from the config file.
bool readconf(std::string &hostname, std::string &port,
std::string &username, std::string &password, std::string &group,
std::string &xover, std::string &start, std::string &end,
std::string &ssl, std::string &body, std::string &messageid,
std::string &path, std::string &posting, std::string &head,
std::string &xfeature);

// Show some examples of using the library.
int testusenet1(std::string &hostname, std::string &port,
std::string &username, std::string &password, std::string &group,
std::string &xover, std::string &start, std::string &end,
std::string &ssl, std::string &body, std::string &messageid,
std::string &path, std::string &posting, std::string &head,
std::string &xfeature);

// Show some more examples of using the library.
int testusenet2(std::string &hostname, std::string &port,
std::string &username, std::string &password, std::string &group,
std::string &xover, std::string &start, std::string &end,
std::string &ssl, std::string &body, std::string &messageid,
std::string &path, std::string &posting, std::string &head,
std::string &xfeature);

// Get the config options and run the examples.
int main() {

	// Create strings to store the settings in.
	std::string hostname,port,username,password,group,xfeature,
		xover,start,end,ssl,body,messageid,path,posting,head;

	// Read the config file and store the settings in the strings.
	if (!readconf(hostname, port, username, password, group, xover,
		start, end, ssl, body, messageid, path, posting, head, xfeature)) {
		std::cerr << "Error getting options from config.cfg\n";
		return 1;
	}

	// Run the first set of tests.
	if (testusenet1(hostname, port, username, password, group, xover,
			start, end, ssl, body, messageid, path, posting, head, xfeature) != 0)
		return 1;

	// Sleep for 2 seconds.
	auto seconds(boost::posix_time::milliseconds(2000));
	boost::this_thread::sleep(seconds);

	// Run the second set of tests.
	if (testusenet2(hostname, port, username, password, group, xover,
			start, end, ssl, body, messageid, path, posting, head, xfeature) != 0)
		return 1;

	return 0;
}

int testusenet1(std::string &hostname, std::string &port,
				std::string &username, std::string &password,
				std::string &group, std::string &xover,
				std::string &start, std::string &end,
				std::string &ssl, std::string &body,
				std::string &messageid, std::string &path,
				std::string &posting, std::string &head,
				std::string &xfeature) {

	std::cout << "Start of first set of examples.\n";

	// Create instance of class nntp.
	cppnntplib::nntp nntp;

	bool encrypt = false;
	if (ssl == "true")
		encrypt = true;
	// Connect to the usenet server.
	std::cout << "\nExample of connect:\n";
	if (nntp.connect(hostname, port, encrypt))
		std::cout << "Succesfully connected to usenet.\n";
	else {
		std::cerr << "Problem connecting to usenet.\n";
		return 1;
	}

	/* This will log in to usenet using the supplied password
	 * and username.
	 */
	std::cout << "\nExample of login:\n";
	if (nntp.login(username, password))
		std::cout << "Succesfully logged in to usenet.\n";
	else {
		std::cerr << "Problem when trying to log in to usenet.\n";
		return 1;
	}

	/* Send the HELP command which displays a list of
	 * all the legal commands your usenet provider accepts.
	 */
	std::cout << "\nExample of HELP command:\n";
	if (nntp.help())
		std::cout << "HELP messages succesfully retrieved.\n";
	else {
		std::cerr << "Problem passing the HELP command.\n";
		return 1;
	}

	/* Send the DATE command which displays the
	 * local time of your usenet provider.
	 */
	std::cout << "\nExample DATE command:\n";
	if (nntp.date())
		std::cout << "Retrieved the date from the usenet server.\n";
	else {
		std::cerr << "Problem passing the DATE command.\n";
		return 1;
	}

	if (posting == "true") {
		/* Post an article to usenet.
		*/
		std::cout << "\nExample of posting an article:\n";

		// From ; Who is posting the article.
		std::string from = "Demo User <nobody@example.net>";
		// Groups ; Where is the article going.
		std::string groups = "alt.test.misc.test";
		// Subject ; Subject of the article.
		std::string subject = "I am just a test article";
		// Message ; Contents of the article.
		std::string message = "This is just a test article\r\n";
		if (nntp.post(from, groups, subject, message))
			std::cout << "Succesfully posted an article to usenet.\n";
		else {
			std::cerr << "Problem posting article to usenet, possibly your"
			<< " NNTP server didn't give you the right to post articles.\n";
			return 1;
		}

		std::cout << "\nTry to fetch the article we posted (also an example of ARTICLE command).\n";

		/* We must select a group before downloading articles so
		* let's select the group we posted to.
		*/
		if (!nntp.group(groups)) {
			std::cerr << "Problem selecting the GROUP.\n";
			return 1;
		}

		/* Download the newest article (which is probably ours, since
		* this is a dead group).
		*/
		if (nntp.article(std::to_string(nntp.group_newest())))
			std::cout << "Succesfully downloaded an article.\n";
		else {
			std::cerr << "Problem downloading the article.\n";
			return 1;
		}
	}

	std::cout << "\nEnd of first set of examples.\n";

	 return 0;
	/* Since we go out of scope, the desctructors of both
	 * socket and nntp will handle closing the connection.
	 */
}

int testusenet2(std::string &hostname, std::string &port,
				std::string &username, std::string &password,
				std::string &group, std::string &xover,
				std::string &start, std::string &end,
				std::string &ssl, std::string &body,
				std::string &messageid, std::string &path,
				std::string &posting, std::string &head,
				std::string &xfeature) {

	std::cout << "\nStart of second set of examples.\n";

	// Create instance of class nntp.
	cppnntplib::nntp nntp;

	bool encrypt = false;
	if (ssl == "true")
		encrypt = true;
	// Connect to the usenet server.
	std::cout << "\nExample of connect:\n";
	if (nntp.connect(hostname, port, encrypt))
		std::cout << "Succesfully connected to usenet.\n";
	else {
		std::cerr << "Problem connecting to usenet.\n";
		return 1;
	}

	/* This will log in to usenet using the supplied password
	 * and username.
	 */
	std::cout << "\nExample of login:\n";
	if (nntp.login(username, password))
		std::cout << "Succesfully logged in to usenet.\n";
	else {
		std::cerr << "Problem when trying to log in to usenet.\n";
		return 1;
	}

	/* Send the GROUP command to select a group. This displays information
	 * on the articles stored in the group.
	 */
	std::cout << "\nExample of GROUP command:\n";
	if (nntp.group(group))
		std::cout << "Succesfully passed the GROUP command.\n";
	else {
		std::cerr << "Problem selecting the GROUP.\n";
		return 1;
	}

	/* Fetch some group details.
	 */
	std::cout << "\nExample of group objects:\n"
	<< "Group name: " << nntp.group_name() << std::endl
	<< "Group's newest article: " << nntp.group_newest() << std::endl
	<< "Group's oldest article: " << nntp.group_oldest() << std::endl
	<< "Total amount of articles for the group: " << nntp.group_total() << std::endl;


	if (xfeature == "true") {
		/* Send the Xfeature GZIP command.
		*/
		std::cout << "\nExample of enabling compression:\n";
		if (nntp.xfeaturegzip())
			std::cout << "XFEATURE GZIP COMPRESSION enabled.\n";
		else
			std::cerr << "Problem enabling XFEATURE GZIP COMPRESSION.\n";
	}

	if (xover == "true") {
		/* Send the XOVER command which displays an header with
		 * basic info.
		 *
		 * Right now, this is parsed by the nntp class, but in the
		 * future I will make an overloaded function with
		 * the parsed header as an array.
		 */
		std::cout << "\nExample of XOVER command:\n";
		if (nntp.xover(start, end))
			std::cout << "Succesfully downloaded headers with XOVER command.\n";
		else {
			std::cerr << "Problem downloading the header with XOVER.\n";
			return 1;
		}
	}

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
		std::cout << "\nExample of BODY command:\n";
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

	if (head == "true") {
		/* This sends the HEAD command which displays a full article header,
		 * unlike XOVER which only displays the more useful lines
		 * of the article.
		 */
		std::cout << "\nExample of HEAD command:\n";
		if (nntp.head(start))
			std::cout << "Succesfully downloaded an article header with HEAD command.\n";
		else {
			std::cerr << "Problem downloading the article header with HEAD command.\n";
			return 1;
		}
	}

	/* This is just an example to show we can keep the class instance
	 * and disconnect from usenet, then reconnect at a later time.
	 */
	std::cout << "\nExample of disconnecting and reconnecting:\n";
	nntp.disconnect();
	if (nntp.connect(hostname, port, encrypt))
		std::cout << "Connected to usenet.\n";
	else {
		std::cerr << "Problem connecting to usenet.\n";
		return 1;
	}

	std::cout << "\nEnd of second set of examples.\n";
	return 0;
	/* Again we go out of scope, the destructors take care of
	 * the connection to usenet (so disconnect is not needed if you
	 * go out of scope). However if you need require a connection,
	 * and you do nothing with that connection for a long period of
	 * time, it is a good idea to disconnect.
	 */
}

bool readconf(std::string &hostname, std::string &port,
				std::string &username, std::string &password,
				std::string &group, std::string &xover,
				std::string &start, std::string &end,
				std::string &ssl, std::string &body,
				std::string &messageid, std::string &path,
				std::string &posting, std::string &head,
				std::string &xfeature) {

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
				else if (match[1] == "POSTING")
					posting = match[2];
				else if (match[1] == "HEAD")
					head = match[2];
				else if (match[1] == "XFEATURE_GZIP")
					xfeature = match[2];
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
