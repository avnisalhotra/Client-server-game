#pragma once
#include <iostream>
#include<stdio.h> 
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <winsqlite/winsqlite3.h>
#include <string>
using namespace std;

class Database {
private:
	string ClientId;
	int Score;

public:
	void setScore(int);
	void setClientId(string);
	string getClientId();
	int getScore();

};

