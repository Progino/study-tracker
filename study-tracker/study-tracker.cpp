#include <iostream>
#include <cstdio>
#include <ctime>
#define MAX 10
using namespace std;

struct SUBJECTS {
	string name;
	int hour = 0;
};

int menu() {
	int response;
	cout << "1. Add subject" << endl;
	cout << "2. Start timer" << endl;
	cout << "3. View your time" << endl;
	cout << "4. Exit" << endl;
	cout << "Enter your choice: ";
	cin >> response;
	return response;
}
/*
int add(SUBJECTS *s, int p) {
	s += p;
	string name;
	cout << "Enter the subject name: ";
	cin >> name;
	while (true) {
		for (int i = 0;i < MAX;i++) {
			if (name == s->name) {
				cout << "Subject already exists. Please try again." << endl;
			}
			else {
				s->name = name;
				cout << "subject added successfully." << endl;
				break;
			}
			s += 1;
		}
	}
	p++;
	return p;
}*/

/*
void view(SUBJECTS* s) {
	string name;
	cout << "Enter the subject you want to see: ";
	cin >>name;
	while (true) {
		for (int i = 0;i < MAX;i++) {
			if (name == s->name) {
				cout << "Subject: " << s->name << endl;
				cout << "Hours studied: " << s->hour << endl;
				break;
			}
			else {
				cout << "Subject not found. Please try again." << endl;
			}
			s += 1;
		}
	}

}*/

/*
void start(SUBJECT* s) {

}*/

int main() {
	SUBJECTS subjects[MAX];
	int choice;
	int position = 0;

	while (true) {

		choice = menu();

		switch (choice) {
		case 1:
			//position = add(subjects,position);
			break;
		case 2:
			//start(subjects); 
			break;
		case 3:
			//view(subjects);
			break;
		case 4:
			return 0;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
}//works?

