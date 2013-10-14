#include "nntp.hpp"

namespace cppnntp {
	/**
	 * Constructor.
	 *
	 * @public
	 */
	nntp::nntp() {
	}

	/**
	 * Destructor.
	 *
	 * @note Closes the NNTP connection and the socket.
	 * @public
	 */
	nntp::~nntp() {
		disconnect();
	}

	/**
	 * Connects to usenet.
	 *
	 * @note This passes the authinfo user and passwords command
	 * to usenet, then outputs the response to the command line.
	 * @public
	 *
	 * @param  hostname = The NNTP server address.
	 * @param      port = The NNTP port.
	 * @param       ssl = Does this connection require SSL?
	 * @return     bool = Did we connect?
	 */
	bool nntp::connect(const std::string &hostname,
						const std::string &port, const bool &ssl) {
		// If a SSL connection was requested, connect using SSL.
		if (ssl) {
			if (!sock.sslconnect(hostname, port))
				return false;
		}
		else {
			if (!sock.connect(hostname, port))
				return false;
		}
		return true;
	}

	/**
	 * Disconnects from usenet.
	 *
	 * @note This passes the QUIT command to usenet, then outputs
	 * the usenet response to the command line.
	 * @public
	 */
	void nntp::disconnect() {
		// Tell usenet we want to disconnect.
		if (sock.is_connected()) {
			sock.send_command("QUIT");
			sock.read_line(RESPONSECODE_DISCONNECTING_REQUESTED);
		}
		// Set groupselected back to false.
		groupselected = false;
		// Set compression flag in socket to false.
		sock.togglecompression(false);
		sock.close();
	}

	/**
	 * Authenticate to usenet.
	 *
	 * @note This passes authinfo user and password to the NNTP server,
	 * then outputs the usenet response to the command line.
	 * @public
	 *
	 * @param  username = The username for the NNTP server.
	 * @param  password = The password for the user.
	 * @return     bool = Did we logon succesfully?
	 */
	bool nntp::login(const std::string &username, const std::string &password) {
		// Send the username.
		if (!sock.send_command("AUTHINFO USER " + username))
			return false;

		if (!sock.read_line(RESPONSECODE_AUTHENTICATION_CONTINUE))
			return false;

		// Send the password.
		if (!sock.send_command("AUTHINFO PASS " + password))
			return false;

		if (!sock.read_line(RESPONSECODE_AUTHENTICATION_ACCEPTED))
			return false;

		return true;
	}

	/**
	 * Send the HELP command.
	 *
	 * @note This sends the HELP command to usenet and displays
	 * the response on the command line. This displays the list
	 * of available commands on the server.
	 * @public
	 *
	 * @return bool = Did we retrieve the help messages?
	 */
	bool nntp::help() {
		if (!sock.send_command("HELP"))
			return false;

		if (!sock.read_lines(RESPONSECODE_HELP_FOLLOWS))
			return false;

		return true;
	}

	/**
	 * Send the CAPABILITIES command.
	 *
	 * @note This sends the CAPABILITIES command to usenet and
	 * displays the response on the command line. This is a
	 * list of capabilities on the server.
	 * @public
	 *
	 * @return bool = Did we retrieve the capabilities list?
	 */
	bool nntp::capabilities() {
		if (!sock.send_command("CAPABILITIES"))
			return false;

		if (!sock.read_lines(RESPONSECODE_CAPABILITIES_FOLLOW))
			return false;

		return true;
	}

	/**
	 * Send the DATE command.
	 *
	 * @note This sends the DATE command the server returns the
	 * current UTC time.
	 * @public
	 *
	 * @return bool = Did we get the date from the NNTP server?
	 */
	 bool nntp::date() {
		if (!sock.send_command("DATE"))
			return false;

		if (!sock.read_line(RESPONSECODE_SERVER_DATE))
			return false;

		return true;
	}

