#include "cppnntp/nntp/nntp.hpp"

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
		 */
		headers();

		/**
		 * Destructor.
		 * 
		 * @public
		 */
		~headers();

		/**
		 * Download new headers for active groups.
		 * 
		 * @public
		 */
		forward();
	};
}
