#pragma once
#include <iostream>
#include "mongo/client/dbclient.h"
#include "../utils/readconf.hpp"
#include "../nntp/nntp.hpp"

namespace cppnntp
{
	// Handle fetching of headers from usenet and storing them in mongodb.
	class headers
	{
	public:
		/**
		 * Constructor.
		 *
		 * @public
		 * @param mongo DBclientConnection pointer.
		 */
		headers(mongo::DBClientConnection * dbptr);

		/**
		 * Destructor.
		 * 
		 * @public
		 */
		~headers();

		/**
		 * Fetch the active groups or the specified groups for new headers.
		 * 
		 * @public
		 * 
		 * @param group = (optional) Group name.
		 */
		void forward(const std::string &group = "");

	private:

		/**
		 * Fetch new article headers.
		 * 
		 * @private
		 * 
		 * @param   group = The group.
		 * @param lastart = The newest article we have for the group.
		 */
		void loopforward(const std::string &group,
						const std::string &lastart);

		/**
		 * Initiate the settings object.
		 * 
		 * @private
		 */
		cppnntp::readconf config;

		/**
		 * Take a connection pointer from the constructor, turn it
		 * into an object (ex: db->query(...)).
		 * 
		 * @private
		 */
		mongo::DBClientConnection *db;
	};
}
