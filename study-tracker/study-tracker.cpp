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
	string color = "";
	double time = 0;
};

struct TESTS {
	string color = "";
	int day = 0;
	int month = 0;
};

const string FILENAMESUBJECTS = "Time.txt";
const string FILENAMETESTS = "Tests.txt";

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

int loadSubjectsFromFile(SUBJECTS s[]) {
	ifstream file(FILENAMESUBJECTS);
	int p = 0;

	if (!file.is_open()) {
		return 0;
	}

	string name, color, timeStr;

	while (p < MAX && getline(file, name, ',')) {
		if (getline(file, color, ',') && getline(file, timeStr)) {
			s[p].name = name;
			s[p].color = color;
			s[p].time = stod(timeStr);
			p++;
		}
	}

	file.close();
	return p;
}

void saveSubjectsToFile(SUBJECTS s[], int p) {
	ofstream file(FILENAMESUBJECTS);
	if (!file.is_open()) {
		cout << "Error: Could not save data to file!" << endl;
		return;
	}
	for (int j = 0;j < p - 1;j++) {
		for (int k = 0;k < p - j - 1;k++) {
			if (s[k].time < s[k + 1].time) {
				SUBJECTS q = s[k];
				s[k] = s[k + 1];
				s[k + 1] = q;
			}
		}

	}

	for (int i = 0; i < p; i++) {
		file << s[i].name << "," << s[i].color << "," << s[i].time <<endl;
	}
	file.close();
}

int loadTestsFromFile(TESTS t[]) {
	ifstream file(FILENAMETESTS);
	int p = 0;

	if (!file.is_open()) {
		return 0;
	}

	string color;
	string dayStr;
	string monthStr;

	while (p < MAX &&
		getline(file, color, ',') &&
		getline(file, dayStr, ',') &&
		getline(file, monthStr)) {

	
			t[p].color = color;
			t[p].day = stoi(dayStr);
			t[p].month = stoi(monthStr);
			p++;
		
	}
	file.close();
	return p;
}

void saveTestsToFileTests(TESTS t[], int p) {
	ofstream file(FILENAMETESTS);
	if (!file.is_open()) {
		cout << "Error: Could not save data to file!" << endl;
		return;
	}

	for (int i = 0; i < p; i++) {
		file << t[i].color << "," << t[i].day << "," << t[i].month << endl;
	}
	file.close();
}


