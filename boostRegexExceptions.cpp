#include <boost/regex.hpp>
void boostRegexExceptions(boost::regex_error e)
{
	std::cerr << "Regex Error: ";
	switch (e.code())
	{
		case boost::regex_constants::error_collate:
			std::cerr << "The expression contains an invalid collating element name.\n";
			break;
		case boost::regex_constants::error_ctype:
			std::cerr << "The expression contains an invalid character class name.\n";
			break;
		case boost::regex_constants::error_escape:
			std::cerr << "The expression contains an invalid escaped character or a trailing escape.\n";
			break;
		case boost::regex_constants::error_backref:
			std::cerr << "The expression contains an invalid back reference.\n";
			break;
		case boost::regex_constants::error_brack:
			std::cerr << "The expression contains mismatched square brackets ('[' and ']').\n";
			break;
		case boost::regex_constants::error_paren:
			std::cerr << "The expression contains mismatched parentheses ('(' and ')').\n";
			break;
		case boost::regex_constants::error_brace:
			std::cerr << "The expression contains mismatched curly braces ('{' and '}').\n";
			break;
		case boost::regex_constants::error_badbrace:
			std::cerr << "The expression contains an invalid range in a {} expression.\n";
			break;
		case boost::regex_constants::error_range:
			std::cerr << "The expression contains an invalid character range (e.g. [b-a]).\n";
			break;
		case boost::regex_constants::error_space:
			std::cerr << "There was not enough memory to convert the expression into a finite state machine.\n";
			break;
		case boost::regex_constants::error_badrepeat:
			std::cerr << "One of *?+{ was not preceded by a valid regular expression.\n";
			break;
		case boost::regex_constants::error_complexity:
			std::cerr << "The complexity of an attempted match exceeded a predefined level.\n";
			break;
		case boost::regex_constants::error_stack:
			std::cerr << "There was not enough memory to perform a match.\n";
			break;
		default:
			std::cerr << "Unkown.\n";
	}
}
