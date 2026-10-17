#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#define MAX 20
using namespace std;
using namespace std::chrono;

struct SUBJECTS {
	string name;
	double time = 0;
};

const string FILENAME = "Time.txt";

string toLowerString(string str){
	transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return tolower(c);
	});
	return str;
}

string formatTimeStr(int totalSeconds) {
	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds % 3600) / 60;
	int seconds = totalSeconds % 60;

	string hh = (hours < 10 ? "0" : "") + to_string(hours);
	string mm = (minutes < 10 ? "0" : "") + to_string(minutes);
	string ss = (seconds < 10 ? "0" : "") + to_string(seconds);

	return hh + ":" + mm + ":" + ss;
}

void displayTime(int totalSeconds) {

	cout << "[ " << formatTimeStr(totalSeconds) << " ]" << endl << endl;
}

void saveToFile(SUBJECTS s[], int p) {
	ofstream file(FILENAME);
	if (!file.is_open()) {
		cout << "Error: Could not save data to file!" << endl;
		return;
	}
	for (int i = 0; i < p; i++) {
		file << s[i].name << "," << s[i].time << endl;
	}
	file.close();
}

int loadFromFile(SUBJECTS s[]) {
	ifstream file(FILENAME);
	int p = 0;

	if (!file.is_open()) {
		return 0;
	}

	string name;
	double time;

	while (p < MAX && getline(file, name, ',')) {
		if (file >> time) {
			s[p].name = name;
			s[p].time = time;
			p++;
		}
		file >> ws;
	}

	file.close();
	return p;
}

int menu() {
	int response;
	cout << "=======================" << endl;
	cout << "1. Add subject" << endl;
	cout << "2. Remove subject" << endl;
	cout << "3. Start timer" << endl;
	cout << "4. View your time" << endl;
	cout << "5. View all subjects" << endl;
	cout << "6. Exit" << endl;
	cout << "=======================" << endl;
	cin >> response;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return response;
}

int add(SUBJECTS s[], int p) {
	if (p >= MAX) {
		cout << "Error: Maximum subject limit (" << MAX << ") reached!" << endl<<endl;
		return p;
	}
	string name;
	bool alreadyExists;

	while (true) {
		cout << "Enter the subject name (or '0' to cancel): ";
		getline(cin, name);
		alreadyExists = false;

		if (name == "0") {
			return p;
		}
		if (name.empty()) {
			continue;
		}

		for (int i = 0; i < p; i++) {
				if (toLowerString(name) == toLowerString(s[i].name)) {
					alreadyExists = true;
					break;
				}
		}

		if (alreadyExists) {
			cout << "Subject already exists. Please try again." << endl;
		}
		else {
			s[p].name = name;
			cout << "Subject added successfully." << endl<<endl;
			p++;
			saveToFile(s, p);
			return p;
		}
	}
}

int remove(SUBJECTS s[], int p) {

	if (p == 0) {
		cout << "No subjects to remove. Please add a subject first." << endl << endl;
		return p;
	}


	string name;
	while (true) {
		cout << "Enter the subject to delete (or '0' to cancel): ";
		getline(cin, name);

		if (name == "0") return p;
		if (name.empty()) continue;

		int foundIndex = -1;

		for (int i = 0;i < p;i++) {

			if (toLowerString(name) == toLowerString(s[i].name)) {
				foundIndex = i;
				break;
			}
		}
		if (foundIndex == -1) {
			cout << "Subject not found. Please try again." << endl;
		}
		else {
			for (int j = foundIndex; j < p - 1; j++) {
				s[j] = s[j + 1];
			}
			p--;
			cout << "Subject deleted successfully." << endl << endl;
			saveToFile(s, p);
			return p;
		}

		
	}
}

void view(SUBJECTS s[],int p){

	if (p == 0) {
		cout << "No subjects to view. Please add a subject first." << endl << endl;
		return;
	}


	string name;
	int i;
	while(true) {
		bool found = false;
		cout << "Enter the subject you want to see (or '0' to cancel): ";
		getline(cin, name);
		if (name == "0") {
			return;
		}
		for (i = 0;i < p;i++) {
			if (toLowerString(name) == toLowerString(s[i].name)) {
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Subject not found. Please try again." << endl;
		}
		else {
			cout << endl << "----------------------" << endl;
			cout <<"Subject: " << s[i].name << endl;
			cout << "Time: ";
			displayTime(s[i].time);
			break;
		}
	}
}
			
void start(SUBJECTS s[],int p) {
	if (p == 0) {
		cout << "Add a subject first. Press Enter to continue...";
		cin.get();
		return;
	}

	string name;
	int i;
	while (true) {
		bool found = false;
		cout << "Enter the subject you want to start tracking (or '0' to cancel): ";
		getline(cin, name);
		if (name == "0") {
			return;
		}
		for (i = 0;i < p;i++) {
			if (toLowerString(name) == toLowerString(s[i].name)) {
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Subject not found. Please try again." << endl;
		}
		else {
			cout << "press Enter to start the timer" << endl;
			cin.get();
			auto start_time = steady_clock::now();

			cout << "Timer started! Press Enter to stop it..." << endl<<endl;
			cin.get();

			auto end_time = steady_clock::now();
			duration<double> elapsed = end_time - start_time;
			double seconds_elapsed = elapsed.count();

			cout << "Session time: ";
			displayTime(seconds_elapsed);

			s[i].time += seconds_elapsed;
			saveToFile(s, p);
			break;
		}
	}
}

void viewAll(SUBJECTS s[], int p) {
	if (p == 0) {
		cout << "No subjects to view. Please add a subject first." << endl << endl;
		return;
	}

	cout << endl << "=========================================" << endl;
	cout << "             STUDY DASHBOARD             " << endl;
	cout << "=========================================" << endl;
	cout << left << setw(25) << "Subject Name" << " | " << "Time Studied" << endl;
	cout << "-----------------------------------------" << endl;

	int totalOverallSeconds = 0;
	for (int i = 0; i < p; i++) {
		cout << left << setw(25) << s[i].name << " | " << formatTimeStr(static_cast<int>(s[i].time)) << endl;
		totalOverallSeconds += static_cast<int>(s[i].time);
	}

	cout << "-----------------------------------------" << endl;
	cout << left << setw(25) << "TOTAL STUDY TIME"
		<< " | " << formatTimeStr(totalOverallSeconds) << endl;
	cout << "=========================================" << endl << endl;
}

int main() {
	SUBJECTS subjects[MAX];
	int choice;
	int position = 0;

	position = loadFromFile(subjects);

	while (true) {

		choice = menu();

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (choice) {
		case 1:
			position = add(subjects, position);
			break;
		case 2:
			position = remove(subjects, position);
			break;
		case 3:
			start(subjects, position); 
			break;
		case 4:
			view(subjects, position);
			break;
		case 5:
			viewAll(subjects, position);
			break;
		case 6:
			return 0;
		default:
			cout << "Invalid choice.Please try again." << endl << endl;
		}
	}
}





