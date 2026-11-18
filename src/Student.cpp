
#include "Student.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>

using namespace std;

// Read an integer within the allowed range.
int readInt(const string& prompt, int minVal, int maxVal) {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value &&
            value >= minVal && value <= maxVal) {

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Please try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Read a decimal value within the allowed range.
double readDouble(const string& prompt,
                  double minVal, double maxVal) {
    double value;

    while (true) {
        cout << prompt;

        if (cin >> value &&
            isfinite(value) &&
            value >= minVal && value <= maxVal) {

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Please try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Read a non-empty text value.
string readText(const string& prompt) {
    string value;

    while (true) {
        cout << prompt;
        getline(cin, value);

        if (value.find_first_not_of(" \t\r\n") != string::npos) {
            return value;
        }

        cout << "This field cannot be empty.\n";
    }
}

// Collect a student's information.
Student inputStudent() {
    Student student;

    cout << "\n----- ENTER STUDENT DETAILS -----\n";

    student.id = readInt(
        "Student ID: ", 1, numeric_limits<int>::max()
    );

    student.name = readText("Full Name: ");
    student.department = readText("Department: ");

    student.semester = readInt("Semester (1-8): ", 1, 8);

    for (int i = 0; i < 3; i++) {
        student.marks[i] = readDouble(
            "Subject " + to_string(i + 1) + " Marks (0-100): ",
            0, 100
        );
    }

    student.attendance = readDouble(
        "Attendance Percentage (0-100): ", 0, 100
    );

    return student;
}

// Calculate average marks as a percentage.
double calculatePercentage(const Student& student) {
    double total = 0;

    for (double mark : student.marks) {
        total += mark;
    }

    return total / student.marks.size();
}

// Display a student's information.
void displayStudent(const Student& student) {
    cout << "\n========== STUDENT RECORD ==========\n";

    cout << "Student ID : " << student.id << '\n';
    cout << "Name       : " << student.name << '\n';
    cout << "Department : " << student.department << '\n';
    cout << "Semester   : " << student.semester << '\n';

    for (int i = 0; i < 3; i++) {
        cout << "Subject " << i + 1
             << " Marks: " << student.marks[i] << '\n';
    }

    cout << fixed << setprecision(2);

    cout << "Percentage : "
         << calculatePercentage(student) << "%\n";

    cout << "Attendance : "
         << student.attendance << "%\n";

    cout << "====================================\n";
}
