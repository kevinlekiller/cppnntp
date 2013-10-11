#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include "responsecodes.hpp"

namespace nntp
{
	typedef boost::asio::ip::tcp::socket       unsecure;
	typedef boost::asio::ssl::stream<unsecure> secure;
	typedef boost::asio::ssl::context          ssl_context;

	class socket
	{
	private:
		/**
		 * Asio io_service, required for passing tasks to the socket.
		 *
		 * @private
		 */
		boost::asio::io_service io_service;

		/**
		 * SSL socket.
		 *
		 * @private
		 */
		secure *ssl_sock;

		/**
		 * Non-SSL socket.
		 *
		 * @private
		 */
		unsecure *tcp_sock;

		/**
		 * Has the server given us access to post articles?
		 *
		 * @private
		 */
		bool posting = false;

		/**
		 * Used for xfeature gzip compression.
		 *
		 * @private
		 */
		bool compression = false;

	public:
		/**
		 * Constructor.
		 *
		 * @public
		 */
		socket();

		/**
		 * Destructor.
		 *
		 * @note Closes the socket connection.
		 * @public
		 */
		~socket();

		/**
		 * Toggle compression status.
		 *
		 * @note This is used by class nntp's xfeaturegzip function.
		 * @public
		 *
		 * @param status = Set compression to true or false.
		 */
		void compressionstatus(const bool &status);

		/**
		 * Close the socket.
		 *
		 * @public
		 */
		void close();

		/**
		 * Are we already connected to usenet?
		 *
		 * @public
		 *
		 * @return bool = Are we?
		 */
		bool is_connected();

		/**
		 * Connects to usenet without SSL.
		 *
		 * @public
		 *
		 * @param  hostname = The NNTP server address.
		 * @param      port = The NNTP port.
		 * @return     bool = Did we connect?
		 */
		bool connect(const std::string &hostname, const std::string &port = "119");

		/**
		 * Connects to usenet using SSL.
		 *
		 * @public
		 *
		 * @param  hostname = The NNTP server address.
		 * @param      port = The NNTP port.
		 * @return     bool = Did we connect?
		 */
		bool sslconnect(const std::string &hostname, const std::string &port = "563");

		/**
		 * Return the private variable posting.
		 *
		 * @public
		 *
		 * @return bool = The value of posting.
		 */
		bool poststatus();

		/**
		 * Pass a command to usenet.
		 *
		 * @private
		 *
		 * @param  command = The command to pass to usenet.
		 * @return    bool = Did we succeed?
		 */
		bool send_command(const std::string command);

		/**
		 * Read the line sent back from usenet, check if the code
		 * sent back is good.
		 *
		 * @note   This is for commands that return 1 line.
		 * @private
		 *
		 * @param  response = The expected response from the NNTP server
		 *                    for the passed command.
		 * @return     bool = Did we succeed?
		 */
		bool read_line(const responsecodes &response);

		/**
		 * Read lines sent back from usenet until we find a period on
		 * the 3rd to last char of the buffer. Then verify the
		 * expected response code.
		 *
		 * @note This is for multi line reponses that end with (.\r\n),
		 * this only prints the buffer to screen, if you need the
		 * buffer, see the overloaded function.
		 * @private
		 *
		 * @param  response = The expected response from the NNTP server
		 *                    for the passed command.
		 * @return     bool = Did we succeed?
		 */
		bool read_lines(const responsecodes &response);

		/**
		 * Read lines sent back from usenet until we find a period on
		 * the 3rd to last char of the buffer. Then verify the
		 * expected response code.
		 *
		 * @note This is for multi line reponses that end with (.\r\n)
		 * and the calling function requires the buffer.
		 * @private
		 *
		 * @param     response = The expected response from the NNTP server
		 *                       for the passed command.
		 * @param  finalbuffer = Pass a string reference to store the buffer.
		 * @return        bool = Did we succeed?
		 */
		bool read_lines(const responsecodes &response, std::string &finalbuffer);

		/**
		 * Read lines sent back from usenet used when using gzip compress.
		 *
		 * @note Not started working on this yet.
		 * @private
		 *
		 * @param  response = The expected response from the NNTP server
		 *                    for the passed command.
		 * @return     bool = Did we succeed?
		 */
		bool read_compressed_lines(const responsecodes &response);
	};
}