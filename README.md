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

g++ examples.cpp cppnntplib/*.cpp -o "examples" -lboost_system -lpthread -lboost_regex -std=c++11 -lboost_thread -lssl -lcrypto -lz -lboost_iostreams


------------------------------------------------------------------------
Some info for me:


Current commands implemented:

AUTHINFO      (login)

ARTICLE       (only outputs to CLI, use body if you need the message)

BODY          (fetches the message of the article)

CAPABILITIES  (list of supported capabilities)

DATE          (UTC time of the server, used with NEWNEWS and NEWGROUPS)

GROUP         (tells the server to use a specified group)

HEAD          (single header for a message-id or art #)

HELP          (list of supported commands)

LAST          (previous article, use after ARTICLE/STAT/NEXT)

LIST ACTIVE   (list all groups with their article numbers and post status, optional search wildmat)

LIST ACTIVE.TIMES (list all groups with their creators and create time, optional search wildmat)

LIST NEWSGROUPS (displays list of all groups with descriptions, optional search wildmat)

LIST OVERVIEW.FMT (overview format for OVER/XOVER)  

LISTGROUP     (displays all the article numbers in the group or within a range or higher or lower)

NEXT          (next article)

NEWGROUPS     (new groups since yyyymmdd hhmmss GMT)

NEWNEWS       (list of message-id's since yyyymmdd hhmmss GMT for the selected group)      

POST          (upload an article to usenet)

STAT          (asks if the article exists)

XFEATURE GZIP COMPRESS (compression for XOVER/OVER headers)

XOVER         (need to store the headers in an array)

Even though some commands in RFC 2980 or 3977 are not implemented here, many of them are
not available on my NNTP servers, and some of them I wouldn't use anyway.
