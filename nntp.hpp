#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/system/system_error.hpp>
#include <boost/date_time.hpp>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "socket.hpp"
#include "yencdecode.hpp"

namespace nntp
{
	class nntp
	{
	private:
		/**
		 * Create a socket instance.
		 *
		 * @private
		 */
		socket sock;

		/**
		 * Are we allowed to post to the server?
		 *
		 * @note This is set when we connect to the server.
		 * @private
		 */
		bool posting = false;

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
		void parseheaders(const std::string &finalbuffer);

	public:
		/**
		 * Constructor.
		 *
		 * @public
		 */
		nntp();

		/**
		 * Destructor.
		 *
		 * @note Closes the NNTP connection and the socket.
		 * @public
		 */
		~nntp();

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
		 * the response on the command line.
		 * @public
		 *
		 * @return bool = Did we retrieve the help messages?
		 */
		bool help();

		/**
		 * Send the DATE command.
		 *
		 * @note This sends the DATE command and displays the usenet
		 * server's local date on the command line.
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
	};
}