int menu() {
	int response;
	cout << "=======================" << endl;
	cout << "1. Add subject" << endl;
	cout << "2. Remove subject" << endl;
	cout << "3. Start timer" << endl;
	cout << "4. View stats" << endl;
	cout << "5. add test" << endl;
	cout << "6. remove test" << endl;
	cout << "7. View calendar" << endl;
	cout << "8. Exit" << endl;
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

void addColor(SUBJECTS s[],int k,int p) {
	while (true){
		bool alreadyUsed = false;
		int choice;
		string color;
		cout << "choose the color for the subject:" << endl;
		cout << "1. red" << endl;
		cout << "2. green" << endl;
		cout << "3. yellow" << endl;
		cout << "4. blue" << endl;
		cout << "5. magenta" << endl;
		cout << "6. cyan" << endl;
		cin >> choice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid action. Please insert a valid action" << endl;
			continue;
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		switch (choice) {
		case 1:
			color = "\033[31m";
			break;
		case 2:
			color = "\033[32m";
			break;
		case 3:
			color = "\033[33m";
			break;
		case 4:
			color = "\033[34m";
			break;
		case 5:
			color = "\033[35m";
			break;
		case 6:
			color = "\033[36m";
			break;
		default:
			cout << endl<<"Invalid action.Please insert a valid action" << endl;
			continue;
		}
		for (int i = 0;i < p;i++) {
			if (color == s[i].color) {
				alreadyUsed = true;
				break;
			}
		}
		if (alreadyUsed) {
			cout << endl<<"Color already in use" << endl;
			continue;
		}
		else {
			s[k].color = color;
			return;
		}
	}
}

int addTest(SUBJECTS s[], TESTS t[], int p,int n) {
	if (p == 0) {
		cout << "No subjects for the test. Please add a subject first." << endl << "Press enter to return";
		cin.get();
		clearScreen();;
		return n;
	}


	string name;
	while (true) {
		bool found = false;
		cout << "choose the subject(or '0' to cancel):" << endl;
		for (int i = 0;i < p;i++) {
			cout << s[i].name << " | ";
		}
		cout << endl;
		getline(cin, name);

		if (name == "0") {
			clearScreen();
			return n;
		}
		int i;
		for ( i = 0;i < p;i++) {
			if (toLowerString(name) == toLowerString(s[i].name)) {
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Subject not found. Please try again." << endl;
			continue;
		}
		else {
			t[n].color = s[i].color;
			int daysInMonth[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
			while (true) {
				cout << "Enter the day of the test: ";
				int day;
				cin >> day;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Error. Enter a number" << endl;
					continue;
				}
				else {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				cout << "Enter the the month of the test(in number): ";
				int month;
				cin >> month;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Error. Enter a number" << endl;
					continue;
				}
				else {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				if (month > 12) {
					cout << "Insert a valid month" << endl;
					continue;
				}
				else if (daysInMonth[month] < day) {
					cout << "Insert a valid day" << endl;
					continue;
				}
				else {
					month--;
					t[n].day = day;
					t[n].month = month;
					n++;
					saveTestsToFileTests(t, n);
					cout << "Test added successfully" << endl;
					cout << "Press enter to return";
					cin.get();
					clearScreen();;
					return n;
				}
			}
		}


	}


}

int deleteTest(SUBJECTS s[], TESTS t[], int p, int n) {
	if (p == 0 || n == 0) {
		cout << "No tests to remove. Please add a test first." << endl << "Press enter to return";
		cin.get();
		clearScreen();;
		return n;
	}

	string name;
	while (true) {
		bool found = false;
		cout << "insert the subject of the test you want to remove(or '0' to cancel)"<<endl;
		for (int j = 0;j < p;j++) {
			cout << s[j].name << " | ";
		}
		cout << endl;
		getline(cin, name);
		if (name == "0") {
			clearScreen();
			return n;
		}

		int  i;
		for (i = 0;i < p;i++) {

			if (toLowerString(name) == toLowerString(s[i].name)) {
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Subject not found. Please try again" << endl;
			continue;
		}
		else {
			bool foundT = false;
			for (int j = 0;j < n;j++) {
				if (s[i].color == t[j].color) {
					foundT = true;
					break;
				}
			}
			if(!foundT){
				cout << "This subject doesn't have tests. Pleas try again" << endl << endl;
				continue;
			}
			else {
				int month;
				while (true) {
					bool foundM = false;
					cout << "Insert the month(in number)(or '0' to cancel): ";
					cin >> month;

					if (cin.fail()) {
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Error. Enter a number" << endl << endl;
						continue;
					}
					else {
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}

					if (month == 0) {
						clearScreen();
						return n;
					}
					for (int j = 0;j < n;j++) {
						if (s[i].color == t[j].color && t[j].month == (month - 1)) {
							foundM = true;
							break;
						}
					}
					if (!foundM) {
						cout << "This month doesn't have tests. Pleas try again" << endl << endl;
						continue;
					}
					else {
						int day;
						while (true) {
							bool foundD = false;
							cout << "insert the day(or '0' to cancel): " << endl;
							cin >> day;

							if (cin.fail()) {
								cin.clear();
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
								cout << "Error. Enter a number" << endl << endl;
								continue;
							}
							else {
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
							}
							if (day == 0) {
								clearScreen();
								return n;
							}

							int k;
							for ( k = 0;k < n;k++) {
								if (s[i].color == t[k].color && t[k].month == (month - 1) && t[k].day == day) {
									foundD = true;
									break;
								}
							}
							if (!foundD) {
								cout << "this day doesn't have a test. Pleas try again" << endl << endl;
								continue;
							}
							else {
								for (int j = k;j < n - 1;j++) {
									t[j] = t[j + 1];
								}
								n--;
								cout << "Test deleted" << endl;
								saveTestsToFileTests(t, n);
								cout << "Press enter to return to menu";
								cin.get();
								clearScreen();
								return n;
							}
						}
					}
				}
			}
		}
	}
}

int deleteSubjectTests(SUBJECTS s[], TESTS t[], int k, int n) {
	for (int i = 0;i < n;i++) {
		if (t[i].color == s[k].color) {
			for (int j = i;j < n - 1;j++) {
				t[j] = t[j + 1];
			}
			n--;
			i--;
		}
	}
	saveTestsToFileTests(t, n);
	return n;
}

void viewCalendar(SUBJECTS s[], TESTS t[],int p,int n) {
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
		string color = "\033[0m";

		for (int i = 0;i < n;i++) {
			string rawColor = t[i].color;
			while (!rawColor.empty() && isspace(rawColor.back())) {
				rawColor.pop_back();
			}

			if (t[i].month == currentMonth) {
				if (t[i].day == day) {
					color = "\033" + rawColor;
					break;
				}
			}
		}

		if (day == localTime->tm_mday) {
			if (day < 10) {
				dayStr = " [ " + color + to_string(day) + "\033[0m" + "]";
			}
			else {
				dayStr = " [" + color + to_string(day) + "\033[0m" + "]";
			}
		}
		else {
			if (day == localTime->tm_mday + 1) {
				if (day < 10) {
					dayStr = "  " + color + to_string(day) + "\033[0m";
				}
				else {
					dayStr = " " + color + to_string(day) + "\033[0m";
				}
			}
			else {
				if (day < 10) {
					dayStr = "   " + color + to_string(day) + "\033[0m";
				}
				else {
					dayStr = "  " + color + to_string(day) + "\033[0m";
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
	cout << "Current day is highlighted in [ ]" << endl;
	cout << "Subject: ";
	for (int i = 0;i < p;i++) {
		if (i == 0) {
			cout<< "\033" << s[i].color << s[i].name << "\033[0m" << endl;
		}
		else {
			cout << "\t " << "\033" << s[i].color << s[i].name << "\033[0m" << endl;
		}
	}
	cout <<endl<< "press enter to return to menu";
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
		int k =0;
		for (int i = 0; i < p; i++) {
				if (toLowerString(name) == toLowerString(s[i].name)) {
					alreadyExists = true;
					break;
				}
				k++;
		}

		if (alreadyExists) {
			cout << "Subject already exists. Please try again." << endl;
		}
		else {
			s[p].name = name;
			addColor(s,k,p);
			cout << "Subject added successfully."<<endl;
			p++;
			saveSubjectsToFile(s, p);
			cout << "Press enter to return";
			cin.get();
			clearScreen();
			return p;
		}
	}
}

int remove(SUBJECTS s[], int p,TESTS t[],int &n) {

	if (p == 0) {
		cout << "No subjects to remove. Please add a subject first." << endl << "Press enter to return";
		cin.get();
		clearScreen();;
		return p;
	}


	string name;
	while (true) {
		cout << "Enter the subject to delete (or '0' to cancel):"<<endl;
		for (int j = 0;j < p;j++) {
			cout << s[j].name << " | ";
		}
		cout << endl;
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
			n = deleteSubjectTests(s, t, foundIndex, n);

			for (int j = foundIndex; j < p - 1; j++) {
				s[j] = s[j + 1];
			}
			p--;
			cout << "Subject deleted successfully."<<endl;
			saveSubjectsToFile(s, p);
			cout << "Press enter to return to menu";
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
		cout << "Enter the subject you want to start tracking (or '0' to cancel):"<<endl;
		for (int j = 0;j < p;j++) {
			cout << s[j].name << " | ";
		}
		cout << endl;
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
					saveSubjectsToFile(s, p);
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
							saveSubjectsToFile(s, p);
							cout << endl << "Session stopped early!"<< endl << "Press enter to return";
							cin.get();
							clearScreen();;
							return;
						}
						cout <<endl << "Studying time ended"<<endl;

						s[i].time += activeTimeSeconds;
						saveSubjectsToFile(s, p);

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
		int choice;
		cout << endl << "1. view one subject" << endl;
		cout << "2. view all subjects" << endl;
		cout << "0. cancel" << endl;
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		if (choice == 0) {
			return;
		}
		if (choice == 1) {
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			string name;
			while (true) {
				bool found = false;
				int i;
				cout << "Enter the subject you want to see (or '0' to cancel):" << endl;
				for (int j = 0;j < p;j++) {
					cout << s[j].name << " | ";
				}
				cout << endl;
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
					cout << "Time: ";
					displayTime(s[i].time);
					cout << "Press enter to return";
					cin.get();
					clearScreen();
					return;

				}
				cout << "Subject not found. Please try again" << endl;
			}
		}
		else if (choice == 2) {
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
	TESTS tests[100];
	int choice;
	int position = 0;
	int numberOfTests = 0;

	position = loadSubjectsFromFile(subjects);
	numberOfTests = loadTestsFromFile(tests);

	cout << "=======================" << endl;
	cout << "     STUDY TRACKER     " << endl;

	while (true) {

		choice = menu();

		switch (choice) {
		case 1:
			position = add(subjects, position);
			break;
		case 2:
			position = remove(subjects, position,tests,numberOfTests);
			break;
		case 3:
			start(subjects, position); 
			break;
		case 4:
			viewStats(subjects, position);
			break;
		case 5:
			numberOfTests = addTest(subjects, tests, position, numberOfTests);
			break;
		case 6:
			numberOfTests = deleteTest(subjects, tests,position, numberOfTests);
			break;
		case 7:
			viewCalendar(subjects, tests,position,numberOfTests);
			break;
		case 8:
			return 0;
		default:
			cout << "Invalid choice"<<endl<<"Press enter to return";
			cin.get();
			clearScreen();
		}
	}
}