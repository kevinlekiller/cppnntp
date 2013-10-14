#include "headers.hpp"

namespace cppnntp {

	/**
	 * Constructor.
	 *
	 * @public
	 * @param mongo DBclientConnection pointer.
	 */
	headers::headers(mongo::DBClientConnection * dbptr) {
		db = dbptr;
	}

	/**
	 * Destructor.
	 * 
	 * @public
	 */
	 headers::~headers() {
	}

	/**
	 * Download new headers for active groups or the specified one.
	 * 
	 * @public
	 */
	bool headers::forward(const std::string &group) {
		// Check if it's a specified group.
		if (group == "") {
			return true;
		}
		// Fetch the names of the groups and loop through them.
		else {
			return true;
		}
	}
}
