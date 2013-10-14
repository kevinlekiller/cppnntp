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
		/**
		 * Constructor.
		 * 
		 * @public
		 * @note Runs checkconfig.
		 */
		readconf();

		/**
		 * Destructor.
		 * 
		 * @public
		 */
		~readconf();

		/**
		 * Return the NNTP username.
		 * 
		 * @public
		 */
		std::string nntpuser();

		/**
		 * Return the NNTP password.
		 * 
		 * @public
		 */
		std::string nntppass();

		/**
		 * Return the NNTP hostname.
		 * 
		 * @public
		 */
		std::string nntphost();

		/**
		 * Return the NNTP port.
		 * 
		 * @public
		 */
		std::string nntpport();

		/**
		 * Return if the NNTP connection requires SSL.
		 * 
		 * @public
		 */
		bool nntpssl();

	private:
		/**
		 * The NNTP connection options.
		 * 
		 * @private
		 */
		std::string NNTPUSER = "";
		std::string NNTPPASS = "";
		std::string NNTPHOST = "";
		std::string NNTPPORT = "";
		bool        NNTPSSL  = false;

		/**
		 * The MongoDB connection options.
		 * 
		 * @private
		 */
		std::string DBHOST   = "";
		std::string DBPORT   = "";

		/**
		 * Check if there is a config file, load it, if not make one.
		 * 
		 * @private
		 */
		void checkconfig();

		/**
		 * Create a configuration file.
		 * 
		 * @private
		 */
		void initiateconfig();

		/**
		 * Read the configuration file.
		 * 
		 * @private
		 */
		void readconfig();
	};

	/**
	 * Exceptions for class readconf.
	 * 
	 * @class
	 */
	class ConfigException : std::runtime_error
	{
		public: ConfigException(const std::string& error) : runtime_error(error) {
		}
	};
}
