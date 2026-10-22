#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <thread>
#include <conio.h>
#define MAX 20
using namespace std;
using namespace std::chrono;


struct SUBJECTS {
	string name;
	double time = 0;
};

const string FILENAME = "Time.txt";

void clearScreen() {
	system("cls");
}

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
	cout << "4. View stats" << endl;
	cout << "5. View calendar" << endl;
	cout << "6. Exit" << endl;
	cout << "=======================" << endl;
	cin >> response;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	else {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return response;
}

void viewCalendar() {
	cout << endl << "==================================" << endl;
	cout << "             CALENDAR             " << endl;
	cout << "==================================" << endl;

	auto currentTime = system_clock::to_time_t(system_clock::now());
	tm localTimeStruct;
	localtime_s(&localTimeStruct, &currentTime);
	tm* localTime = &localTimeStruct;

	int currentYear = localTime->tm_year + 1900;
	int currentMonth = localTime->tm_mon;

	string monthNames[] = {"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER" };
	int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	if ((currentYear % 4 == 0 && currentYear % 100 != 0) || (currentYear % 400 == 0)) {
		daysInMonth[1] = 29;
	}

	tm firstOfMonth = *localTime;
	firstOfMonth.tm_mday = 1;
	mktime(&firstOfMonth);

	int initialSpaces = (firstOfMonth.tm_wday == 0) ? 6 : firstOfMonth.tm_wday - 1;
	cout << "          " << monthNames[currentMonth] << " " << currentYear << endl << endl;
	cout << " Mon  Tue  Wed  Thu  Fri  Sat  Sun" << endl;
	cout << "----------------------------------" << endl;

	for (int i = 0;i < initialSpaces;i++) {
		cout << "     ";
	}

	for (int day = 1;day <= daysInMonth[currentMonth]; day++) {
		string dayStr = "";
		if (day == localTime->tm_mday) {
			if (day < 10) {
				dayStr = " [ " + to_string(day) + "]";
			}
			else {
				dayStr = " [" + to_string(day) + "]";
			}
		}
		else {
			if (day == localTime->tm_mday + 1) {
				if (day < 10) {
					dayStr = "  " + to_string(day);
				}
				else {
					dayStr = " " + to_string(day);
				}
			}
			else {
				if (day < 10) {
					dayStr = "   " + to_string(day);
				}
				else {
					dayStr = "  " + to_string(day);
				}
			}
		}
		cout << dayStr << " ";
		if ((day + initialSpaces) % 7 == 0) {
			cout << endl;
		}
	}

	if ((daysInMonth[currentMonth] + initialSpaces) % 7 != 0) {
		cout << endl;
	}
	cout << endl << "---------------------------------" << endl;
	cout << "Current day is highlighted in [ ]" << endl << endl;
	cout << "Press enter to return to menu";
	cin.get();
	clearScreen();
}

int add(SUBJECTS s[], int p) {
	if (p >= MAX) {
		cout << "Error: Maximum subject limit (" << MAX << ") reached!" << endl<<endl;
		cout << "Invalid choice" << endl << "Press enter to return";
		cin.get();
		clearScreen();
		return p;
	}
	string name;
	bool alreadyExists;
	while (true) {
		cout << "Enter the subject name (or '0' to cancel): ";
		getline(cin, name);
		alreadyExists = false;

		if (name == "0") {
			clearScreen();
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
			cout << "Subject added successfully."<<endl;
			p++;
			saveToFile(s, p);
			cout << "Press enter to return";
			cin.get();
			clearScreen();
			return p;
		}
	}
}

int remove(SUBJECTS s[], int p) {

	if (p == 0) {
		cout << "No subjects to remove. Please add a subject first." << endl << "Press enter to return";
		cin.get();
		clearScreen();;
		return p;
	}


	string name;
	while (true) {
		cout << "Enter the subject to delete (or '0' to cancel): ";
		getline(cin, name);

		if (name == "0") {
			clearScreen();
			return p;
		}
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
			cout << "Subject deleted successfully."<<endl;
			saveToFile(s, p);
			cout << "Press enter to return";
			cin.get();
			clearScreen();;
			return p;
		}

		
	}
}
			