	/**
	 * Send the GROUP command for a group.
	 *
	 * @note This passes the GROUP command for a single goup
	 * to usenet and displays the overview information for that group
	 * on the command line.
	 * @public
	 *
	 * @param  group = The name of the group.
	 * @return  bool = Did we get the group overview?
	 */
	bool nntp::group(const std::string &groupname) {
		if (!sock.send_command("GROUP " + groupname))
			return false;

		std::string finalbuffer = "";
		if (!sock.read_line(RESPONSECODE_GROUP_SELECTED, finalbuffer))
			return false;

		groupselected = true;
		// Parse and store the data as objects.
		parsegroup(finalbuffer);

		return true;
	}

	/**
	 * Return the total amount of articles for the selected group.
	 *
	 * @public
	 *
	 * @return The total amount of articles.
	 */
	long unsigned nntp::group_total() {
		if (groupselected)
			return grouptotal;
		else {
			throw NNTPException("No group selected.");
			return 0;
		}
	}

	/**
	 * Return the oldest article number for the group.
	 *
	 * @public
	 *
	 * @return The oldest article.
	 */
	long unsigned nntp::group_oldest() {
		if (groupselected)
			return groupoldest;
		else {
			throw NNTPException("No group selected.");
			return 0;
		}
	}

	/**
	 * Return the newest article number for the group.
	 *
	 * @public
	 *
	 * @return The newest article.
	 */
	long unsigned nntp::group_newest() {
		if (groupselected)
			return groupnewest;
		else {
			throw NNTPException("No group selected.");
			return 0;
		}
	}

	/**
	 * Return the name of the selected group.
	 *
	 * @public
	 *
	 * @return The name of the group.
	 */
	std::string nntp::group_name() {
		if (groupselected)
			return groupname;
		else {
			throw NNTPException("No group selected.");
			return "";
		}
	}

	/**
	 * Send the LISTGROUP command 1 group.
	 *
	 * @note This passes the LISTGROUP command for a single group
	 * to usenet and displays the overview information
	 * and a list of all the article numbers for that group
	 * on the command line.
	 * @public
	 *
	 * @param group = The name of the group.
	 * @return bool = Did we get the group info?
	 */
	bool nntp::listgroup(const std::string &groupname) {
		if (!sock.send_command("LISTGROUP " + groupname))
			return false;

		if (!sock.read_lines(RESPONSECODE_GROUP_SELECTED))
			return false;

		return true;
	}

	/**
	 * Send the LISTGROUP command 1 group with an article and a direction.
	 *
	 * @note This passes the LISTGROUP command for a single group
	 * to usenet and displays the overview information
	 * and a list of all the article numbers for that group
	 * on the command line newer or older than the supplied
	 * article number or message-id.
	 * @public
	 *
	 * @param     group = The name of the group.
	 * @param   anumber = The article number or message-id.
	 * @param direction = True: Articles newer than anumber, False:
	 * articles older than anumber.
	 * @return bool = Did we get the group info?
	 */
	bool nntp::listgroup(const std::string &groupname,
				const std::string &anumber, const bool &direction) {
		if (direction) {
			if (!sock.send_command("LISTGROUP " + groupname + " " + anumber + '-'))
				return false;
		}
		else {
			if (!sock.send_command("LISTGROUP " + groupname + " -" + anumber))
				return false;
		}

		if (!sock.read_lines(RESPONSECODE_GROUP_SELECTED))
			return false;

		return true;
	}

	/**
	 * Send LISTGROUP command for 1 group with a range of
	 * article numbers.
	 *
	 * @note This passes the LISTGROUP command for a single group
	 * to usenet and displays the overview information
	 * and a list of all the article numbers between the
	 * specified range for that group on the command line.
	 * @public
	 *
	 * @param group = The name of the group.
	 * @param start = The oldest article number for the range.
	 * @param   end = The newest article number for the range.
	 * @return bool = Did we get the group info?
	 */
	bool nntp::listgroup(const std::string &groupname, const std::string &start,
					const std::string &end) {
		if (!sock.send_command("LISTGROUP " + groupname + " " + start + '-' + end))
			return false;

		if (!sock.read_lines(RESPONSECODE_GROUP_SELECTED))
			return false;

		return true;
	}

