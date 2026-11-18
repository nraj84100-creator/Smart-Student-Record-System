#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "LinkedList.h"
#include "BST.h"
#include "UndoStack.h"

class StudentManager {
private:
    LinkedList studentList;
    BST studentIndex;
    UndoStack history;

public:
    bool addStudent(
        const Student& student,
        bool recordHistory = true
    );
    bool updateStudent(
        const Student& student,
        bool recordHistory = true
    );
    bool deleteStudent(
        int id,
        bool recordHistory = true
    );

    bool undoLastChange();
    bool canUndo() const;
    bool validateConsistency() const;

    const Student* searchStudent(int id) const;
    std::vector<Student> getAllStudents() const;
    void displayAll() const;
};

#endif
