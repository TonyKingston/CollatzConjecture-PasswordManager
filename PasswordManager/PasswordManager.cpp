#include "PasswordManager.h"
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "User.h"
#include <random>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <map>
#include <unordered_map>
#include <sstream>
#include "PasswordTree.h"
#include <ctime>

using namespace std;

template<typename Iter, typename RandomGenerator>
Iter random_select(Iter start, Iter end, RandomGenerator& g) {
	uniform_int_distribution<> distribution(0, distance(start, end) - 1);
	advance(start, distribution(g));
	return start;
}

template<typename Iter>
Iter random_select(Iter start, Iter end) {
	static random_device rd;
	static mt19937 gen(rd());
	return random_select(start, end, gen);
}

PasswordManager::PasswordManager()
{
}

PasswordManager::~PasswordManager()
{
}

void PasswordManager::start() {
	collatzMap = createCollatzMap();
	string username;
	string password;
	string encryptedPassword;
	unsigned char* arr;
	vector<string> user;
	int option;

	/*for (auto it = collatzMap.begin(); it != collatzMap.end(); ++it) {
		cout << *max_element(begin(it->second), end(it->second)) << endl;
	}*/

	cout << "Please select an option from the following:\n" << endl;
	cout << "1. Create username/password" << endl;
	cout << "2. Check username and password" << endl;
	cout << "3. Generate password strength analysis file" << endl;
	cout << "4. Analyse password strength analysis file" << endl;

	cin >> option;

	switch (option) {
	case 1:
		cout << "Please enter a username" << endl;
		cin.ignore();
		getline(cin, username);
		username.erase(remove(username.begin(), username.end(), ' '), username.end());
		cout << "Please enter a password" << endl;
		getline(cin, password);
		cout << password.length();
		arr = new unsigned char[password.length() + 1];
		strcpy_s((char*)arr, password.length() + 1, password.c_str());
		encryptedPassword = encryptPassword(arr);

		createUser(username, encryptedPassword);
		delete[] arr;
		break;
	case 2:
		cout << "Please enter a username" << endl;
		cin.ignore();
		getline(cin, username);
		username.erase(remove(username.begin(), username.end(), ' '), username.end());
		user = checkUser(username);
		if (user.size() == 2) {
			int attempts = 3;
			while (attempts > 0) {
				cout << "Please enter a password" << endl;
				getline(cin, password);
				arr = new unsigned char[password.length() + 1];
				strcpy_s((char*)arr, password.length() + 1, password.c_str());
				if (encryptPassword(arr) == user[1]) {
					cout << "Success. User confirmed.";
					break;
				}
				delete[] arr;
				attempts--;
				cout << attempts << " attempts left." << endl;
			}
			
		}
		break;
	case 3:
		generateFile();
		break;
	case 4:
		analyseFile();
		break;
	default:
		cout << "Invalid input." << endl;
		break;
	}
}

void PasswordManager::createUser(string username, string password) {
	User newUser = User(username, password);
	ofstream file("password.txt");
	if (file.is_open()) {
		file << newUser;
		file.close();
	}
	else {
		cout << "File could not be opened" << endl;
	}

}

vector<string> PasswordManager::checkUser(string username) {
	string line;
	ifstream file("password.txt");
	vector<string> subs;
	while (getline(file, line)) {
		if (line.find(username, 0) != string::npos) {
			subs = split(line);
		} 
		else {
			cout << "Failure. Incorrect username.";
		}
	}
	return subs;
}

vector<string> PasswordManager::split(string line, char delim) {
	vector<string> result;
	istringstream iss(line);
	string s;
	while (getline(iss, s, ' ')) {
		result.push_back(s);
	}
	return result;
}

