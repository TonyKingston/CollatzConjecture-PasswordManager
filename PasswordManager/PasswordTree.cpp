#include "PasswordTree.h"

PasswordTree::PasswordTree(PasswordManager* manager, Node* root) : Tree(root) {
	collatzMap = &manager->collatzMap;
	//this->root = root;
	finalString = NULL;
}

void PasswordTree::generateTree(Node* currentNode, string line, int subCount) {
	bool valid = true;
	while (finalString == NULL && valid) {
		string sub;

		if (line == "" || finalString != NULL) return;
		int maxSubs = (line.length() / 3);
		maxSubs += (line.length() % 3 == 0) ? 0 : 1;
		/*if (currentNode->depth >= subCount / 2 && line.length() < (root->value.length() + 1) / 2) {
			int d = root->value.length() / 2;
			return;
		}*/
		if (currentNode->depth + maxSubs > subCount) {
			return;
		}
		/*if (currentNode != root && currentNode->depth > 5 && currentNode->length / currentNode->depth > 2.5) {
			return;
		}*/
		//|| (subCount - currentNode->depth)
		// We know from the collatz map that y will not be more than 3 digits
		for (int y = 1; y < MAX_DIGITS && y <= line.length(); y++) {
			y = (currentNode->depth == subCount - 1) ? line.length() : y;
			sub = line.substr(0, y);
			if (sub[0] == '0') {
				return;
			}
			auto it = collatzMap->find(sub);
			// sub could be a large integer, stoi using stoi will cause memory range error so it checks the number of digits
			if (sub.length() < MAX_DIGITS && it != collatzMap->end()) {
			//	int count = 0;
				Node* newNode = new Node(sub);
				newNode->parent = currentNode;
				newNode->depth = currentNode->depth + 1;
				newNode->length = currentNode->length + sub.length();
				currentNode->children.push_back(newNode);
				if (newNode->depth == subCount) {
					finalString = newNode;
					return;
					/*Node* p = newNode;
					while (p->parent != NULL) {
						p = p->parent;
					}
					currentNode = p;*/
				}
			}

		}
		valid = !valid;
		// Be greedy
		if (currentNode->children.size() >= 1) {
			int z = currentNode->children.size() - 1;
			while (z >= 0) {
				generateTree(currentNode->children[z], line.substr(currentNode->children[z]->value.length(), line.length()), subCount);
				z--;
			}
		}
		else {
			for (auto& node : currentNode->children) {
				generateTree(node, line.substr(node->value.length(), line.length()), subCount);
			}
		}
		/*for (auto& node : currentNode->children) {
			generateTree(node, line.substr(node->value.length(), line.length()), subCount);
		}*/
	}
}

vector<string> PasswordTree::getValidStringSet() {
	vector<string> substrings;
	if (finalString != NULL) {
		Node* node = finalString;
		while (node->parent != NULL) {
			substrings.push_back(node->value);
			node = node->parent;
		}
	}
	return substrings;
}
/*void PasswordTree::generateTree(Node* currentNode, string line, int subCount) {
	bool valid = true;
	do {
		string sub;
		
		if (line == "" || finalString != NULL) return;
		/*if (currentNode != root && currentNode->depth > 5 && currentNode->length / currentNode->depth > 2.5) {
			return;
		}
		//|| (subCount - currentNode->depth)
		// We know from the collatz map that y will not be more than 3 digits
		for (int y = 1; y < 4 && y <= line.length(); y++) {
			y = (currentNode->depth == subCount - 1) ? line.length() : y;
			sub = line.substr(0, y);
			if (sub[0] == '0') {
				return;
			}
			// sub could be a large integer, stoi using stoi will cause memory range error so it checks the number of digits
			if (sub.length() < 4 && collatzMap->find(sub) != collatzMap->end()) {
				Node* newNode = new Node(sub);
				newNode->parent = currentNode;
				newNode->depth = currentNode->depth + 1;
				newNode->length = currentNode->length + sub.length();
				currentNode->children.push_back(newNode);
				if (newNode->depth == subCount) {
					finalString = newNode;
					Node* p = newNode;
					while (p->parent != NULL) {
						p = p->parent;
					}
					currentNode = p;
				}
			}
			
		}
		valid = !valid;
		for (auto& node : currentNode->children) {
			generateTree(node, line.substr(node->value.length(), line.length()), subCount);
		}
	} while (currentNode->children.size() == 0 && valid);
}*/

