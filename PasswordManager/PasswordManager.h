#pragma once
#include <string>
#include <map>
#include <vector>
#include <unordered_map>

using namespace std;

struct Node {
	string value;
	Node* parent;
	vector<Node*> children;
	int depth;
	int length;

	Node(string value) {
		this->value = value;
	}

 //   Node(const Node&) = delete;
	//Node& operator=(const Node&) = delete;
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
	unordered_map<string, vector<int>> createCollatzMap();


private:
	void passwordTree(Node* node, string line);
	void passwordTree(Node* node, string line, int subCount);
	void deleteTree(Node* root);
	void getValidStringSet(Node* root, int subCount);
	//unordered_map<int, vector<int>> collatzMap;
	unordered_map<string, vector<int>> collatzMap;
};

