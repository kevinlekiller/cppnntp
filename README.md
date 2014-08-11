cppnntp
==========

Work in progress basic Usenet / NNTP c++ library.

------------------------------------------------------------------------

[![Build Status](https://travis-ci.org/kevinlekiller/cppnntp.svg?branch=master)](https://travis-ci.org/kevinlekiller/cppnntp)

Requirements:

Zlib is for header compression.

Boost 1.49+

openssl for SSL connections

g++ compiler with c++11 standard

Compiling examples:

g++ examples.cpp -o examples cppnntp/*.cpp -lboost_iostreams -lboost_regex -lboost_system -lcrypto -lpthread -lssl -lz -std=c++11

------------------------------------------------------------------------

License:

GPLv2

------------------------------------------------------------------------

Credits:

[LegalizeAdulthood](https://github.com/LegalizeAdulthood) for the Cmake Build scripts.  
[libnntp](https://code.google.com/p/libnntp/) For many of the ideas used here.

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
