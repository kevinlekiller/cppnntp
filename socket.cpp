#include "socket.hpp"
namespace nntp {
	/**
	 * Constructor.
	 *
	 * @public
	 */
	socket::socket() : ssl_sock(NULL), tcp_sock(NULL) {
	}

	/**
	 * Destructor.
	 *
	 * @note Closes the socket connection.
	 * @public
	 */
	socket::~socket() {
		close();
	}

	/**
	 * Toggle compression status.
	 *
	 * @note This is used by class nntp's xfeaturegzip function.
	 * @public
	 *
	 * @param status = Set compression to true or false.
	 */
	void socket::compressionstatus(const bool &status) {
		compression = status;
	}

	/**
	 * Close the socket.
	 *
	 * @public
	 */
	void socket::close() {
		if (tcp_sock != NULL) {
			tcp_sock->close();
			delete tcp_sock;
			tcp_sock = NULL;
		}
		if (ssl_sock != NULL) {
			ssl_sock->lowest_layer().close();
			delete ssl_sock;
			ssl_sock = NULL;
		}
	}

	/**
	 * Are we already connected to usenet?
	 *
	 * @public
	 *
	 * @return bool = Are we?
	 */
	 bool socket::is_connected() {
		if ((ssl_sock != NULL && ssl_sock->lowest_layer().is_open())
			|| (tcp_sock != NULL && tcp_sock->is_open()))
			return true;
		else
			return false;
	}

	/**
	 * Connects to usenet without SSL.
	 *
	 * @public
	 *
	 * @param  hostname = The NNTP server address.
	 * @param      port = The NNTP port.
	 * @return     bool = Did we connect?
	 */
	bool socket::connect(const std::string &hostname, const std::string &port) {
		// Check if we are already connected.
		if (is_connected()) {
			std::cerr << "NNTP Error: Already connected to usenet.\n";
			return true;
		}

		if (port == "443" || port == "563") {
			std::cerr
			<< "NNTP error: Using a SSL port on the non SSL connect function.\n";
			return false;
		}

		// Store boost error codes.
		boost::system::error_code err;
		// Create an endpoint to connnect a socket to and another to compare.
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator, endpoint;
		// Query.
		boost::asio::ip::tcp::resolver::query query(hostname, port);
		// Resolver to resolve the query.
		boost::asio::ip::tcp::resolver resolver(io_service);

		try {
			endpoint_iterator = resolver.resolve(query, err);
		} catch (boost::system::system_error& error) {
			std::cerr << "Resolver error: " << error.what() << std::endl;
			return false;
		}

		if (err) {
			std::cerr << "Boost error: " << err << std::endl;
			return false;
		}

		tcp_sock = new unsecure(io_service);
		// loop through the available endpoints until we can connect without an error
		while (endpoint_iterator != endpoint) {
			// try to connect to the endpoint and do a handshake
			if (!tcp_sock->connect(*endpoint_iterator, err)) {
				// Verify the NNTP response and return.
				try {
					bool done = false;
					std::string resp = "";
					do {
						boost::array<char, 1024> buffer;
						size_t bytesRead = tcp_sock->read_some(boost::asio::buffer(buffer));

						if (resp == "") {
							resp += buffer[0]; resp += buffer[1]; resp += buffer[2];
						}

						if (std::stoi(resp) == RESPONSECODE_READY_POSTING_ALLOWED) {
							posting = true;
							done = true;
						}
						else if (std::stoi(resp) == RESPONSECODE_READY_POSTING_PROHIBITED) {
							posting = false;
							done = true;
						}
						else {
							std::cerr << "Error connecting to usenet: ";
							std::cerr.write(buffer.data(), bytesRead);
							std::cerr << std::endl;
							return false;
						}
					} while (!done);
				} catch (boost::system::system_error& error) {
					std::cerr << "NNTP error: " << error.what() << std::endl;
					return false;
				}
				return true;
			}
			// Close the socket.
			tcp_sock->close();

			// Increment the iterator.
			++endpoint_iterator;
		}
		if (err)
			std::cerr << "Boost error: " << err << std::endl;

		// Unable to connect.
		delete tcp_sock;
		tcp_sock = NULL;
		return false;
	}

