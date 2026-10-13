#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <fstream>
#define MAX 10
using namespace std;
using namespace std::chrono;

struct SUBJECTS {
	string name;
	double hour = 0;
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

int add(SUBJECTS s[], int p) {
	if (p >= MAX) {
		cout << "Error: Maximum subject limit reached!" << endl<<endl;
		return p;
	}
	string name;
	bool alreadyExists;

	while (true) {
		cout << "Enter the subject name: ";
		cin >> name;
		alreadyExists = false;

		for (int i = 0; i < p; i++) {
			if (name == s[i].name) {
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
			return p;
		}
	}
}

void view(SUBJECTS s[]){
	string name;
	bool found = false;
	int i;
	while(true) {
		cout << "Enter the subject you want to see: ";
		cin >>name;
		for (i = 0;i < MAX;i++) {
			if (name == s[i].name) {
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Subject not found. Please try again." << endl;
		}
		else {
			cout << endl <<"Subject: " << s[i].name << endl;
			cout << "Hours studied: " << s[i].hour << endl<<endl;
			break;
		}
	}
}

			
void start(SUBJECTS s[]) {
	string name;
	int i;
	while (true) {
		bool found = false;
		cout << "Enter the subject you want to start tracking: ";
		cin >> name;
		for (i = 0;i < MAX;i++) {
			if (name == s[i].name) {
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Subject not found. Please try again." << endl;
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			auto start_time = steady_clock::now();

			cout << "Timer started! Press Enter to stop it..." << endl<<endl;
			cin.get();

			auto end_time = steady_clock::now();
			duration<double> elapsed = end_time - start_time;
			double seconds_elapsed = elapsed.count();

			s[i].hour += seconds_elapsed;

			cout << "Time for this session: " << seconds_elapsed << " seconds." << endl;
			cout << "Total time accumulated: " << s[i].hour << " seconds." << endl;

			ofstream outfile("timer_log.txt");
			if (outfile.is_open()) {
				outfile << "Total time accumulated: " << s[i].hour << " seconds." << endl;
				outfile.close();
				cout << "Updated value saved in 'timer_log.txt'." << endl;
			}
			else {
				cerr << "Error opening the file!" << endl;
			}
			break;
		}
	}
}

int main() {
	SUBJECTS subjects[MAX];
	int choice;
	int position = 0;

	while (true) {

		choice = menu();

		switch (choice) {
		case 1:
			position = add(subjects, position);
			break;
		case 2:
			start(subjects); 
			break;
		case 3:
			view(subjects);
			break;
		case 4:
			return 0;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	}
}

