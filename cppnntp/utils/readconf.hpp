#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <boost/regex.hpp>
#include "boostRegexExceptions.hpp"

namespace cppnntp
{
	class readconf
	{
	public:
		// Run checkconfig.
		readconf();
		~readconf();

		// Return the nntp user.
		std::string nntpuser();

		// Return the nntp password.
		std::string nntppass();

		// Return the nntp host.
		std::string nntphost();

		// Return the nntp port.
		std::string nntpport();

		// Return the nntp ssl setting.
		bool nntpssl();

	private:
		// NNTP settings.
		std::string NNTPUSER = "";
		std::string NNTPPASS = "";
		std::string NNTPHOST = "";
		std::string NNTPPORT = "";
		bool        NNTPSSL  = false;

		// DB settings.
		std::string DBHOST   = "";
		std::string DBPORT   = "";

		// Check if the config file is there if it is, load it, if not create it.
		void checkconfig();

		// Initiate a config file.
		void initiateconfig();

		// Read a config file.
		void readconfig();
	};

	/**
	 * Exceptions for class readconf.
	 */
	class ConfigException : std::runtime_error
	{
		public: ConfigException(const std::string& error) : runtime_error(error) {
		}
	};
}
