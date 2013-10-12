cppnntplib
==========

Work in progress Usenet / NNTP c++ library

This is a project I'm currently working on, it's no where near ready
yet, I won't offer any support so use at your own risk. Eventually
it will be used in some CLI scripts I will make (downloading headers
and storing them in a database).

I started messing around with libnntp (on googlecode), but I decided to
make my own, I used some stuff from libnntp so thanks the creators of
libnntp and I read the HTTP asio tutorials by captainOz on github
which helped me tremendously (some pretty good tutorials if you want to
learn more about asio sockets).

If you want to test you can edit the config.cfg file (you'll have to
change the article numbers), then compile. You can comment out parts
of test.cpp or edit parts of it to do different things, it has comments
and examples on what most of the things do.

Zlib is required for XFEATURE GZIP COMPRESS.

Compiled fine for me on g++ 4.8.1 on ubuntu 13.10, boost 1.54, also
compiles fine on boost 1.53.

g++ test.cpp nntp.cpp boostRegexExceptions.cpp socket.cpp yencdecode.cpp -o "test" -lboost_system -lpthread -lboost_regex -std=c++11 -lboost_thread -lssl -lcrypto -lz -lboost_iostreams


------------------------------------------------------------------------
Some info for me:


Current commands implemented:

AUTHINFO      (login)

ARTICLE       (only outputs to CLI, use body if you need the message)

BODY          

DATE          (need to parse this with BOOST)

GROUP         

HEAD          

HELP          

LISTGROUP     (need to do more testing on this)

POST          

STAT          (not tested yet)

XFEATURE GZIP COMPRESS

XOVER         (need to store the headers in an array)


Not implemented:

CAPABILITIES  

IHAVE         

LAST          

LIST          

MODE READER   (not needed?)

NEWGROUPS     

NEWNEWS       

NEXT          

OVER          (RFC3977 2006, none of my NNTP providers have this, it's
the same as RFC2980 2000's XOVER which all my providers have)

XHDR          (RFC3977 = HDR, none of my servers have HDR, this command
seems to only display the article number + the subect)
