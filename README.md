cppnntplib
==========

Work in progress Usenet / NNTP c++ library

This is a project I'm currently working on, it's no where near ready
yet, I won't offer any support so use at your own risk. Eventually
it will be used in some CLI scripts I will make.

I started messing around with libnntp (on googlecode), but I decided to
make my own, I used some stuff from libnntp so thanks the creators of
libnntp and read the HTTP asio tutorials by captainOz on github
(some pretty good stuff if you want to learn more about asio).

If you want to test you can edit the config.cfg file (you'll have to
change the article numbers), then compile. You can comment out parts
of test.cpp or edit parts of it to do different things, it has comments
and examples on what most of the things do.

(Compiled fine for me on g++ 4.8.1 on ubuntu 13.10, boost 1.54).
g++ test.cpp nntp.cpp boostRegexExceptions.cpp socket.cpp yencdecode.cpp -o "test" -lboost_system -lpthread -lboost_regex -std=c++11 -lboost_thread -lssl -lcrypto