	/**
	 * Connects to usenet using SSL.
	 *
	 * @public
	 *
	 * @param  hostname = The NNTP server address.
	 * @param      port = The NNTP port.
	 * @return     bool = Did we connect?
	 */
	bool socket::sslconnect(const std::string &hostname, const std::string &port) {
		// Check if we are already connected.
		if (is_connected()) {
			std::cerr << "NNTP Error: Already connected to usenet.\n";
			return true;
		}

		if (port == "119") {
			std::cerr
			<< "NNTP error: Using a non-SSL port on the SSL connect function.\n";
			return false;
		}

		// Store boost error codes.
		boost::system::error_code err;
		// Create an endpoint to connnect a socket to and another to compare.
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator, endpoint;
		// SSL context.
		boost::asio::ssl::context context(io_service, ssl_context::sslv23);
		// Query.
		boost::asio::ip::tcp::resolver::query query(hostname, port);
		// Resolver to resolve the query.
		boost::asio::ip::tcp::resolver resolver(io_service);

		// Try to resolve the query.
		try {
			endpoint_iterator = resolver.resolve(query, err);
			if (err) {
				std::cerr << "Resolver error: " << err << std::endl;
				return false;
			}
		} catch (boost::system::system_error& error) {
			std::cerr << "Resolver error: " << error.what() << std::endl;
			return false;
		}

		if (err) {
			std::cerr << "Boost error: " << err << std::endl;
			return false;
		}

		ssl_sock = new secure(io_service, context);
		// loop through the available endpoints until we can connect without an error
		while (endpoint_iterator != endpoint) {
			// try to connect to the endpoint and do a handshake
			if (!ssl_sock->lowest_layer().connect(*endpoint_iterator, err)
				&& !ssl_sock->handshake(boost::asio::ssl::stream_base::client, err)) {

				// Verify the NNTP response and return.
				try {
					bool done = false;
					std::string resp = "";
					do {
						boost::array<char, 1024> buffer;
						size_t bytesRead = ssl_sock->read_some(boost::asio::buffer(buffer));

						if (resp == "") {
							resp += buffer[0]; resp += buffer[1]; resp += buffer[2];
						}

						if (std::stoi(resp) == RESPONSECODE_READY_POSTING_ALLOWED) {
							posting = false;
							done = true;
						}
						else if (std::stoi(resp) == RESPONSECODE_READY_POSTING_PROHIBITED) {
							posting = false;
							done = true;
						}
						else {
							std::cout << "Error connecting to usenet: ";
							std::cout.write(buffer.data(), bytesRead);
							std::cout << std::endl;
							return false;
						}
					} while (!done);
				} catch (boost::system::system_error& error) {
					std::cerr << "NNTP error: " << error.what() << std::endl;
					return false;
				}
				return true;
			}
			if (err)
				std::cerr << "Boost error: " << err << std::endl;
			// Close the socket.
			ssl_sock->lowest_layer().close();

			// Increment the iterator.
			++endpoint_iterator;
		}

		// Unable to connect.
		delete ssl_sock;
		ssl_sock = NULL;
		return false;
	}

	/**
	 * Return the private variable posting.
	 *
	 * @public
	 *
	 * @return bool = The value of posting.
	 */
	bool socket::poststatus() {
		return posting;
	}

	/**
	 * Pass a command to usenet.
	 *
	 * @private
	 *
	 * @param  command = The command to pass to usenet.
	 * @return    bool = Did we succeed?
	 */
	bool socket::send_command(const std::string command) {
		if (!is_connected()) {
			std::cerr << "NNTP error: Not connected to usenet.\n";
			return false;
		}

		try {
			// Add return + newline to the command.
			std::string cmd = command + "\r\n";

			// Send the command to usenet.
			if (tcp_sock != NULL)
				tcp_sock->write_some(boost::asio::buffer(cmd.c_str(), strlen(cmd.c_str())));
			else
				ssl_sock->write_some(boost::asio::buffer(cmd.c_str(), strlen(cmd.c_str())));
		} catch (boost::system::system_error& error) {
			std::cerr << "NNTP error: " << error.what() << std::endl;
			return false;
		}
		return true;
	}

	/**
	 * Read a single line response from usenet, return the response
	 * code.
	 *
	 * @note   This is for commands where usenet returns 1 line.
	 * @private
	 *
	 * @return The response code.
	 */
	unsigned short socket::read_reponse() {
		unsigned short code = 0;
		try {
			bool done = false;
			// Create a string to store the response.
			std::string resp = "";
			// Int to store code.
			do {
				// Create an array, max 1024 chars to store the buffer.
				boost::array<char, 1024> buffer;
				// Store the buffer into the array, get the size.
				size_t bytesRead;
				if (tcp_sock != NULL)
					bytesRead = tcp_sock->read_some(boost::asio::buffer(buffer));
				else
					bytesRead = ssl_sock->read_some(boost::asio::buffer(buffer));

				// Prints the buffer sent from usenet.
				std::cout.write(buffer.data(), bytesRead);

				// Get the 3 first chars of the array, the response.
				if (resp == "") {
					resp += buffer[0]; resp += buffer[1]; resp += buffer[2];
					code = std::stoi(resp);
				}

				// Check if we got the response.
				if (code > 99)
					done = true;
				else {
					std::cerr << "NNTP error: ";
					std::cerr.write(buffer.data(), bytesRead);
					std::cerr << std::endl;
					return code;
				}
			} while (!done);
		} catch (boost::system::system_error& error) {
			std::cerr << "NNTP error: " << error.what() << std::endl;
			return code;
		}
		return code;
	}

