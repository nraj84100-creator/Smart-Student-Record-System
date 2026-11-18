#include <iostream>
#include <limits>
#include <string>

#include "Student.h"
#include "StudentManager.h"
#include "FileManager.h"
#include "Analytics.h"

using namespace std;

const string DATA_FILE = "data/students.csv";

void displayMenu() {
    cout << "\n========================================\n";
    cout << "    SMART STUDENT RECORD MANAGEMENT\n";
    cout << "========================================\n";
    cout << "1. Add Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Sort Students\n";
    cout << "7. Academic Performance Report\n";
    cout << "8. Attendance Alerts\n";
    cout << "9. Undo Last Change\n";
    cout << "0. Save and Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

int readStudentId() {
    int id;

    cout << "Enter Student ID: ";

    if (!(cin >> id) || id <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return id;
}

void saveChanges(const StudentManager& manager) {
    if (FileManager::saveStudents(manager, DATA_FILE)) {
        cout << "Changes saved successfully.\n";
    } else {
        cout << "WARNING: Changes were not saved."
             << " Retry before closing the program.\n";
    }
}

int main() {
    StudentManager manager;
    int choice;

    cout << "\nLoading student records...\n";

    if (!FileManager::loadStudents(manager, DATA_FILE)) {
        cerr << "\nUnable to load saved records safely.\n";
        cerr << "Check the CSV file before restarting.\n";
        return 1;
    }

    while (true) {
        displayMenu();

        if (!(cin >> choice)) {
            cout << "\nInvalid input. Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            Student student = inputStudent();

            if (manager.addStudent(student)) {
                cout << "\nStudent added successfully!\n";
                saveChanges(manager);
            } else {
                cout << "\nCould not add student. Check for duplicate ID.\n";
            }
            break;
        }

        case 2:
            manager.displayAll();
            break;

        case 3: {
            int id = readStudentId();

            if (id <= 0) {
                cout << "\nInvalid Student ID.\n";
                break;
            }

            const Student* student = manager.searchStudent(id);

            if (student != nullptr) {
                displayStudent(*student);
            } else {
                cout << "\nStudent not found.\n";
            }
            break;
        }

        case 4: {
            cout << "\n--- UPDATE STUDENT ---\n";
            int id = readStudentId();

            if (id <= 0) {
                cout << "\nInvalid Student ID.\n";
                break;
            }

            const Student* existing = manager.searchStudent(id);

            if (existing == nullptr) {
                cout << "\nStudent not found.\n";
                break;
            }

            cout << "\nCurrent Student Details:\n";
            displayStudent(*existing);
            cout << "\nEnter updated details.\n";
            cout << "Student ID must remain " << id << ".\n";

            Student updated = inputStudent();

            if (updated.id != id) {
                cout << "\nError: Student ID cannot be changed.\n";
                break;
            }

            if (manager.updateStudent(updated)) {
                cout << "\nStudent updated successfully!\n";
                saveChanges(manager);
            } else {
                cout << "\nStudent update failed.\n";
            }
            break;
        }

        case 5: {
            cout << "\n--- DELETE STUDENT ---\n";
            int id = readStudentId();

            if (id <= 0) {
                cout << "\nInvalid Student ID.\n";
                break;
            }

            const Student* student = manager.searchStudent(id);

            if (student == nullptr) {
                cout << "\nStudent not found.\n";
                break;
            }

            cout << "\nRecord to delete:\n";
            displayStudent(*student);

            string answer;
            cout << "\nConfirm deletion (y/n): ";
            getline(cin, answer);

            if (answer != "y" && answer != "Y") {
                cout << "\nDeletion cancelled.\n";
                break;
            }

            if (manager.deleteStudent(id)) {
                cout << "\nStudent deleted successfully!\n";
                saveChanges(manager);
            } else {
                cout << "\nStudent deletion failed.\n";
            }
            break;
        }

        case 6: {
            cout << "\n--- SORT STUDENTS ---\n";
            cout << "1. Sort by Student ID\n";
            cout << "2. Sort by Name\n";
            cout << "3. Sort by Percentage (High to Low)\n";
            cout << "Enter sorting choice: ";

            int sortChoice;

            if (!(cin >> sortChoice)) {
                cout << "\nInvalid sorting choice.\n";
                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );
                break;
            }

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );

            Analytics::sortStudents(
                manager.getAllStudents(),
                sortChoice
            );
            break;
        }

        case 7:
            Analytics::performanceReport(
                manager.getAllStudents()
            );
            break;

        case 8:
            Analytics::attendanceAlerts(
                manager.getAllStudents()
            );
            break;

        case 9: {
            cout << "\n--- UNDO LAST CHANGE ---\n";

            if (!manager.canUndo()) {
                cout << "No actions available to undo.\n";
                break;
            }

            if (manager.undoLastChange()) {
                cout << "\nLast change undone successfully!\n";
                saveChanges(manager);
            } else {
                cout << "\nUndo operation failed.\n";
            }

            break;
        }

        case 0:
            cout << "\nSaving student records...\n";

            if (FileManager::saveStudents(manager, DATA_FILE)) {
                cout << "All records saved successfully.\n";
                cout << "Thank you for using the system!\n";
                return 0;
            }

            cout << "\nSave failed. Application remains open"
                 << " so you can retry.\n";
            break;

        default:
            cout << "\nInvalid choice. Try again.\n";
        }
    }
}