	/**
	 * Send LIST ACTIVE command which displays a list of all
	 * groups with their article numbers and if we can post
	 * in them or not.
	 * 
	 * @public
	 * 
	 * @return bool = Did we get the list of groups?
	 */
	bool nntp::listactive() {
		if (!sock.send_command("LIST ACTIVE"))
			return false;

		if (!sock.read_lines(RESPONSECODE_LIST_RESPONSE))
			return false;

		return true;
	}

	/**
	 * Send LIST ACTIVE command which displays a list of groups
	 * matching the wildmat (search query), with first and last
	 * article numbers, and if we can post in them or not.
	 * 
	 * @public
	 * 
	 * @param wildmat = (See RFC3977 for detailed info) Allows you
	 * to search for groups (example: binaries*,*linux)
	 * @return   bool = Did we get the list of groups?
	 */
	bool nntp::listactive(const std::string &wildmat) {
		if (!sock.send_command("LIST ACTIVE " + wildmat))
			return false;

		if (!sock.read_lines(RESPONSECODE_LIST_RESPONSE))
			return false;

		return true;
	}

	/**
	 * Send LIST ACTIVE.TIMES command which displays a list of all
	 * groups with their created time and creator.
	 * 
	 * @public
	 * 
	 * @return bool = Did we get the list of groups?
	 */
	bool nntp::listactivetimes() {
		if (!sock.send_command("LIST ACTIVE.TIMES"))
			return false;

		if (!sock.read_lines(RESPONSECODE_LIST_RESPONSE))
			return false;

		return true;
	}

	/**
	 * Send LIST ACTIVE.TIMES command which displays a list of groups
	 * matching the wildmat (search query), with their created time
	 * and creator.
	 * 
	 * @public
	 * 
	 * @param wildmat = (See RFC3977 for detailed info) Allows you
	 * to search for groups (example: binaries*,*linux)
	 * @return   bool = Did we get the list of groups?
	 */
	bool nntp::listactivetimes(const std::string &wildmat) {
		if (!sock.send_command("LIST ACTIVE.TIMES " + wildmat))
			return false;

		if (!sock.read_lines(RESPONSECODE_LIST_RESPONSE))
			return false;

		return true;
	}

	/**
	 * Send LIST NEWSGROUPS command which displays a list of all
	 * groups with their descriptions.
	 * 
	 * @public
	 * 
	 * @return bool = Did we get the list of groups?
	 */
	bool nntp::listnewsgroups() {
		if (!sock.send_command("LIST NEWSGROUPS"))
			return false;

		if (!sock.read_lines(RESPONSECODE_LIST_RESPONSE))
			return false;

		return true;
	}

	/**
	 * Send LIST NEWSGROUPS command which displays a list of groups
	 * matching the wildmat (search query), with their descriptions.
	 * 
	 * @public
	 * 
	 * @param wildmat = (See RFC3977 for detailed info) Allows you
	 * to search for groups (example: binaries*,*linux)
	 * @return   bool = Did we get the list of groups?
	 */
	bool nntp::listnewsgroups(const std::string &wildmat) {
		if (!sock.send_command("LIST NEWSGROUPS " + wildmat))
			return false;

		if (!sock.read_lines(RESPONSECODE_LIST_RESPONSE))
			return false;

		return true;
	}

	/**
	 * Send NEWGROUPS command which displays a list of groups
	 * since the specified UTC(GMT) time.
	 * 
	 * @public
	 * @example       newgroups("20131013", "143200");
	 * 
	 * @param  date = The date in this format: yyyymmdd
	 * @param  time = The time in this format: hhmmss
	 * @return bool = Did we get the list of groups?
	 */
	bool nntp::newgroups(const std::string &date, const std::string &time) {
		if (!sock.send_command("NEWGROUPS " + date + " " + time + " GMT"))
			return false;

		if (!sock.read_lines(RESPONSECODE_NEW_GROUPS_FOLLOW))
			return false;

		return true;
	}