	/**
	 * Read the line sent back from usenet, check if the code
	 * sent back is good.
	 *
	 * @note   This is for commands where usenet returns 1 line.
	 * @private
	 *
	 * @param  response = The expected response from the NNTP server
	 *                    for the passed command.
	 * @return     bool = Did we succeed?
	 */
	bool socket::read_line(const responsecodes &response) {
		try {
			bool done = false;
			// Create a string to store the response.
			std::string resp = "";
			do {
				// Create an array, max 1024 chars to store the buffer.
				boost::array<char, 1024> buffer;
				// Store the buffer into the array, get the size.
				size_t bytesRead;
				if (tcp_sock != NULL)
					bytesRead = tcp_sock->read_some(boost::asio::buffer(buffer));
				else
					bytesRead = ssl_sock->read_some(boost::asio::buffer(buffer));

				// Prints the buffer sent from usenet.
				std::cout.write(buffer.data(), bytesRead);

				// Get the 3 first chars of the array, the response.
				if (resp == "") {
					resp += buffer[0]; resp += buffer[1]; resp += buffer[2];
				}

				// Check if the response is good (convert resp to int).
				if (std::stoi(resp) == response)
					done = true;
				else {
					std::cerr << "NNTP error: ";
					std::cerr.write(buffer.data(), bytesRead);
					std::cerr << std::endl;
					return false;
				}
			} while (!done);
		} catch (boost::system::system_error& error) {
			std::cerr << "NNTP error: " << error.what() << std::endl;
			return false;
		}
		return true;
	}

	/**
	 * Read the line sent back from usenet, check if the code
	 * sent back is good.
	 *
	 * @note   This is for commands where usenet returns 1 line and
	 * if you require the buffer.
	 * @private
	 *
	 * @param  response = The expected response from the NNTP server
	 *                    for the passed command.
	 * @param  finalbuffer = Pass a string reference to store the buffer.
	 * @return     bool = Did we succeed?
	 */
	bool socket::read_line(const responsecodes &response, std::string &finalbuffer) {
		try {
			bool done = false;
			// Create a string to store the response.
			std::string resp = "";
			do {
				// Create an array, max 1024 chars to store the buffer.
				boost::array<char, 1024> buffer;
				// Store the buffer into the array, get the size.
				size_t bytesRead;
				if (tcp_sock != NULL)
					bytesRead = tcp_sock->read_some(boost::asio::buffer(buffer));
				else
					bytesRead = ssl_sock->read_some(boost::asio::buffer(buffer));

				// Prints the buffer sent from usenet.
				std::cout.write(buffer.data(), bytesRead);

				// Append the current buffer to the final buffer.
				unsigned short iter = 0;
				while (buffer[iter] != '\0') {
					finalbuffer += buffer[iter];
					if (iter++ == (bytesRead-1))
						break;
				}

				// Get the 3 first chars of the array, the response.
				if (resp == "") {
					resp += buffer[0]; resp += buffer[1]; resp += buffer[2];
				}

				// Check if the response is good (convert resp to int).
				if (std::stoi(resp) == response)
					done = true;
				else {
					std::cerr << "NNTP error: ";
					std::cerr.write(buffer.data(), bytesRead);
					std::cerr << std::endl;
					return false;
				}
			} while (!done);
		} catch (boost::system::system_error& error) {
			std::cerr << "NNTP error: " << error.what() << std::endl;
			return false;
		}
		return true;
	}