bool PasswordManager::generateFile() {
	mt19937 generator{ random_device{}() };
	uniform_int_distribution<int> distribution{ 97,122 };
	vector<int> letterSet(10);
	for (int i = 0; i < 10; i++) {
		letterSet[i] = 97 + (rand() % (122 - 97 + 1));
	}
	//	cout << distribution << endl;
	ofstream file("passwordtest.txt");
	int length = 1;
	int count = 0;
	if (file.is_open()) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				unsigned char* str = new unsigned char[length + 1];
				//shared_ptr<unsigned char> str(new unsigned char[length]);

				str[length] = '\0';
				//string rand_str(length, '\0');
				/*for (char& dis : rand_str)
					dis = distribution(generator);*/

				for (int x = 0; x < length; x++) {
					//str[x] = distribution(generator);
					str[x] = *random_select(letterSet.begin(), letterSet.end());
					cout << char(str[x]);
				}
				cout << endl;
				//cout << rand_str << endl;
				string encryptedPassword = encryptPassword(str);
				file << encryptedPassword << endl;
				count++;
				delete[] str;
			}
			length++;
		}
		length = 1;
		
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				unsigned char* str = new unsigned char[length + 1];

				str[length] = '\0';
				/*string rand_str(length, '\0');
				for (char& dis : rand_str)
					dis = distribution2(generator);*/

				for (int x = 0; x < length; x++) {
					str[x] = distribution(generator);
					//str[x] = *random_select(letterSet.begin(), letterSet.end());
					cout << char(str[x]);
				}
				cout << endl;
				//cout << rand_str << endl;
				string encryptedPassword = encryptPassword(str);
				file << encryptedPassword << endl;
				count++;
				delete[] str;
			}
			length++;
		}
	}
	//generateFullASCIIPasswords();
	file.close();
	cout << count << endl;
	return true;
}

void PasswordManager::analyseFile() {
	string line;
	ifstream file("passwordtest.txt");
	int length = 1;
	int crackCount = 0;
	int c1totalCount = 0;
	int c2totalCount = 0;
	double c1duration = 0;
	double c2duration = 0;
	unsigned char* guess;
	int c = 0;
	for (int i = 0; i < 20; i++) {
		length = (i == 10) ? 1 : length;
		double duration = 0;
		crackCount = 0;
		for (int j = 0; j < 10; j++) {
			guess = new unsigned char[length + 1];
			guess[length] = '\0';
			c++;

			
			//offset = 0;
			getline(file, line);

			if (length == 1) {
				//if (to_string(collatzEncrypt(collatzMap[stoi(line)][0])) == line) 
				if (to_string(collatzEncrypt(collatzMap[line][0])) == line) {
					crackCount++;
				}
			}
			else {
				Node root = Node(line);
				root.parent = NULL;
				root.length = 0;
				root.depth = 0;
				Node* currentNode = &root;
			//	string test = "1233103264667106523628904436153328108114127127109131081147031413641362310834";
				string test = "1233103114391032110313113";
				/*if (line == test) {
					cout << test.length() << endl;
					cout << test.length() * length << endl;
					PasswordTree tree = PasswordTree(this);
					tree.generateTree(currentNode, line, length);
					deleteTree(currentNode);
					currentNode = NULL;
				}*/

			/*	if (length == 100) {
					PasswordTree tree = PasswordTree(this);
					tree.generateTree(currentNode, line, length);
					deleteTree(currentNode);
					currentNode = NULL;
					cout << c << " " << length << endl;
					cout << line << endl;
				}*/
				
				unsigned char* start = guess;
				if (line == "106111212123") {
				//	cout << "hello";
				}
				PasswordTree tree = PasswordTree(this, currentNode);
				clock_t timer;
				timer = clock();
				tree.generateTree(currentNode, line, length);
				vector<string> strings = tree.getValidStringSet();
				reverse(strings.begin(), strings.end());
				int offset = 0;
				for (auto s : strings) {
					for (int y = 0; y < collatzMap[s].size(); y++) {
						if (collatzMap[s][y] - offset > 0) {
						//	cout << collatzMap[s][y];
							*guess = collatzMap[s][y] - offset;
							break;
						}
					}
					offset = stoi(s);
					guess++;
				}
				guess = start;
				crackCount += (encryptPassword(guess) == line) ? 1 : 0;
				duration += (std::clock() - timer) / (double)CLOCKS_PER_SEC;
				deleteTree(currentNode);
				currentNode = NULL;
			//	string t = encryptPassword(guess);
				//cout << crackCount << endl;
				//cout << c << endl;
				
			}
			/*int subCount = 0;
			do {
				string sub;
				subCount++;
				for (int y = currentNode->length; y  < currentNode->length + 4; y++) {
					sub = line.substr(currentNode->length, y);
					//	sub = (substringCount == length - 1) ? line.substr(totalChars, line.length() - totalChars) : line.substr(totalChars, sublength);
					if (collatzMap.find(stoi(sub)) != collatzMap.end()) {
						Node* newNode = new Node(sub);
						newNode->parent = currentNode;
						newNode->depth = subCount;
						newNode->length = currentNode->length + sub.length();
						currentNode->children.push_back(newNode);
					}
				}
				
			} while (currentNode->children.size() > 0);*/
			/*for (int x = 0; x < line.length(); x++) {
				string sub;
				subCount++;
				for (int y = x; y < 4; y++) {
					sub = line.substr(x, y);
				//	sub = (substringCount == length - 1) ? line.substr(totalChars, line.length() - totalChars) : line.substr(totalChars, sublength);
					if (collatzMap.find(stoi(sub)) != collatzMap.end()) {
						Node* newNode = new Node(sub);
						newNode->parent = currentNode;
						newNode->depth = subCount;
						newNode->length = currentNode->length + sub.length();
						currentNode->children.push_back(newNode);
					}
				}
			}*/
		}
		delete[] guess;
		if (i >= 10) {
			cout << "Category 2: " << (crackCount * 100) / 10 << "% of passwords cracked for " << length << " long password" << endl;
			cout << "Category 2: " << "Average duration taken " << duration / 10 << endl;
			c2totalCount += crackCount;
			c2duration += duration;
		}
		else {
			cout << "Category 1: " << (crackCount * 100) / 10 << "% of passwords cracked for " << length << " long password" << endl;
			cout << "Category 1: " << "Average duration taken " << duration / 10 << endl;
			c1totalCount += crackCount;
			c1duration += duration;
		}
		length++;
		
	}

	cout << "\nCategory 1: Total No. of password cracked: " << c1totalCount << endl;
	cout << (c1totalCount * 100) / 100 << "% of passwords cracked" << endl;
	cout << "Total duration " << c1duration << endl;
	cout << "Category 2: Total No. of password cracked: " << c2totalCount << endl;
	cout << (c2totalCount * 100) / 100 << "% of passwords cracked" << endl;
	cout << "Total duration " << c2duration << endl;

}

