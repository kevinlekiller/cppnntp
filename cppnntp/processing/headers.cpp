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
	 *
	 * @param (optional) Group name.
	 */
	void headers::forward(const std::string &group) {
		/*mongo::auto_ptr<mongo::DBClientCursor> cursor;
		// Do all groups.
		if (group == "")
			cursor = db->query("cppnntp.groups", QUERY("factive" << 1));
		// Do the specified group.
		else
			cursor = db->query("cppnntp.groups", QUERY("name" << group));

		// Loop over the groups.
		while (cursor->more()) {
			
		}*/
	}

	/**
	 * Fetch new article headers.
	 * 
	 * @private
	 * 
	 * @param   group = The group.
	 * @param lastart = The newest article we have for the group.
	 */
	void loopforward(const std::string &group,
					const std::string &lastart) {
		// Login to NNTP, get the current group information.
		cppnntp::nntp nntp(true);

		if (!nntp.group(group))
			return;

		std::string wantedarticle;
		// Check if the group is new.
		if (lastart == "0") {
			std::cout << "Initiating new group " << group << ".\n";
			wantedarticle = std::to_string(nntp.group_newest() - 20000);
		}
		// Check if our local article is older than the servers.
		else if (std::stoi(lastart) < nntp.group_newest())
			wantedarticle = std::to_string(nntp.group_newest());
		else
			return;
	}
}
