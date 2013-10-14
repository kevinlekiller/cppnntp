namespace cppnntp
{
	/**
	 * Response codes from usenet.
	 *
	 * First digit :
	 *     1xx - Informative message.
	 *     2xx - Command completed OK.
	 *     3xx - Command OK so far; send the rest of it.
	 *     4xx - Command was syntactically correct but failed for some reason.
	 *     5xx - Command unknown, unsupported, unavailable, or syntax error.
	 *
	 * Second digit:
	 *     x0x - Connection, setup, and miscellaneous messages.
	 *     x1x - Newsgroup selection.
	 *     x2x - Article selection.
	 *     x3x - Distribution functions.
	 *     x4x - Posting.
	 *     x8x - Reserved for authentication and privacy extensions.
	 *     x9x - Reserved for private use (non-standard extensions).
	 */
	enum responsecodes
	{
		/**
		*         RFC: 977
		*        Code: 100
		*     Command: HELP
		*    Response: Help text follows
		* Description: The server returns a short summary of commands
		*              that are understood by this implementation of the server.
		*/
		RESPONSECODE_HELP_FOLLOWS = 100,

		/**
		*         RFC: 3977
		*        Code: 101
		*     Command: CAPABILITIES
		*    Response: Capability list:
		* Description: The server returns a list of its
		* implemented capabilities.
		*/
		RESPONSECODE_CAPABILITIES_FOLLOW = 101,

		/**
		*         RFC: 2980
		*        Code: 111
		*     Command: DATE
		*    Response: (string of numbers representing the date and time)
		* Description: The server returned the date/time.
		*/
		RESPONSECODE_SERVER_DATE = 111,

		/**
		*         RFC: 977
		*        Code: 199
		*     Command: N\A
		*    Response: (Debugging Output)
		* Description: Debugging Information. Possibly only available
		*              to administrators?
		*/
		RESPONSECODE_DEBUGGING_OUTPUT = 199,

		/**
		*         RFC: 977
		*        Code: 200
		*     Command: AUTHINFO / MODE READER
		*    Response: Service available, posting allowed /
		*              Reader mode, posting permitted
		* Description: The server is ready to accept commands and
		*              the client is allowed to post articles.
		*/
		RESPONSECODE_READY_POSTING_ALLOWED = 200,

		/**
		*         RFC: 977
		*        Code: 201
		*     Command: AUTHINFO / MODE READER
		*    Response: Service available, posting prohibited
		* Description: The server is ready to accept commands and
		*              the client is not allowed to post articles.
		*/
		RESPONSECODE_READY_POSTING_PROHIBITED = 201,

		/**
		*         RFC: 977
		*        Code: 202
		*     Command: SLAVE
		*    Response: Slave status noted
		* Description: The server indicates that this client connection
		*              is to a slave server, rather than a user.
		*/
		RESPONSECODE_SLAVE_RECOGNIZED = 202,

		/**
		*         RFC: 2980
		*        Code: 203
		*     Command: MODE STREAM
		*    Response: Streaming is OK
		* Description: The server indicates that it is ready to accept
		*              TAKETHIS and CHECK commands.
		*/
		RESPONSECODE_STREAM_OK = 203,

		/**
		*         RFC: 977
		*        Code: 205
		*     Command: QUIT
		*    Response: NNTP Service exits normally
		* Description: The server acknowledges the QUIT command and then
		*              closes the connection to the client.
		*/
		RESPONSECODE_DISCONNECTING_REQUESTED = 205,

		/**
		*         RFC: 977
		*        Code: 211
		*     Command: GROUP / LISTGROUP
		*    Response: Group selected
		* Description: The server returned the first and last articles
		*              numbers in the group, and an estimate of
		*              the number of articles for the group.
		*/
		RESPONSECODE_GROUP_SELECTED = 211,

		/**
		*         RFC: 977
		*        Code: 215
		*     Command: LIST
		*    Response: Information follows
		* Description: The server returned a list of valid newsgroups
		*              and associated information.
		*/
		RESPONSECODE_LIST_RESPONSE = 215,

		/**
		*         RFC: 2980
		*        Code: 218
		*     Command: XINDEX
		*    Response: Tin-style index follows
		* Description: Only ued for the Tin news reader.
		*/
		RESPONSECODE_TIN_INDEX_FOLLOWS = 218,

		/**
		*         RFC: 977
		*        Code: 220
		*     Command: ARTICLE
		*    Response: Article follows
		* Description: The server returned the full article,
		*              header + message.
		*/
		RESPONSECODE_ARTICLE_FOLLOWS = 220,

		/**
		*         RFC: 2980
		*        Code: 221
		*     Command: HEAD / XHDR / XPAT
		*    Response: Article headers follow
		* Description: The server returned the header, no message.
		*/
		RESPONSECODE_HEAD_FOLLOWS = 221,

		/**
		*         RFC: 977
		*        Code: 222
		*     Command: BODY
		*    Response: Article body follows
		* Description: The server returned the message, no header.
		*/
		RESPONSECODE_BODY_FOLLOWS = 222,

		/**
		*         RFC: 977
		*        Code: 223
		*     Command: STAT / LAST / NEXT / XPATH
		*    Response: Article exists and selected
		* Description: The server returned the message-id which the
		*              client can use to see if the article exists
		*              without downloading the whole article.
		*/
		RESPONSECODE_ARTICLE_SELECTED = 223,

		/**
		*         RFC: 2980
		*        Code: 224
		*     Command: XOVER / OVER / XROVER
		*    Response: Overview information follows
		* Description: The server returned the header, along with non
		*              standard x information.
		*/
		RESPONSECODE_OVERVIEW_FOLLOWS = 224,

		/**
		*         RFC: 3977
		*        Code: 225
		*     Command: HDR
		*    Response: Headers follow
		* Description: The server returned the header, along with other
		*              non standard infomation.
		*/
		RESPONSECODE_HEADERS_FOLLOW = 225,

		/**
		*         RFC: 3977
		*        Code: 230
		*     Command: NEWNEWS
		*    Response: List of new articles follows
		* Description: The server returned a list of message-ids from
		*              articles posted or received to the specified
		*              newsgroup since the specified date.
		*/
		RESPONSECODE_NEW_ARTICLES_FOLLOW = 230,

		/**
		*         RFC: 997
		*        Code: 231
		*     Command: NEWGROUPS
		*    Response: List of new newsgroups follows
		* Description: The server returned a list of newsgroups created
		*              since a specified date, it is in the same format
		*              as the LIST command.
		*/
		RESPONSECODE_NEW_GROUPS_FOLLOW = 231,

		/**
		*         RFC: 997
		*        Code: 235
		*     Command: IHAVE
		*    Response: Article transferred OK
		* Description: The server received the article succesfully
		*              after the client sent it using IHAVE.
		*/
		RESPONSECODE_TRANSFER_OKAY = 235,

		/**
		*         RFC: 2980
		*        Code: 238
		*     Command: CHECK
		*    Response: No such article found, please send it to me
		* Description: The server requests the client to send the article.
		*/
		RESPONSECODE_CHECK_REQUESTS_ARTICLE = 238,

		/**
		*         RFC: 2980
		*        Code: 239
		*     Command: TAKETHIS
		*    Response: Article transferred ok
		* Description: The server received the article succesfully
		*              after the client sent it using TAKETHIS.
		*/
		RESPONSECODE_TRANSFER_OK = 239,

		/**
		*         RFC: 997
		*        Code: 240
		*     Command: POST
		*    Response: Article posted OK
		* Description: The server received the article succesfully
		*              after the client sent it using POST.
		*/
		RESPONSECODE_POSTING_SUCCESS = 240,

		/**
		*         RFC: 2980
		*        Code: 250
		*     Command: AUTHINFO
		*    Response: Authorization accepted
		* Description: Successful authentication to the server
		*              using the AUTHINFO command.
		*/
		RESPONSECODE_AUTHORIZATION_ACCEPTED = 250,

		/**
		*         RFC: 2980
		*        Code: 281
		*     Command: AUTHINFO
		*    Response: Authentication accepted
		* Description: Successful authentication to the server
		*              using the AUTHINFO command.
		*/
		RESPONSECODE_AUTHENTICATION_ACCEPTED = 281,

		/**
		*         RFC: 2980
		*        Code: 282
		*     Command: XGTITLE
		*    Response: List of groups and descriptions follows
		* Description: The server returned descriptions to the
		*              client for the list of groups.
		*/
		RESPONSECODE_XGTITLE_SUCCESS = 282,

		/**
		*         RFC: 2980
		*        Code: 288
		*     Command: XTHREAD
		*    Response: Binary data to follow
		* Description: Only used for TRN newsreader.
		*/
		RESPONSECODE_XTHREAD_SUCCESS = 288,

		/**
		*         RFC: 3977
		*        Code: 290
		*     Command: (various X commands)
		*    Response: (various)
		* Description: Succesful response to various X commands.
		*/
		RESPONSECODE_X_COMMAND_SUCCESS = 290,

		/**
		*         RFC: 997
		*        Code: 335
		*     Command: IHAVE
		*    Response: Send article to be transferred
		* Description: The server is requesting an article from the
		*              client after a succesful IHAVE command is passed.
		*/
		RESPONSECODE_TRANSFER_SEND = 335,

		/**
		*         RFC: 997
		*        Code: 340
		*     Command: POST
		*    Response: Send article to be posted
		* Description: The server is requesting an article from the
		*              client after a succesful POST command is passed.
		*/
		RESPONSECODE_POSTING_SEND = 340,

		/**
		*         RFC: 2980
		*        Code: 350
		*     Command: AUTHINFO SIMPLE
		*    Response: Continue with authorization sequence
		* Description: The server is requesting the user to enter
		*              his username and password.
		*/
		RESPONSECODE_CONTINUE_AUTH_SEQUENCE = 350,

		/**
		*         RFC: 2980
		*        Code: 381
		*     Command: AUTHINFO
		*    Response: More authentication information required
		* Description: The server is requesting the user enter his
		*              password with AUTHINFO PASS after a succesfull
		*              AUTHINFO USER command.
		*/
		RESPONSECODE_AUTHENTICATION_CONTINUE = 381,

		/**
		*         RFC: 997
		*        Code: 400
		*     Command: N/A
		*    Response: Service not available or no longer available
		* Description: The server disconnected the client due to an
		*              unspecified error.
		*/
		RESPONSECODE_DISCONNECTING_FORCED = 400,

		/**
		*         RFC: 3977
		*        Code: 401
		*     Command: N/A
		*    Response: The server is in the wrong mode; the indicated
		*              capability should be used to change the mode
		* Description: The server requests the client to change mode.
		*/
		RESPONSECODE_WRONG_MODE = 401,

		/**
		*         RFC: Draft
		*        Code: 402
		*     Command: LIST EXTENSIONS
		*    Response: Server has no extensions
		* Description: The server is telling the client
		*              it does not have any extensions.
		*/
		RESPONSECODE_NO_EXTENSIONS = 402,

		/**
		*         RFC: 3977
		*        Code: 403
		*     Command: N/A
		*    Response: Internal fault or problem preventing action being taken
		* Description: The server understood the command but was not
		*              able to process it due to an unspecified error.
		*/
		RESPONSECODE_INTERNAL_FAULT = 403,

		/**
		*         RFC: 997
		*        Code: 411
		*     Command: GROUP / LISTGROUP
		*    Response: No such newsgroup
		* Description: The client requested a group that does not
		*              exist on the server.
		*/
		RESPONSECODE_NO_SUCH_GROUP = 411,

		/**
		*         RFC: 997
		*        Code: 412
		*     Command: ARTICLE / BODY / GROUP / HDR / HEAD / LAST /
		*              LISTGROUP / NEXT / OVER / STAT / XHDR / XOVER
		*    Response: No newsgroup has been selected
		* Description: The server is telling the client that it has not
		*              selected a group before sending of the valid commands.
		*/
		RESPONSECODE_NO_GROUP_SELECTED = 412,

		/**
		*         RFC: 2980
		*        Code: 418
		*     Command: XINDEX
		*    Response: No tin-style index is available for this news group
		* Description: Only used for the TIN news reader.
		*/
		RESPONSECODE_NO_TIN_INDEX = 418,

		/**
		*         RFC: 977
		*        Code: 420
		*     Command: ARTICLE / BODY / HDR / HEAD / LAST / NEXT /
		*              OVER / STAT / XHDR / XOVER
		*    Response: Current article number is invalid
		* Description: The client requested and invalid article number or the
		*              client has not sent an article number in the request.
		*/
		RESPONSECODE_NO_ARTICLE_SELECTED = 420,

		/**
		*         RFC: 977
		*        Code: 421
		*     Command: NEXT
		*    Response: No next article in this group
		* Description: The server is telling the client there is no
		*              newer article than the last requested article.
		*/
		RESPONSECODE_NO_NEXT_ARTICLE = 421,

		/**
		*         RFC: 977
		*        Code: 422
		*     Command: LAST
		*    Response: No previous article in this group
		* Description: The server is telling the client there is no
		*              older article than the last requested article.
		*/
		RESPONSECODE_NO_PREVIOUS_ARTICLE = 422,

		/**
		*         RFC: 977
		*        Code: 423
		*     Command: ARTICLE / BODY / HDR / HEAD / OVER / STAT
		*    Response: No article with that number or in that range
		* Description: The server is telling the client the requested
		*              article with that number does not exist.
		*/
		RESPONSECODE_NO_SUCH_ARTICLE_NUMBER = 423,

		/**
		*         RFC: 977
		*        Code: 430
		*     Command: ARTICLE / BODY / HDR / HEAD / OVER / STAT / XHDR / XPAT / XPATH
		*    Response: No article with that message-id
		* Description: The server is telling the client the requested
		*              article with that message-id does not exist.
		*/
		RESPONSECODE_NO_SUCH_ARTICLE_ID = 430,

		/**
		*         RFC: 2980
		*        Code: 431
		*     Command: CHECK
		*    Response: Try sending it again later
		* Description: The server can not currently accept the article.
		*/
		RESPONSECODE_CHECK_SEND_LATER = 431,

		/**
		*         RFC: 977
		*        Code: 435
		*     Command: IHAVE
		*    Response: Article not wanted
		* Description: The server already has the article and is telling
		*              the client not to send it.
		*/
		RESPONSECODE_IHAVE_UNWANTED = 435,

		/**
		*         RFC: 977
		*        Code: 436
		*     Command: IHAVE
		*    Response: Transfer failed - try again later
		* Description: The server failed receiving the clients article.
		*/
		RESPONSECODE_IHAVE_FAILED = 436,

		/**
		*         RFC: 977
		*        Code: 437
		*     Command: IHAVE
		*    Response: Article rejected - do not try again
		* Description: The server refused the clients article.
		*/
		RESPONSECODE_IHAVE_REJECTED = 437,

		/**
		*         RFC: 2980
		*        Code: 438
		*     Command: CHECK
		*    Response: Already have it, please don't send it to me
		* Description: The server already has the article and is
		*              telling the client not to send it.
		*/
		RESPONSECODE_CHECK_UNWANTED = 438,

		/**
		*         RFC: 2980
		*        Code: 439
		*     Command: TAKETHIS
		*    Response: Article transfer failed
		* Description: The server failed receiving the article
		*/
		RESPONSECODE_TAKETHIS_FAILED = 439,

		/**
		*         RFC: 997
		*        Code: 440
		*     Command: POST
		*    Response: Posting not permitted
		* Description: The server refuses the client to post an article,
		*              the client might not have posting rights.
		*/
		RESPONSECODE_POSTING_PROHIBITED = 440,

		/**
		*         RFC: 997
		*        Code: 441
		*     Command: POST
		*    Response: Posting failed
		* Description: The server failed to receive the clients article.
		*/
		RESPONSECODE_POSTING_FAILED = 441,

		/**
		*         RFC: 2980
		*        Code: 450
		*     Command: AUTHINFO SIMPLE
		*    Response: Authorization required for this command
		* Description: The server is asking the client to authenticate.
		*/
		RESPONSECODE_AUTHORIZATION_REQUIRED = 450,

		/**
		*         RFC: 2980
		*        Code: 452
		*     Command: AUTHINFO SIMPLE
		*    Response: Authorization rejected
		* Description: The server is telling the client it failed to login.
		*/
		RESPONSECODE_AUTHORIZATION_REJECTED = 452,

		/**
		*         RFC: 2980
		*        Code: 480
		*     Command: AUTHINFO GENERIC / CHECK
		*    Response: Command unavailable until the client has
		*              authenticated itself / Transfer permission denied
		* Description: The server is asking the client to login.
		*/
		RESPONSECODE_AUTHENTICATION_REQUIRED = 480,

		/**
		*         RFC: 2980
		*        Code: 481
		*     Command: XGTITLE
		*    Response: Groups and descriptions unavailable
		* Description: The server could not return the list of groups
		*              and descriptions.
		*/
		RESPONSECODE_XGTITLE_FAILED = 481,

		/**
		*         RFC: 2980
		*        Code: 482
		*     Command: AUTHINFO
		*    Response: Authentication rejected
		* Description: The server is telling the client it failed to login.
		*/
		RESPONSECODE_AUTHENTICATION_REJECTED = 482,

		/**
		*         RFC: 3977
		*        Code: 483
		*     Command: XSECRET / XENCRYPT /
		*              (accessing secure groups; ie.: GROUP secure.example)
		*    Response: Command unavailable until suitable privacy
		*              has been arranged
		* Description: The client must negotiate appropriate privacy
		*              protection on the connection (NNTP-TLS).
		*/
		RESPONSECODE_ENCRYPTION_REQUIRED = 483,

		/**
		*         RFC: 977
		*        Code: 500
		*     Command: N/A
		*    Response: Unknown command
		* Description: The server has received an unknown command, or
		*              there was an unspecified error on the server side.
		*/
		RESPONSECODE_UNKNOWN_COMMAND = 500,

		/**
		*         RFC: 977
		*        Code: 501
		*     Command: N/A
		*    Response: Syntax error in command
		* Description: The client did not type the command in correctly.
		*/
		RESPONSECODE_SYNTAX_ERROR = 501,

		/**
		*         RFC: 977
		*        Code: 502
		*     Command: N/A
		*    Response: Access restriction or permission denied
		* Description: The client is not permited to log in, or the
		*              client is trying to access a restricted command.
		*/
		RESPONSECODE_NOT_PERMITTED = 502,

		/**
		*         RFC: 977
		*        Code: 503
		*     Command: N/A
		*    Response: Program fault - command not performed
		* Description: The command is not supported by the server or the
		*              server has suffered an unrecoverable error so the
		*              client's command was not executed.
		*/
		RESPONSECODE_SERVER_ERROR = 503,

		/**
		*         RFC: 3977
		*        Code: 504
		*     Command: XENCRYPT RSA
		*    Response: Base64 encoding error
		* Description: The client did not send a valid Base64 encoded
		*              string to the server.
		*/
		RESPONSECODE_BASE64_ENCODING_ERROR = 504
	};
}
