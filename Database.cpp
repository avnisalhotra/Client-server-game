// a simple table that stores client id and score...zeros if no value.
// it should get the client id from main.cpp of the server.
// set up get and set score part

#include "Database.h"


Database myDatabase;
//string clientID;
//int Score;
// get client id and score from the game server

int mainDatabase();

int mainDatabase(int argc, char** argv) // need to figure out how this initiates could maybe make a call to this in server.
{
	sqlite3* DB;
	std::string sql = "CREATE TABLE PERSON("
		"ClientId TEXT PRIMARY KEY	 NOT NULL, "
		"SCORE		 INT NOT NULL) ;";

	int exit = 0;
	exit = sqlite3_open("example.db", &DB); //database name here, change to game score when ready
	char* messaggeError;
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error Create Table" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
		std::cout << "Table created Successfully" << std::endl;
	sqlite3_close(DB);
	return (0);
}

void Database::setClientId(string ID)
{
	ClientId = ID;
}

void Database::setScore(int s) {

	Score = s;
}

string Database::getClientId()
{
	return ClientId;
}

int Database::getScore() {
	return Score;
}


