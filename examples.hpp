#pragma once
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include "mongo/client/dbclient.h"
#include "cppnntp/nntp/nntp.hpp"
#include "cppnntp/processing/headers.hpp"
#include "cppnntp/utils/readconf.hpp"

void start();
void help();
void examples();
int main();
