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
#include "PasswordManager.h"


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

int main() {
	//SetConsoleOutputCP(GetACP());
	cout << char(129) << endl;
	unsigned char c = char(128);
	cout << c << endl;
	cout << '€' << endl;
	cout << abs(int('ƒ')) << endl;

	PasswordManager manager = PasswordManager();
	manager.start();
}