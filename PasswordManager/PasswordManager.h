#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

class PasswordManager
{
public:
	PasswordManager();
	~PasswordManager();
	void start();
	void createUser(string username, string password);
	vector<string> checkUser(string username);
	vector<string> split(string line, char delim= ' ');
	bool generateFile();
	void analyseFile();
	string encryptPassword(unsigned char* pwd);
	inline int collatzEncrypt(int n);
	map<int, vector<int>> createCollatzMap();


private:
	map<int, vector<int>> collatzMap;
};

