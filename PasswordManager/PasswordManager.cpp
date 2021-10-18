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



using namespace std;

//map<int, int> collatzMap;



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
	map<int, vector<int>> collatzMap = createCollatzMap();
	string username;
	string password;
	string encryptedPassword;
	unsigned char* arr;
	int option;

	cout << "Please select an option from the following:\n" << endl;
	cout << "1. Create username/password" << endl;
	cout << "2. Check username and password" << endl;
	cout << "3. Generate password strength analysis file" << endl;
	cout << "4. Analyse password strength analysis file" << endl;

	cin >> option;

	switch (option) {
	case 1:
		cout << "Please enter a username" << endl;
		cin >> username;
		cout << "Please enter a password" << endl;
		cin >> password;
		arr = new unsigned char[password.length() + 1];
		strcpy_s((char*)arr, password.length() + 1, password.c_str());
		encryptedPassword = encryptPassword(arr);

		createUser(username, encryptedPassword);
		delete[] arr;
		break;
	case 2:
		cout << "Please enter a username" << endl;
		cin >> username;
		cout << "Please enter a password" << endl;
		cin >> password;
		arr = new unsigned char[password.length() + 1];
		strcpy_s((char*)arr, password.length() + 1, password.c_str());
		encryptedPassword = encryptPassword(arr);
		checkUser(username, encryptedPassword);
		delete[] arr;
		break;
	case 3:
		generateFile();
		break;
	case 4:
		analyseFile();
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

bool PasswordManager::checkUser(string username, string password) {
	string line;
	ifstream file("password.txt");
	while (getline(file, line)) {
		cout << line << endl;
		/*if (username == line)
			if (password == line)
				return true;*/
	}
	return 0;
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
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
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
	int count = 0;
	while (getline(file, line)) {
		cout << line;

	}
}

string PasswordManager::encryptPassword(unsigned char* pwd) {
	int offset = 0;
	string encryptedPwd;
	for (int i = 0; pwd[i] != '\0'; i++) {
		int steps = 0;
		int n = int(pwd[i]) + offset;
		while (n != 1) {
			n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
			steps++;
		}
		offset = steps;
		encryptedPwd = encryptedPwd + to_string(steps);
		//collatzEncrypt(pwd[i] + offset);
	}
	return encryptedPwd;
}

int PasswordManager::collatzEncrypt(int n) {
	int steps = 0;
	while (n != 1) {
		n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
		steps++;
	}
	return steps;
}

map<int, vector<int> > PasswordManager::createCollatzMap() {
	map<int, vector<int>> map;
	for (int i = 1; i < 128; i++) {
		//	c[0] = i;
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



