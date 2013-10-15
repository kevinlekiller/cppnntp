#pragma once
#include <iostream>
/**
 *
 * Parse a boost regex error code, display a textual response to the user using cerr.
 *
 * @param boost::regex_error
 */
void boostRegexExceptions(boost::regex_error e);