void PasswordManager::passwordTree(Node* currentNode, string line) {
	do {
		string sub;
		if (line == "") return;
		for (int y = 1; y < 4 && y <= line.length(); y++) {
			sub = line.substr(0, y);
			//	sub = (substringCount == length - 1) ? line.substr(totalChars, line.length() - totalChars) : line.substr(totalChars, sublength);
			if (collatzMap.find(sub) != collatzMap.end()) {
				Node* newNode = new Node(sub);
				newNode->parent = currentNode;
				newNode->depth = currentNode->depth + 1;
				newNode->length = currentNode->length + sub.length();
				currentNode->children.push_back(newNode);
			}
		}
		for (auto& node : currentNode->children) {
			passwordTree(node, line.substr(node->value.length(), line.length()));
		}
	} while (currentNode->children.size() == 0);
}

void PasswordManager::passwordTree(Node* currentNode, string line, int subCount) {
	bool valid = true;
	do {
		string sub;
		if (line == "" || currentNode->depth == subCount) return;
		//|| (subCount - currentNode->depth)
		// We know from the collatz map that y will not be more than 3 digits
		for (int y = 1; y < 4 && y <= line.length(); y++) {
			y = (currentNode->depth == subCount - 1) ? line.length() : y;
			sub = line.substr(0, y);
			if (sub[0] == '0') {
				return;
			}
			// sub could be a large integer, stoi using stoi will cause memory range error so it checks the number of digits
			if (sub.length() < 4 &&  collatzMap.find(sub) != collatzMap.end()) {
				Node* newNode = new Node(sub);
				newNode->parent = currentNode;
				newNode->depth = currentNode->depth + 1;
				newNode->length = currentNode->length + sub.length();
				currentNode->children.push_back(newNode);
			}
			
		}
		valid = !valid;
		for (auto& node : currentNode->children) {
			passwordTree(node, line.substr(node->value.length(), line.length()), subCount);
		}
	} while (currentNode->children.size() == 0 && valid);
}

void PasswordManager::getValidStringSet(Node* root, int subCount) {
	if (root->children.size() > 0) {
		for (auto node : root->children) {
			getValidStringSet(node, subCount);
		}
	}
	if (root->depth == subCount);
//	return root->parent;
}


void PasswordManager::deleteTree(Node* root) {
	if (root == NULL) return;
	for (auto node : root->children) {
		deleteTree(node);
	}
	if (root->parent == NULL) return;
	delete root;
	
	root = NULL;
}

string PasswordManager::encryptPassword(unsigned char* pwd) {
	int offset = 0;
	string encryptedPwd;
	for (int i = 0; pwd[i] != '\0'; i++) {
		int n = int(pwd[i]) + offset;
		offset = collatzEncrypt(n);
		encryptedPwd = encryptedPwd + to_string(offset);
	}
	return encryptedPwd;
}

inline int PasswordManager::collatzEncrypt(int n) {
	int steps = 0;
	while (n != 1) {
		n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
		steps++;
	}
	return steps;
}