	/**
	 * Read lines sent back from usenet until we find a period on
	 * the 3rd to last char of the buffer. Then verify the expected
	 * response code.
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
	bool socket::read_lines(const responsecodes &response) {
		// Check if gzip compression is on.
		//if (compression)
			//return read_compressed_lines(response);

		// Read until we find the period.
		try {
			bool done = false;
			std::string resp = "";

			do {
				// Create an array, max 1024 chars to store the buffer.
				boost::array<char, 1024> buffer;

				// Store the buffer into the array, get the array size.
				size_t bytesRead;
				if (tcp_sock != NULL)
					bytesRead = tcp_sock->read_some(boost::asio::buffer(buffer));
				else
					bytesRead = ssl_sock->read_some(boost::asio::buffer(buffer));

				// Prints the buffer sent from usenet.
				std::cout.write(buffer.data(), bytesRead);

				// Get the 3 first chars of the first buffer, the response.
				if (resp == "") {
					resp += buffer[0]; resp += buffer[1]; resp += buffer[2];
					if (std::stoi(resp) != response)
						return false;
				}

				// Look for the terminator (.\r\n)
				if (buffer[bytesRead-3] == '.'
					&& buffer[bytesRead-2] == '\r'
					&& buffer[bytesRead-1] == '\n') {
					// Check if the response is good (convert resp to int).
					if (std::stoi(resp) == response)
						done = true;
					else {
						std::cerr << "NNTP error: Wrong response code from usenet.\n";
						return false;
					}
				}
			} while (!done);

		} catch (boost::system::system_error& error) {
			std::cerr << "NNTP error: " << error.what() << std::endl;
			return false;
		}
		return true;
	}

	/**
	 * Read lines sent back from usenet until we find a period on
	 * the 3rd to last char of the buffer. Then verify the expected
	 * response code.
	 *
	 * @note This is for multi line reponses that end with (.\r\n) and
	 * the calling function requires the buffer.
	 * @private
	 *
	 * @param     response = The expected response from the NNTP server
	 *                       for the passed command.
	 * @param  finalbuffer = Pass a string reference to store the buffer.
	 * @return        bool = Did we succeed?
	 */
	bool socket::read_lines(const responsecodes &response, std::string &finalbuffer) {
		// Check if gzip compression is on.
		//if (compression)
			//return read_compressed_lines(response);

		// Read until we find the period.
		try {
			bool done = false;
			std::string resp = "";
			do {
				// Create an array, max 1024 chars to store the buffer.
				boost::array<char, 1024> buffer;

				// Store the buffer into the array, get the array size.
				size_t bytesRead;
				if (tcp_sock != NULL)
					bytesRead = tcp_sock->read_some(boost::asio::buffer(buffer));
				else
					bytesRead = ssl_sock->read_some(boost::asio::buffer(buffer));

				// Prints the buffer sent from usenet.
				//std::cout.write(buffer.data(), bytesRead);

				// Append the current buffer to the final buffer.
				unsigned short iter = 0;
				while (buffer[iter] != '\0') {
					finalbuffer += buffer[iter];
					if (iter++ == (bytesRead-1))
						break;
				}

				// Get the 3 first chars of the first buffer, the response.
				if (resp == "") {
					resp += buffer[0]; resp += buffer[1]; resp += buffer[2];
					if (std::stoi(resp) != response)
						return false;
				}

				// Look for the terminator (.\r\n)
				if (buffer[bytesRead-3] == '.'
					&& buffer[bytesRead-2] == '\r'
					&& buffer[bytesRead-1] == '\n') {
					// Check if the response is good (convert resp to int).
					if (std::stoi(resp) == response)
						done = true;
					else {
						std::cerr << "NNTP error: Wrong response code from usenet.\n";
						return false;
					}
				}
			} while (!done);
		} catch (boost::system::system_error& error) {
			std::cerr << "NNTP error: " << error.what() << std::endl;
			return false;
		}
		return true;
	}

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
	bool socket::read_compressed_lines(const responsecodes &response) {
		// Read until we find the period.
		try {
		bool done = false;
		std::string resp = "";
		do {
			// Create an array, max 1024 chars to store the buffer.
			boost::array<char, 1024> buffer;
			// Store the buffer into the array, get the size.
			size_t bytesRead;
			if (tcp_sock != NULL)
				bytesRead = tcp_sock->read_some(boost::asio::buffer(buffer));
			else
				bytesRead = ssl_sock->read_some(boost::asio::buffer(buffer));

			// Prints the buffer sent from usenet.
			std::cout.write(buffer.data(), bytesRead);

			// Get the 3 first chars of the array, the response.
			if (resp == "") {
				resp += buffer[0]; resp += buffer[1]; resp += buffer[2];
				if (std::stoi(resp) != response)
					return false;
			}

			// Look for the terminator (.\r\n)
			if (buffer[bytesRead-3] == '.'
				&& buffer[bytesRead-2] == '\r'
				&& buffer[bytesRead-1] == '\n') {
				// Check if the response is good (convert resp to int).
				if (std::stoi(resp) == response)
					done = true;
				else {
					std::cerr << "NNTP error: Wrong response code from usenet.\n";
					return false;
				}
			}
		} while (!done);
		} catch (boost::system::system_error& error) {
			std::cerr << "NNTP error: " << error.what() << std::endl;
			return false;
		}
		return true;
	}
}
