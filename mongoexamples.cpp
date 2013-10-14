#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h"

void run();

int main() {
	run();
}

void run() {
	const std::string host = "localhost";
	const std::string port = "27017";

	mongo::DBClientConnection db;

	std::cout
		<< "Trying to connect to MongoDB on: "
		<< host << ':'
		<< port << std::endl;

	// Try connecting to mongo.
	try {
		c.connect(host + ':' + port);
		std::cout << "Succesfully connected!" << std::endl;
	} catch (const mongo::DBException &e) {
		std::cerr << "Mongo error: " << e.what() << std::endl;
		return;
	}

	// Check if we have documents in the collection persons.
	if (c.count("tutorial.persons") < 10) {

		std::cout << "Try to insert an article.\n";

		// Mongo uses BSON format for inserting, so build a BSON object.
		mongo::BSONObj p = BSON(mongo::GENOID << "name" << "Joe" << "age" << 33);

		// Try to insert it.
		try {
			c.insert("tutorial.persons", p);
			std::cout << "Succefully inserted!" << std::endl;
		} catch (const mongo::DBException &e) {
			std::cerr << "Mongo error: " << e.what() << std::endl;
			return;
		}
	}
	else
		std::cout << "Not going to insert because there are already rows in tutorial.persons.\n";

	// Print the amount of documents in persons collection.
	std::cout << "Number of rows in tutorials.persons: " << c.count("tutorial.persons") << std::endl;

	// Select all the rows in persons collection.
	std::cout << "Now going to select the rows.\n\n";
	mongo::auto_ptr<mongo::DBClientCursor> cursor = c.query("tutorial.persons", mongo::BSONObj());

	// Loop over them and print them.
	while (cursor->more())
		std::cout << cursor->next().toString() << std::endl;

	// Query persons collection for documents where age == 33.
	std::cout << "Fetch the name(s) of people who are 33 years old.\n";
	mongo::auto_ptr<mongo::DBClientCursor> cursor2 = c.query("tutorial.persons", QUERY("age" << 33));

	// Loop over them but just print the name.
	while (cursor2->more()) {
		mongo::BSONObj p = cursor2->next();
		std::cout << p.getStringField("name") << std::endl;
	}
}