/*unordered_map<int, vector<int> > PasswordManager::createCollatzMap() {
	unordered_map<int, vector<int>> map;
	for (int i = 32; i < 255; i++) {
		int steps = collatzEncrypt(i);
		auto it = map.find(steps);
		if (it == map.end()) {
			vector<int> numbers;
			numbers.push_back(i);
			map.insert({ steps, numbers });
		}
		else {
			map[it->first].push_back(i);
		}
	}
	return map;
}*/

map<string, vector<int> > PasswordManager::createCollatzMap() {
	map<string, vector<int>> map;
	for (int i = 1; i <= 255 + 127; i++) {
		int steps = collatzEncrypt(i);
		auto it = map.find(to_string(steps));
		if (it == map.end()) {
			vector<int> numbers;
			numbers.push_back(i);
			map.insert({ to_string(steps), numbers });
		}
		else {
			map[it->first].push_back(i);
		}
	}
	return map;
}


	/*string encryptPassword(string pwd) {
		int offset = 0;
		string encryptedPwd;
		for (string::size_type i = 0; i < pwd.size(); i++) {
			int steps = 0;
			int n = unsigned int(pwd[i]) + offset;
			while (n != 1) {
				n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
				steps++;
			}
			offset = steps;
			encryptedPwd = encryptedPwd + to_string(steps);
			//collatzEncrypt(pwd[i] + offset);
		}
		return encryptedPwd;
	}*/

	/*int collatzEncrypt(int n) {
		int steps = 0;
		if (n == 1) {
			return steps;
		}
		if (n % 2 == 0) {
			n = n / 2;
			steps++;
		}
		else {
			n = (3 * n) + 1;
			steps++;
		}
		collatzEncrypt(n);
	}*/


	/*void PasswordManager::analyseFile() {
		string line;
		ifstream file("passwordtest.txt");
		int length = 1;
		int crackCount = 0;
		unsigned char* guess;
		int offset;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				guess = new unsigned char[length + 1];
				guess[length] = '\0';
				offset = 0;
				getline(file, line);
				if (line == "1135249") {
					cout << line << endl;
				}


				if (length == 1) {
					if (to_string(collatzEncrypt(collatzMap[stoi(line)][0])) == line) {
						crackCount++;
					}
				}
				else {
					unsigned char* start = guess;
					// Find first valid substring of password
					//TODO: If sublength count < length - 1, break because combination isn't possible;
					int sublength;
					int steps = 0;
					int substringCount = 0;
					int totalChars = 0;
					string previousSub;
					//for (int totalChars = 0; totalChars < line.length();) {
					while (totalChars < line.length()) {
						sublength = 1;
						//string sub;
						string sub = (substringCount == length - 1) ? line.substr(totalChars, line.length() - totalChars) : line.substr(totalChars, sublength);

						if (substringCount == length - 1) {
							sub = line.substr(totalChars, line.length() - totalChars);
					/*	if (collatzMap.find(stoi(sub)) == collatzMap.end()) {

							}*/
							/*	}
								else {
									sub = line.substr(totalChars, sublength);
								}
								sublength = sub.length();
								auto it = collatzMap.find(stoi(sub));
								if ((totalChars + sublength) <= line.length()) {
									while (it == collatzMap.end()) {
										if (sub.length() >= 3) {
											sub = line.substr(totalChars - previousSub.length(), previousSub.length() + 1);
											it = collatzMap.find(stoi(sub));
											//if (it == collatzMap.end()) {
												//backtrack;
											//}
											substringCount--;
											totalChars -= previousSub.length();
											break;
										}
										sublength++;
										sub = line.substr(totalChars, sublength);
										it = collatzMap.find(stoi(sub));
									}
									steps = it->first;
									substringCount++;
									totalChars += sub.length();
									int d = it->second[0] - offset;
									*guess = it->second[0] - offset;
									guess++;

									//g = it->second[0] - offset;
									offset = steps;
								}*/
								/*else if(it != collatzMap.end()) {

									crackCount += (encryptPassword(guess) == line) ? 1 : 0;
								}*/
								/*				previousSub = sub;
											}
											guess = start;
											for (int y = 0; y < length; y++) {
												cout << guess[y];
											}
											cout << endl;
											crackCount += (encryptPassword(guess) == line) ? 1 : 0;

										}

									}
									delete[] guess;
									length++;
								}
								cout << "No. of password cracked: " << crackCount << endl;
								cout << (crackCount / 100) * 100 << "% of passwords cracked" << endl;

							}*/



