#pragma once
#include "Node.h"

class Tree
{
public:
	Tree(Node* root);
	virtual void generateTree(Node* root, string line, int length) = 0;
private:
	Node* root;
};