void start(SUBJECTS s[],int p) {
	if (p == 0) {
		cout << "Add a subject first" << endl << "Press enter to return";
		cin.get();
		clearScreen();
		return;
	}

	string name;
	int i;
	while (true) {
		bool found = false;
		cout << "Enter the subject you want to start tracking (or '0' to cancel): ";
		getline(cin, name);
		if (name == "0") {
			clearScreen();
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
			while (true) {
				int mode;
				cout <<endl<<"  CHOOSE TRACKING MODE " << endl;
				cout << "1. Standard Stopwatch Timer" << endl;
				cout << "2. Pomodoro Timer" << endl;
				cin >> mode;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Invalid input. Please enter a number" << endl << endl;
					continue;
				}
				if (mode == 1) {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "press Enter to start the timer" << endl;
					cin.get();
					auto start_time = steady_clock::now();

					cout << "Timer started! Press Enter to stop it..." << endl << endl;
					cin.get();

					auto end_time = steady_clock::now();
					duration<double> elapsed = end_time - start_time;
					double seconds_elapsed = elapsed.count();

					cout << "Session time: ";
					displayTime(seconds_elapsed);

					s[i].time += seconds_elapsed;
					saveToFile(s, p);
					cout << endl <<"Press enter to return";
					cin.get();
					clearScreen();;
					return;
				}
				else if (mode == 2) {
					int activeTime;
					int pauseTime;
					int rounds;
					cout << "Enter study duration in minutes (e.g., 25): ";
					cin >> activeTime;
					cout << "Enter break duration in minutes (e.g., 5): ";
					cin >> pauseTime;
					cout << "Enter the number of rounds to complete (e.g., 4): ";
					cin >> rounds;

					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					cout << "press Enter to start the timer" << endl;
					cin.get();
					int activeTimeSeconds = activeTime * 60;
					int pauseTimeSeconds = pauseTime * 60;

					bool stopEarly = false;
					cout << "[Press 'Q' to Quit early]"<<endl;
					for (int r = 1;r <= rounds;r++) {
						int currentSeconds = 0;
						while (currentSeconds <= activeTimeSeconds && !stopEarly) {
							cout << "\rstudying time: " << formatTimeStr(currentSeconds) << "/" << formatTimeStr(activeTimeSeconds) << flush;

							this_thread::sleep_for(seconds(1));

							if (_kbhit()) {
								char ch = _getch();
								if (ch == 'q' || ch == 'Q') {
									stopEarly = true;
								}
							}
							if(!stopEarly) currentSeconds++;
						}

						if (stopEarly) {
							s[i].time += currentSeconds;
							saveToFile(s, p);
							cout << endl << "Session stopped early!"<< endl << "Press enter to return";
							cin.get();
							clearScreen();;
							return;
						}
						cout <<endl << "Studying time ended"<<endl;

						s[i].time += activeTimeSeconds;
						saveToFile(s, p);

						if (r == rounds)break;

						currentSeconds = 0;
						while (currentSeconds <= pauseTimeSeconds) {
							cout << "\rbreak time: " << formatTimeStr(currentSeconds) << "/" << formatTimeStr(pauseTimeSeconds) << "   " << flush;
							
							this_thread::sleep_for(seconds(1));
							if (_kbhit()) {
								char ch = _getch();
								if (ch == 'q' || ch == 'Q') {
									stopEarly = true;
								}
							}

							if (!stopEarly) currentSeconds++;
						}

						if (stopEarly) {
							cout << endl << "Session stopped early!"<<endl  << "Press enter to return";
							cin.get();
							clearScreen();;
							return;
						}
						cout <<endl<< "Break time ended" << endl;
					}
					cout << "Pomodoro timer ended" << endl << "Press enter to return";
					cin.get();
					clearScreen();
					return;
				}
				else {
					cout << "Invalid choice.Please try again." << endl << endl;
				}
			}
		}
	}
}

void viewStats(SUBJECTS s[], int p) {
	if (p == 0) {
		cout << "No subjects to calculate stats for" << endl << "Press enter to return";
		cin.get();
		clearScreen();
		return;
	}
	while (true) {
		int scelta;
		cout << endl << "1. view one subject" << endl;
		cout << "2. view all subjects" << endl;
		cout << "0. cancel" << endl;
		cin >> scelta;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		if (scelta == 0) {
			return;
		}
		if (scelta == 1) {
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			string name;
			while (true) {
				bool found = false;
				int i;
				cout << "Enter the subject you want to see (or '0' to cancel): ";
				getline(cin, name);
				if (name == "0") {
					clearScreen();
					return;
				}
				for (i = 0;i < p;i++) {
					if (toLowerString(name) == toLowerString(s[i].name)) {
						found = true;
						break;
					}
				}
				if (found) {
					cout << endl << "Subject: " << s[i].name << endl;
					cout << "Time: "<< endl;
					displayTime(s[i].time);
					cout << "Press enter to return";
					cin.get();
					clearScreen();
					return;

				}
				cout << "Subject not found. Please try again" << endl;
			}
		}
		else if (scelta == 2) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl << "=========================================" << endl;
			cout << "             STUDY DASHBOARD             " << endl;
			cout << "=========================================" << endl;
			cout << left << setw(25) << "Subject Name" << " | " << "Time Studied" << endl;
			cout << "-----------------------------------------" << endl;

			int totalOverallSeconds = 0;
			for (int i = 0;i < p;i++) {
				cout << left << setw(25) << s[i].name << " | " << formatTimeStr(static_cast<int>(s[i].time)) << endl;
				totalOverallSeconds += static_cast<int>(s[i].time);
			}
			cout << "-----------------------------------------" << endl;
			cout << left << setw(25) << "TOTAL STUDY TIME"
				<< " | " << formatTimeStr(totalOverallSeconds) << endl;
			cout << "=========================================" << endl;
			cout << "Press enter to return";
			cin.get();
			clearScreen();
			return;
		}
		else {
			cout << "Invalid choice .Please try again." << endl << endl;
		}
	}
}

int main() {
	SUBJECTS subjects[MAX];
	int choice;
	int position = 0;

	position = loadFromFile(subjects);

	cout << "=======================" << endl;
	cout << "     STUDY TRACKER     " << endl;

	while (true) {

		choice = menu();

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
			viewStats(subjects, position);
			break;
		case 5:
			viewCalendar();
			break;
		case 6:
			return 0;
		default:
			cout << "Invalid choice"<<endl<<"Press enter to return";
			cin.get();
			clearScreen();
		}
	}
}