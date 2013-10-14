//#include "../nntp/nntp.hpp"
#include <iostream>
#include "mongo/client/dbclient.h"

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
		 * Download new headers for active groups or the specified one.
		 * 
		 * @public
		 */
		bool forward(const std::string &group = "");

	private:
		/**
		 * Take a connection pointer from the constructor, turn it
		 * into an object (ex: db->query(...)).
		 * 
		 * @private
		 */
		mongo::DBClientConnection *db;
	};
}
