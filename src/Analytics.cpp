#include "Analytics.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

using namespace std;

bool Analytics::hasPassed(const Student& student) {
    for (double mark : student.marks) {
        if (mark < 40.0) {
            return false;
        }
    }

    return true;
}

string Analytics::calculateGrade(const Student& student) {
    if (!hasPassed(student)) {
        return "F";
    }

    double percentage = calculatePercentage(student);

    if (percentage >= 90) return "O";
    if (percentage >= 80) return "A+";
    if (percentage >= 70) return "A";
    if (percentage >= 60) return "B+";
    if (percentage >= 50) return "B";
    return "C";
}

void Analytics::sortStudents(
    vector<Student> students, int choice
) {
    if (students.empty()) {
        cout << "\nNo students available.\n";
        return;
    }

    switch (choice) {
    case 1:
        sort(students.begin(), students.end(),
             [](const Student& first, const Student& second) {
                 return first.id < second.id;
             });
        break;

    case 2:
        sort(students.begin(), students.end(),
             [](const Student& first, const Student& second) {
                 if (first.name == second.name) {
                     return first.id < second.id;
                 }

                 return first.name < second.name;
             });
        break;

    case 3:
        sort(students.begin(), students.end(),
             [](const Student& first, const Student& second) {
                 double firstPercentage =
                     calculatePercentage(first);
                 double secondPercentage =
                     calculatePercentage(second);

                 if (firstPercentage == secondPercentage) {
                     return first.id < second.id;
                 }

                 return firstPercentage > secondPercentage;
             });
        break;

    default:
        cout << "\nInvalid sorting choice.\n";
        return;
    }

    cout << "\n========== SORTED STUDENTS ==========\n";
    cout << left << setw(10) << "ID"
         << setw(23) << "Name"
         << right << setw(12) << "Percentage"
         << setw(10) << "Grade" << '\n';
    cout << string(55, '-') << '\n';

    for (const Student& student : students) {
        cout << left << setw(10) << student.id
             << setw(23) << student.name.substr(0, 22)
             << right << fixed << setprecision(2)
             << setw(11) << calculatePercentage(student) << "%"
             << setw(9) << calculateGrade(student) << '\n';
    }
}

void Analytics::performanceReport(
    const vector<Student>& students
) {
    if (students.empty()) {
        cout << "\nNo student records available.\n";
        return;
    }

    double totalPercentage = 0;
    int passed = 0;
    int failed = 0;
    const Student* topper = &students.front();

    for (const Student& student : students) {
        double percentage = calculatePercentage(student);
        totalPercentage += percentage;

        if (hasPassed(student)) {
            passed++;
        } else {
            failed++;
        }

        if (percentage > calculatePercentage(*topper)) {
            topper = &student;
        }
    }

    double classAverage = totalPercentage / students.size();

    cout << fixed << setprecision(2);
    cout << "\n========== ACADEMIC REPORT ==========\n";
    cout << "Total Students : " << students.size() << '\n';
    cout << "Class Average  : " << classAverage << "%\n";
    cout << "Passed         : " << passed << '\n';
    cout << "Failed         : " << failed << '\n';
    cout << "Pass Rate      : "
         << 100.0 * passed / students.size() << "%\n";

    cout << "\nHighest Overall Percentage:\n";
    cout << "Student ID     : " << topper->id << '\n';
    cout << "Name           : " << topper->name << '\n';
    cout << "Percentage     : "
         << calculatePercentage(*topper) << "%\n";
    cout << "Grade          : "
         << calculateGrade(*topper) << '\n';

    cout << "\nIndividual Student Performance:\n";

    for (const Student& student : students) {
        cout << student.id << " - " << student.name
             << " | " << calculatePercentage(student) << "%"
             << " | Grade: " << calculateGrade(student)
             << " | " << (hasPassed(student) ? "PASS" : "FAIL")
             << '\n';
    }
}

void Analytics::attendanceAlerts(
    const vector<Student>& students,
    double threshold
) {
    if (students.empty()) {
        cout << "\nNo student records available.\n";
        return;
    }

    int count = 0;

    cout << "\n========== ATTENDANCE ALERTS ==========\n";
    cout << "Minimum required: " << threshold << "%\n\n";

    for (const Student& student : students) {
        if (student.attendance < threshold) {
            count++;

            cout << "ALERT - Student ID: " << student.id
                 << " | Name: " << student.name
                 << " | Attendance: " << fixed
                 << setprecision(2)
                 << student.attendance << "%\n";
        }
    }

    if (count == 0) {
        cout << "All students meet the attendance requirement.\n";
    } else {
        cout << "\nStudents below threshold: "
             << count << '\n';
    }
}
