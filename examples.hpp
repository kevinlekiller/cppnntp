#pragma once
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <boost/regex.hpp>
#include <stdexcept>
#include "cppnntp/nntp.hpp"

std::string NNTP_USER = "";
std::string NNTP_PASS = "";
std::string NNTP_PORT = "";
std::string NNTP_HOST = "";
bool        NNTP_SSL = false;

void start();
void help();
bool readconf();
int main();
