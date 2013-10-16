#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/system/system_error.hpp>
#include <boost/date_time.hpp>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "socket.hpp"
#include "yencdecode.hpp"

namespace cppnntp
{
	class nntp
	{
	public:
		/**
		 * Constructor.
		 *
		 * @public
		 *
		 * @param clioutput = Output NNTP responses to CLI.
		 */
		nntp(const bool &clioutput = true);

		/**
		 * Destructor.
		 *
		 * @note Closes the NNTP connection and the socket.
		 * @public
		 */
		~nntp();

		/**
		 * Toggle cli output.
		 * 
		 * @public
		 * 
		 * @param output = Turn cli output on or off.
		 */
		bool clioutput(const bool &output = false);

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
		bool connect(const std::string &hostname, const std::string &port, const bool &ssl);

		/**
		 * Disconnects from usenet.
		 *
		 * @note This passes the QUIT command to usenet, then outputs
		 * the usenet response to the command line.
		 * @public
		 */
		void disconnect();

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
		bool login(const std::string &username, const std::string &password);

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
		bool help();

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
		bool capabilities();

		/**
		 * Send the DATE command.
		 *
		 * @note This sends the DATE command the server returns the
		 * current UTC time.
		 * @public
		 *
		 * @return bool = Did we get the date from the NNTP server?
		 */
		bool date();

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
		bool group(const std::string &groupname);

		/**
		 * Return the total amount of articles for the selected group.
		 *
		 * @public
		 *
		 * @return The total amount of articles.
		 */
		long unsigned group_total();

		/**
		 * Return the oldest article number for the group.
		 *
		 * @public
		 *
		 * @return The oldest article.
		 */
		long unsigned group_oldest();

		/**
		 * Return the newest article number for the group.
		 *
		 * @public
		 *
		 * @return The newest article.
		 */
		long unsigned group_newest();

		/**
		 * Return the name of the selected group.
		 *
		 * @public
		 *
		 * @return The name of the group.
		 */
		std::string group_name();

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
		bool listgroup(const std::string &groupname);

		/**
		 * Send the LISTGROUP command 1 group.
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
		bool listgroup(const std::string &groupname,
					const std::string &anumber, const bool &direction);

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
		bool listgroup(const std::string &groupname, const std::string &start,
						const std::string &end);

		/**
		 * Send LIST ACTIVE command which displays a list of all
		 * groups with their article numbers and if we can post
		 * in them or not.
		 *
		 * @public
		 *
		 * @return bool = Did we get the list of groups?
		 */
		bool listactive();

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
		bool listactive(const std::string &wildmat);

		/**
		 * Send LIST ACTIVE.TIMES command which displays a list of all
		 * groups with their created time and creator.
		 *
		 * @public
		 *
		 * @return bool = Did we get the list of groups?
		 */
		bool listactivetimes();

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
		bool listactivetimes(const std::string &wildmat);

		/**
		 * Send LIST NEWSGROUPS command which displays a list of all
		 * groups with their descriptions.
		 *
		 * @public
		 *
		 * @return bool = Did we get the list of groups?
		 */
		bool listnewsgroups();

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
		bool listnewsgroups(const std::string &wildmat);

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
		bool newgroups(const std::string &date, const std::string &time);

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
		bool newnews(const std::string &date, const std::string &time);

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
		bool stat(const std::string &anumber);

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
		bool last();

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
		bool next();

		/**
		 * Send ARTICLE command for 1 article number or message-id.
		 *
		 * @note This passes the ARTICLE command for 1 article number
		 * or message-id, then displays the article (message + header)
		 * on the command line.
		 * @public
		 *
		 * @todo Decode yEnc messages?
		 *
		 * @param  anumber = The number or message-id of the article.
		 * @return    bool = Did we receive the article?
		 */
		bool article(const std::string &anumber);

		/**
		 * Send the BODY command for 1 article number or message-id.
		 *
		 * @note This passes the BODY command for 1 article number or
		 * message-id, then displays the article message on the
		 * command line.
		 * @public
		 *
		 * @todo Decode yEnc messages?
		 *
		 * @param  anumber = The number or message-id of the body.
		 * @return    bool = Did we receive the body?
		 */
		bool body(const std::string &anumber);

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
		bool body(const std::string &anumber, std::string &data,
						const std::string &store = "");

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
		bool head(const std::string &anumber);

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
		bool xover(const std::string &anumber);

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
		bool xover(const std::string &start, const std::string &end);

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
		bool xover(const std::string &anumber, bool &direction);

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
		bool post(const std::string &from, const std::string &groups,
						const std::string &subject, std::string &message);

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
		bool xfeaturegzip();

	private:
		/**
		 * Output NNTP responses to cli output.
		 *
		 * @private
		 */
		bool echocli = true;

		/**
		 * Create a socket instance.
		 *
		 * @private
		 */
		socket sock;

		/**
		 * Did we already parse the overview format?
		 *
		 * @private
		 */
		bool overviewfmtparsed = false;

		/**
		 * Is the server's overview format specifications the default
		 * RFC2980/RFC3977 format, or does it contain extra lines?
		 *
		 * @note If it has extra lines, XOVER/OVER will not be parsed,
		 * it will just be printed on the screen.
		 * @private
		 */
		bool overviewfmtspec = false;

		/**
		 * Send the LIST OVERVIEW.FMT command which gets the format
		 * of the returned XOVER/OVER headers.
		 *
		 * @private
		 *
		 * @return bool = Did we receive the overview format?
		 */
		bool overviewformat();

		/**
		 * Parse response from LIST OVERVIEW.FMT
		 *
		 * @note See overviewfmtspec above.
		 * @private
		 *
		 * @param finalbuffer = The buffer to parse.
		 */
		void parseoverviewfmt(std::string &finalbuffer);

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
		void parseheaders(std::string &finalbuffer);

		/**
		 * Parse response from GROUP command.
		 *
		 * @note This takes the response from a GROUP command and
		 * stores the results as objects.
		 * @private
		 *
		 * @param   finalbuffer = The buffer reference.
		 */
		void parsegroup(const std::string &finalbuffer);

		/* Group objects for the currently selected group follow.
		 */
		/**
		 * Did we select a group?
		 *
		 * @private
		 */
		bool groupselected = false;

		/**
		 * Total amount of articles in the currently selected group.
		 *
		 * @private
		 */
		unsigned long grouptotal;

		/**
		 * Oldest article number in the currently selected group.
		 *
		 * @private
		 */
		unsigned long groupoldest;

		/**
		 * Newest article number in the currently selected group.
		 *
		 * @private
		 */
		unsigned long groupnewest;

		/**
		 * Name of the currently selected group.
		 *
		 * @private
		 */
		std::string groupname;
	};

	/**
	 * Exceptions for class nntp.
	 */
	class NNTPException : std::runtime_error
	{
		public: NNTPException(const std::string& error) : runtime_error(error) {
		}
	};
}
