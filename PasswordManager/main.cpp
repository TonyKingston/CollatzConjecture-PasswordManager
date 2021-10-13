#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "User.h"

using namespace std;

void createUser(string username, string password) {
	User newUser = User(username, password);
	ofstream file("password.txt");
	if (file.is_open()) {
		file << newUser;
		file.close();
	} else {
		cout << "File could not be opened" << endl;
	}
		
}

bool checkUser() {
	return 0;
}

bool generateFile() {
	return 0;
}
void analyseFile() {

}

string encryptPassword(string pwd) {
	int offset = 0;
	string encryptedPwd;
	for (string::size_type i = 0; i < pwd.size(); i++) {
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

int main() {
	string username;
	string password;
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
		password = encryptPassword(password);
		//int encryptedPwd = encryptPassword(password);
		createUser(username, password);
		break;
	  case 2:
		checkUser();
		break;
	  case 3:
		generateFile();
		break;
	  case 4:
		analyseFile();
		break;
	  default:
		break;
	}

}