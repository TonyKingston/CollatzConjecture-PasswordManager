#include "User.h"
#include <iostream>

User::User(string name, string pwd) {
	username = name;
	password = pwd;
}

User::~User() {

}

ostream& operator<<(ostream& os, const User& usr) {
	os << usr.username << " " << usr.password;
	return os;
}