	/**
	 * Send NEWNEWS command which displays of message-id's for
	 * the selected group since the specified UTC(GMT) time.
	 * 
	 * @public
	 * @example       newnews("20131013", "143200");
	 * 
	 * @param  date = The date in this format: yyyymmdd
	 * @param  time = The time in this format: hhmmss
	 * @return bool = Did we get the list of message-ids?
	 */
	bool nntp::newnews(const std::string &date, const std::string &time) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("NEWNEWS " + date + " " + time + " GMT"))
			return false;

		if (!sock.read_lines(RESPONSECODE_NEW_ARTICLES_FOLLOW))
			return false;

		return true;
	}

	/**
	 * Send STAT command for 1 article number or message-id.
	 *
	 * @note This passes the STAT command for 1 article number
	 * or message-id, then displays the output on the command
	 * line (the output of this command will tell you if the
	 * article exists or not.).
	 * @public
	 *
	 * @param  anumber = The number or message-id of the article.
	 * @return    bool = Does the article exist, or was there
	 *                   a problem sending the command?
	 */
	bool nntp::stat(const std::string &anumber) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("STAT " + anumber))
			return false;

		if (!sock.read_lines(RESPONSECODE_ARTICLE_SELECTED))
			return false;

		return true;
	}

	/**
	 * Send LAST command.
	 * 
	 * @note This passes the LAST command to the NNTP server,
	 * you must pass the GROUP command first, and a NEXT or STAT
	 * command after that. It will display the previous article
	 * before the NEXT or STAT commands (as long as there is a 
	 * previous article in that group).
	 * @public
	 * 
	 * @return bool = Did we receive the article?
	 */
	bool nntp::last() {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("LAST"))
			return false;

		if (!sock.read_lines(RESPONSECODE_ARTICLE_SELECTED))
			return false;

		return true;
	}

	/**
	 * Send NEXT command.
	 * 
	 * @note This passes the NEXT command to the NNTP server,
	 * you must pass the GROUP command first, if the group has
	 * more then 1 article it will select the second article,
	 * you can also use this after a STAT or LAST command (as long
	 * as there is a next article in the group).
	 * @public
	 * 
	 * @return bool = Did we receive the article?
	 */
	bool nntp::next() {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("NEXT"))
			return false;

		if (!sock.read_lines(RESPONSECODE_ARTICLE_SELECTED))
			return false;

		return true;
	}

	/**
	 * Send ARTICLE command for 1 article number or message-id.
	 *
	 * @note This passes the ARTICLE command for 1 article number
	 * or message-id, then displays the article (message + header)
	 * on the command line.
	 * @public
	 *
	 * @param  anumber = The number or message-id of the article.
	 * @return    bool = Did we receive the article?
	 */
	bool nntp::article(const std::string &anumber) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("ARTICLE " + anumber))
			return false;

		if (!sock.read_lines(RESPONSECODE_ARTICLE_FOLLOWS))
			return false;

		return true;
	}

	/**
	 * Send the BODY command for 1 article number or message-id.
	 *
	 * @note This passes the BODY command for 1 article number or
	 * message-id, then displays the article message on the
	 * command line.
	 * @public
	 *
	 * @param  anumber = The number or message-id of the body.
	 * @return    bool = Did we receive the body?
	 */
	bool nntp::body(const std::string &anumber) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("BODY " + anumber))
			return false;

		if (!sock.read_lines(RESPONSECODE_BODY_FOLLOWS))
			return false;

		return true;
	}

	/**
	 * Send the BODY command for 1 article number or message-id.
	 * Attempts to decode yEnc data.
	 *
	 * @note This passes the BODY command for 1 article number or
	 * message-id, then displaysthen attempts to decode the yEnc data.
	 * @public
	 *
	 * @param  anumber = The number or message-id of the body.
	 * @param     data = The decoded data.
	 * @param    store = (Optional) Path/file to store the decoded
	 * data (example: /temp/data.bin || /home/kevin/data.rar).
	 * @return    bool = Did we receive the yEnc data?
	 */
	bool nntp::body(const std::string &anumber, std::string &data,
					const std::string &store) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("BODY " + anumber))
			return false;

		std::string finalbuffer = "";
		if (!sock.read_lines(RESPONSECODE_BODY_FOLLOWS, finalbuffer))
			return false;

		yencdecode yd;
		if (!yd.decodeyencstring(finalbuffer, data))
			return false;

		if (store != "") {
			// Create a file to store the data.
			std::ofstream outfile;
			outfile.open(store);
			if (!outfile.is_open())
				return false;

			outfile << data;
			outfile.close();
		}
		return true;
	}

	/**
	 * Send the HEAD command for 1 article number or message-id.
	 *
	 * @note This passes the HEAD command for 1 article number or
	 * message-id, then displays the article header on the
	 * command line.
	 * @public
	 *
	 * @param  anumber = The number or message-id of the header.
	 * @return    bool = Did we receive the header?
	 */
	bool nntp::head(const std::string &anumber) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("HEAD " + anumber))
			return false;

		if (!sock.read_lines(RESPONSECODE_HEAD_FOLLOWS))
			return false;

		return true;
	}

	/**
	 * Send the XOVER command for 1 article number or message-id.
	 *
	 * @note This passes the XOVER command for 1 article number or
	 * message-id, then displays the article header (overview)
	 * on the command line.
	 * @public
	 *
	 * @param  anumber = The number or message-id of the article.
	 * @return    bool = Did we receive the header?
	 */
	bool nntp::xover(const std::string &anumber) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("XOVER " + anumber)) {
			// Try using OVER instead.
			if (!sock.send_command("OVER " + anumber))
				return false;
		}

		std::string finalbuffer = "";
		if (!sock.read_lines(RESPONSECODE_OVERVIEW_FOLLOWS, finalbuffer, true))
			return false;

		if (finalbuffer != "")
			parseheaders(finalbuffer);
		return true;
	}

	/**
	 * Send the XOVER command for a range of article numbers.
	 *
	 * @note This passes the XOVER command for multiple article
	 * numbers, then displays the article headers (overview)
	 * on the command line.
	 * @public
	 *
	 * @param  start = The oldest wanted article.
	 * @param    end = The newest wanted article.
	 * @return  bool = Did we receive the headers?
	 */
	bool nntp::xover(const std::string &start, const std::string &end) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		if (!sock.send_command("XOVER " + start + '-' + end)) {
			// Try using OVER instead.
			if (!sock.send_command("OVER " + start + '-' + end))
				return false;
		}

		std::string finalbuffer = "";
		if (!sock.read_lines(RESPONSECODE_OVERVIEW_FOLLOWS, finalbuffer, true))
			return false;

		if (finalbuffer != "")
			parseheaders(finalbuffer);
		return true;
	}

	/**
	 * Send the XOVER command for an article number
	 * (and all above or below).
	 *
	 * @note This passes the XOVER command for an article number or
	 * message-id and a direction (all older than that article,
	 * or all newer), then displays them on the command line.
	 * @public
	 *
	 * @param    anumber = The start article number.
	 * @param  direction = The direction, true to get all articles
	 * newer than anumber, false to get all articles older than anumber.
	 * @return      bool = Did we receive the headers?
	 */
	bool nntp::xover(const std::string &anumber, bool &direction) {
		if (!groupselected) {
			throw NNTPException("No group selected.");
			return false;
		}

		std::string cmd;
		if (direction)
			cmd = "XOVER " + anumber + '-';
		else
			cmd = "XOVER -" + anumber;

		if (!sock.send_command(cmd)) {
			// Try sending a OVER command instead.
			if (direction)
				cmd = "OVER " + anumber + '-';
			else
				cmd = "OVER -" + anumber;
			
			if (!sock.send_command(cmd))
				return false;
		}

		std::string finalbuffer = "";
		if (!sock.read_lines(RESPONSECODE_OVERVIEW_FOLLOWS, finalbuffer, true))
			return false;

		if (finalbuffer != "")
			parseheaders(finalbuffer);
		return true;
	}

	/**
	 * Post an article to usenet.
	 * 
	 * @note Posts a single article, if you don't have posting
	 * rights, this will return false.
	 * @public
	 * 
	 * @param from = The person who is posting the message.
	 * @example = "Demo User" <nobody@example.net>
	 * 
	 * @param groups = The group or list of groups.
	 * @example = alt.binaries.test
	 * @example = alt.binaries.test,alt.binaries.misc
	 * 
	 * @param subject = The subject of the article.
	 * @example = I am just a test article
	 * 
	 * @param message = The message (body) of the article (you end it
	 * with CRLF(\r\n)).
	 * @example = This is just a test article.\r\n
	 * @note Usenet has a limit of 512 chars per line when sending
	 * a command (CRLF counts as 2 chars), if your message will
	 * be longer than this you need to seperate each line with CRLF
	 * 
	 * @return bool = Did the server receive the article?
	 */
	bool nntp::post(const std::string &from, const std::string &groups,
					const std::string &subject, std::string &message) {

		// Send the command to usenet we want to post.
		if (!sock.send_command("POST"))
			return false;

		// Check if the response is good.
		if (!sock.read_line(RESPONSECODE_POSTING_SEND))
			return false;

		std::string CRLF = "\r\n";

		// Check if message ends in CRLF.
		if (message[(message.length() - 2)] != '\r' && message[(message.length() - 1)] != '\n') {
			message += CRLF;
		}

		// Try to send the article.
		if (!sock.send_command
				(
					"FROM: " + from + CRLF +
					"NEWSGROUPS: " + groups + CRLF +
					"SUBJECT: " + subject + CRLF +
					"X-POSTER: cppnntlib" + CRLF + CRLF +
					message + "."
				)
			) {
			return false;
		}

		// Check if the response is good.
		if (!sock.read_line(RESPONSECODE_POSTING_SUCCESS))
			return false;

		return true;
	}

	/**
	 * Send XFEATURE COMPRESS GZIP command.
	 *
	 * @note Not currently functional.
	 * @note This passes the XFEATURE COMPRESS GZIP command
	 * which enables gzip compression on article headers.
	 * @public
	 *
	 * @return bool = Does the server recognize the command?
	 */
	bool nntp::xfeaturegzip() {
		if (!sock.send_command("XFEATURE COMPRESS GZIP"))
			return false;

		if (!sock.read_line(RESPONSECODE_X_COMMAND_SUCCESS))
			return false;

		// Enable the compression flag in socket.
		sock.togglecompression(true);
		return true;
	}

	/**
	 * Send the LIST OVERVIEW.FMT command which gets the format
	 * of the returned XOVER/OVER headers, then we parse it.
	 * 
	 * @private
	 * 
	 * @return bool = Did we receive the overview format?
	 */
	bool nntp::overviewformat() {
		// Check if we already parsed it.
		if (overviewfmtparsed)
			return true;

		// Send the command to usenet we want to post.
		if (!sock.send_command("LIST OVERVIEW.FMT"))
			return false;

		// Check if the response is good.
		std::string finalbuffer = "";
		if (!sock.read_lines(RESPONSECODE_LIST_RESPONSE, finalbuffer))
			return false;

		// Parse the overviewformat.
		parseoverviewfmt(finalbuffer);
		return true;
	}

	/**
	 * Parse response from LIST OVERVIEW.FMT
	 * 
	 * @note See overviewfmtspec in header file.
	 * @private
	 * 
	 * @param finalbuffer = The buffer to parse.
	 */
	void nntp::parseoverviewfmt(std::string &finalbuffer) {
		// We parsed the overview, so set to true.
		overviewfmtparsed = true;
		unsigned short lines = 0;
		// Loop over the buffer, count the lines.
		for (unsigned short i = 0; i < finalbuffer.length(); i++) {
			if (finalbuffer[i] == '\r') {
				if (lines++ > 11)
					break;
			}
		}
		// If there are 10 lines, it's probably to spec.
		if (lines == 10)
			overviewfmtspec = true;
	}

	/**
	 * Parse response from XOVER.
	 *
	 * @note This goes over the headers returned by usenet
	 * when passing an XOVER command, it splits the headers
	 * into readable text.
	 * @private
	 *
	 * @param   finalbuffer = The buffer reference.
	 */
	void nntp::parseheaders(std::string &finalbuffer) {
		// Check if we have the overview fmt, if we can't get it, print the buffer.
		if (!overviewformat()) {
			std::cout << finalbuffer;
			return;
		}

		// If the server's overview fmt is not default spec, just print it.
		if (!overviewfmtspec) {
			std::cout << finalbuffer;
			return;
		}

		bool respfound = false;

		// Loop over the buffer and parse the header lines.
		std::string respline, curheader = "";
		for (unsigned long i = 0; i < (finalbuffer.length() - 2); i++) {

			// Get the response, it's the first line.
			if (!respfound) {

				if (finalbuffer[i] != '\r')
					respline += finalbuffer[i];
				else {
					std::cout << "Response: " << respline << std::endl;
					respfound = true;
				}
			}
			// Go over the header lines.
			else {

				// Remove the new lines.
				if (finalbuffer[i] == '\n')
					continue;

				// Tack on chars to curheader until we find cr.
				if (finalbuffer[i] != '\r')
					curheader += finalbuffer[i];
				else {

					// Add on a tab to the end to get the xref.
					curheader += '\t';

					unsigned long linenumber = 1;
					std::string curline = "";

					// Loop over curheader, add chars until we find a tab.
					for (unsigned long it = 0; it <= curheader.length(); it++) {

						if (curheader[it] != '\t')
							curline += curheader[it];
						else {

							// Print the line type.
							switch (linenumber++) {
								case 1:
									std::cout << "Number: ";
									break;
								case 2:
									std::cout << "Subject: ";
									break;
								case 3:
									std::cout << "From: ";
									break;
								case 4:
									std::cout << "Date: ";
									break;
								case 5:
									std::cout << "Message-ID: ";
									break;
								case 6:
									std::cout << "References: ";
									break;
								case 7:
									std::cout << "Bytes: ";
									break;
								case 8:
									std::cout << "Lines: ";
									break;
								case 9:
									// Reset the line number.
									linenumber = 0;
									break;
							}

							// Print the current line.
							std::cout << curline << std::endl;
							// Reset the current line.
							curline = "";
						}
					}
					// Reset the header line.
					curheader = "";
				}
			}
		}
	}

	/**
	 * Parse response from GROUP command.
	 * 
	 * @note This takes the response from a GROUP command and
	 * stores the results as objects.
	 * @private
	 * 
	 * @param   finalbuffer = The buffer reference.
	 */
	void nntp::parsegroup(const std::string &finalbuffer) {
		unsigned short line = 0;
		std::string curline = "";
		// Loop over every char in the buffer.
		for (unsigned short i = 0; i < (finalbuffer.length() - 2); i++) {
			// Skip spaces.
			if (finalbuffer[i] == 32) {
				switch (line++) {
					// Skip the response code.
					case 0:
						break;
					// Total amount of articles.
					case 1:
						grouptotal = std::stoi(curline);
						break;
					// Oldest article.
					case 2:
						groupoldest = std::stoi(curline);
						break;
					// Newest article.
					case 3:
						groupnewest = std::stoi(curline);
						break;
				}
				curline = "";
			}
			else {
				curline += finalbuffer[i];
			}
		}
		// Name of the group.
		groupname = curline;
	}
}
