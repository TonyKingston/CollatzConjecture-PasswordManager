#pragma once
#include <string>
#include <vector>
#include <any>

using namespace std;
class Node
{
public:
	string value;
	Node* parent;
	vector<Node*> children;
	int depth;
	int length;

	Node(string value) {
		this->value = value;
	}
};

