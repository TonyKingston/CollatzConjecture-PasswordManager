#pragma once
#include <string>
#include <map>

using namespace std;

class PasswordManager
{
public:
	PasswordManager();
	~PasswordManager();
	void start();
	void createUser(string username, string password);
	bool checkUser(string username, string password);
	bool generateFile();
	void analyseFile();
	string encryptPassword(unsigned char* pwd);
	int collatzEncrypt(int n);
	map<int, int> createCollatzMap();


private:

};
