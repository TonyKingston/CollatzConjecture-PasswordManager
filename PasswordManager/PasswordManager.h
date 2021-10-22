#pragma once
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include "PasswordTree.h"
#include <exception>

using namespace std;


struct FileNotFound : public exception
{
	const char* what() const throw ()
	{
		return "File not found";
	}
};

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
	//unordered_map<int, vector<int>> createCollatzMap();
	map<string, vector<int>> createCollatzMap(int lower, int upper);
	map<string, vector<int>> collatzMap;
	

private:
	void passwordTree(Node* node, string line);
	void passwordTree(Node* node, string line, int subCount);
	void deleteTree(Node* root);
	void getValidStringSet(Node* root, int subCount);
	void menu();
	void decryptSentence();
	//unordered_map<int, vector<int>> collatzMap;
};

