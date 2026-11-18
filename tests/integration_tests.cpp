#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "StudentManager.h"
#include "FileManager.h"
#include "Analytics.h"

using namespace std;

Student makeStudent(
    int id,
    const string& name,
    double m1,
    double m2,
    double m3,
    double attendance
) {
    return Student{
        id,
        name,
        "Computer Engineering",
        3,
        {m1, m2, m3},
        attendance
    };
}

void testCRUD() {
    StudentManager manager;

    Student rahul = makeStudent(
        1001, "Rahul", 78, 85, 91, 82.5
    );
    Student priya = makeStudent(
        1002, "Priya", 90, 88, 94, 92
    );

    assert(manager.addStudent(rahul));
    assert(manager.addStudent(priya));
    assert(!manager.addStudent(rahul));
    assert(manager.searchStudent(1001) != nullptr);
    assert(manager.searchStudent(9999) == nullptr);

    priya.attendance = 95;
    assert(manager.updateStudent(priya));
    assert(manager.searchStudent(1002)->attendance == 95);

    assert(manager.deleteStudent(1001));
    assert(manager.searchStudent(1001) == nullptr);
    assert(manager.validateConsistency());

    cout << "[PASS] CRUD operations\n";
}

void testUndo() {
    StudentManager manager;
    Student student = makeStudent(
        2001, "Aman", 70, 80, 90, 75
    );

    assert(manager.addStudent(student));
    assert(manager.undoLastChange());
    assert(manager.searchStudent(2001) == nullptr);

    assert(manager.addStudent(student));
    Student updated = student;
    updated.attendance = 95;
    assert(manager.updateStudent(updated));
    assert(manager.undoLastChange());
    assert(manager.searchStudent(2001)->attendance == 75);

    assert(manager.deleteStudent(2001));
    assert(manager.searchStudent(2001) == nullptr);
    assert(manager.undoLastChange());
    assert(manager.searchStudent(2001) != nullptr);
    assert(manager.validateConsistency());

    cout << "[PASS] Undo operations\n";
}

void testAnalytics() {
    Student student = makeStudent(
        3001, "Sneha", 95, 92, 97, 96
    );

    assert(Analytics::hasPassed(student));
    assert(Analytics::calculateGrade(student) == "O");
    assert(fabs(calculatePercentage(student) - 94.6666667) < 0.001);

    Student failed = makeStudent(
        3002, "Rohan", 35, 55, 60, 72
    );

    assert(!Analytics::hasPassed(failed));
    assert(Analytics::calculateGrade(failed) == "F");

    cout << "[PASS] Academic calculations\n";
}

void testFilePersistence() {
    StudentManager firstManager;
    Student student = makeStudent(
        4001, "Test Student", 80, 85, 90, 88
    );
    const string filename = "tests/integration_students.csv";

    assert(firstManager.addStudent(student));
    assert(FileManager::saveStudents(firstManager, filename));

    StudentManager secondManager;
    assert(FileManager::loadStudents(secondManager, filename));

    const Student* loaded = secondManager.searchStudent(4001);
    assert(loaded != nullptr);
    assert(loaded->name == "Test Student");
    assert(loaded->attendance == 88);
    assert(!secondManager.canUndo());

    remove(filename.c_str());
    cout << "[PASS] File persistence\n";
}

void writeFile(const string& filename, const string& contents) {
    ofstream file(filename);
    assert(file.is_open());
    file << contents;
}

void testNegativeFileHandling() {
    const string filename = "tests/negative_students.csv";
    const string backup = filename + ".bak";

    remove(filename.c_str());
    remove(backup.c_str());

    StudentManager missingManager;
    assert(FileManager::loadStudents(missingManager, filename));
    assert(missingManager.getAllStudents().empty());

    writeFile(filename,
              "ID,Name,Department,Semester,Mark1,Mark2,Mark3,Attendance\n"
              "1,Invalid,IT,3,101,80,80,90\n");
    StudentManager invalidMarksManager;
    assert(!FileManager::loadStudents(
        invalidMarksManager, filename
    ));

    writeFile(filename,
              "ID,Name,Department,Semester,Mark1,Mark2,Mark3,Attendance\n"
              "1,One,IT,3,80,80,80,90\n"
              "1,Duplicate,IT,3,80,80,80,90\n");
    StudentManager duplicateManager;
    assert(!FileManager::loadStudents(
        duplicateManager, filename
    ));

    writeFile(filename,
              "Wrong,Name,Department,Semester,Mark1,Mark2,Mark3,Attendance\n");
    StudentManager headerManager;
    assert(!FileManager::loadStudents(headerManager, filename));

    writeFile(filename,
              "ID,Name,Department,Semester,Mark1,Mark2,Mark3,Attendance\n"
              "1,\"Unfinished,IT,3,80,80,80,90\n");
    StudentManager quotedManager;
    assert(!FileManager::loadStudents(quotedManager, filename));

    remove(filename.c_str());
    writeFile(backup,
              "ID,Name,Department,Semester,Mark1,Mark2,Mark3,Attendance\n");
    StudentManager recoveryManager;
    assert(!FileManager::loadStudents(recoveryManager, filename));

    StudentManager inaccessibleManager;
    assert(!FileManager::saveStudents(
        inaccessibleManager,
        "Z:\\no_such_directory\\students.csv"
    ));

    remove(filename.c_str());
    remove(backup.c_str());
    remove((filename + ".tmp").c_str());

    cout << "[PASS] Negative file handling\n";
}

void testBSTDeletion() {
    StudentManager manager;
    int ids[] = {
        1050, 1025, 1075, 1010, 1040, 1060, 1090
    };

    for (int id : ids) {
        assert(manager.addStudent(
            makeStudent(
                id, "Student " + to_string(id),
                75, 80, 85, 90
            )
        ));
    }

    assert(manager.deleteStudent(1010));
    assert(manager.deleteStudent(1025));
    assert(manager.deleteStudent(1050));

    assert(manager.searchStudent(1010) == nullptr);
    assert(manager.searchStudent(1025) == nullptr);
    assert(manager.searchStudent(1050) == nullptr);
    assert(manager.searchStudent(1040) != nullptr);
    assert(manager.searchStudent(1060) != nullptr);
    assert(manager.searchStudent(1075) != nullptr);
    assert(manager.searchStudent(1090) != nullptr);
    assert(manager.validateConsistency());

    cout << "[PASS] BST deletion cases\n";
}

void testRepeatedOperations() {
    StudentManager manager;

    for (int i = 1; i <= 100; i++) {
        assert(manager.addStudent(
            makeStudent(
                5000 + i,
                "Student " + to_string(i),
                70, 80, 90, 85
            )
        ));
    }

    assert(manager.getAllStudents().size() == 100);
    assert(manager.validateConsistency());

    for (int i = 1; i <= 50; i++) {
        assert(manager.deleteStudent(5000 + i));
    }

    assert(manager.getAllStudents().size() == 50);
    assert(manager.validateConsistency());

    for (int i = 1; i <= 50; i++) {
        assert(manager.undoLastChange());
    }

    assert(manager.getAllStudents().size() == 100);
    assert(manager.validateConsistency());

    for (int i = 1; i <= 100; i++) {
        assert(manager.searchStudent(5000 + i) != nullptr);
    }

    cout << "[PASS] Repeated operations\n";
}

int main() {
    cout << "\n===== INTEGRATION TESTS =====\n\n";

    testCRUD();
    testUndo();
    testAnalytics();
    testFilePersistence();
    testNegativeFileHandling();
    testBSTDeletion();
    testRepeatedOperations();

    cout << "\nAll integration tests passed!\n";
    return 0;
}
