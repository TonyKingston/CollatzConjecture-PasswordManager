#pragma once
#include <string>
#include <iosfwd>

using namespace std;

class User
{
public:
	User(string username, string password);
	~User();
	friend ostream& operator<<(ostream&, const User& usr);

private:
	string username;
	string password;
};



