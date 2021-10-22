#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "PasswordManager.h"
#include "Tree.h"

using namespace std;

/*struct Node {
	string value;
	Node* parent;
	vector<Node*> children;
	int depth;
	int length;

	Node(string value) {
		this->value = value;
	}
};*/

class PasswordManager;

class PasswordTree : public Tree
{
public:
	//PasswordTree(Node* root, string line, int subcount);
	PasswordTree(PasswordManager* manager, Node* root);
	void generateTree(Node* root, string line, int subcount);
	vector<string> getValidStringSet();
private:
	Node* finalString;
	map<string, vector<int>>* collatzMap;
	const int MAX_DIGITS = 4;
};

