#include "yencdecode.hpp"
namespace cppnntplib {
	/**
	 * Constructor.
	 *
	 * @public
	 */
	yencdecode::yencdecode() {}

	/**
	 * Destructor.
	 *
	 * @public
	 */
	yencdecode::~yencdecode() {}

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
	bool yencdecode::decodeyencstring(const std::string &indata, std::string &outdata) {
		// Create a string to store the yEnc data.
		std::string encodedYenc = "";
		// Use regex to fetch the yEnc data from the input string.
		try {
			boost::smatch match;
			boost::regex pattern
			("=ypart.+? end=[[:digit:]]*[\r\n ]*(.*)[\r\n ]*=yend",
			 boost::regex_constants::icase);

			if (boost::regex_search (indata, match, pattern)) {
				// If we found the data, store it in the string.
				if (match[1] != "")
					encodedYenc = match[1];
				// If nothing was found, return false.
				else
					return false;
			}
		} catch (boost::regex_error& e) {
			boostRegexExceptions(e);
			return false;
		}

		if (encodedYenc == "")
			return false;

		// Decode the yEnc data, store it in the string.
		for (unsigned long i = 0; i < encodedYenc.length(); i++) {
			if (encodedYenc[i] == '\r' || encodedYenc[i] == '\n')
				continue;
			if (encodedYenc[i] == '=') {
				i++;
				outdata += ((encodedYenc[i] + 150) % 256);
			}
			else
				outdata += ((encodedYenc[i] + 214) % 256);
		}
		return true;
	}
}
