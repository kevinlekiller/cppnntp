#include <boost/regex.hpp>
#include "boostRegexExceptions.hpp"
#include <iostream>

namespace cppnntplib
{
	class yencdecode
	{
	public:
		/**
		 * Constructor.
		 *
		 * @public
		 */
		yencdecode();

		/**
		 * Destructor.
		 *
		 * @public
		 */
		~yencdecode();

		/**
		 * Decode a yEnc string, return true on success.
		 *
		 * @note The decoded data will be passed to outdata.
		 * @public
		 *
		 * @param  indata = The yEnc data (anything before or after
		 * (example: the NNTP response) is stripped out by this function,
		 * so do not worry about that).
		 * @param outdata = Where the decoded data will be stored.
		 * @return   bool = True if it was decoded.
		 */
		bool decodeyencstring(const std::string &indata, std::string &outdata);
	};
}
