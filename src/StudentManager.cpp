#include "StudentManager.h"

#include <new>
#include <unordered_set>

bool StudentManager::addStudent(
    const Student& student,
    bool recordHistory
) {
    if (student.id <= 0 ||
        studentIndex.search(student.id) != nullptr) {
        return false;
    }

    Student* storedRecord = nullptr;

    try {
        storedRecord = studentList.insertStudent(student);

        if (storedRecord == nullptr) {
            return false;
        }

        if (!studentIndex.insert(student.id, storedRecord)) {
            studentList.deleteStudent(student.id);
            return false;
        }

        if (recordHistory) {
            try {
                history.push({ActionType::ADD, student});
            } catch (const std::bad_alloc&) {
                studentIndex.remove(student.id);
                studentList.deleteStudent(student.id);
                throw;
            }
        }

        return true;
    } catch (const std::bad_alloc&) {
        if (storedRecord != nullptr) {
            studentList.deleteStudent(student.id);
        }
        return false;
    }
}

bool StudentManager::updateStudent(
    const Student& student,
    bool recordHistory
) {
    const Student* existing = studentIndex.search(student.id);

    if (existing == nullptr) {
        return false;
    }

    Student previous = *existing;

    if (recordHistory) {
        try {
            history.push({ActionType::UPDATE, previous});
        } catch (const std::bad_alloc&) {
            return false;
        }
    }

    try {
        if (studentList.updateStudent(student)) {
            return true;
        }
    } catch (const std::exception&) {
        if (recordHistory) {
            history.pop();
        }
        throw;
    }

    if (recordHistory) {
        history.pop();
    }

    return false;
}

bool StudentManager::deleteStudent(int id, bool recordHistory) {
    const Student* existing = studentIndex.search(id);

    if (existing == nullptr) {
        return false;
    }

    Student previous = *existing;

    if (recordHistory) {
        try {
            history.push({ActionType::DELETE, previous});
        } catch (const std::bad_alloc&) {
            return false;
        }
    }

    if (!studentIndex.remove(id)) {
        if (recordHistory) {
            history.pop();
        }
        return false;
    }

    if (studentList.deleteStudent(id)) {
        return true;
    }

    return false;
}

bool StudentManager::undoLastChange() {
    const UndoAction* action = history.peek();

    if (action == nullptr) {
        return false;
    }

    bool success = false;

    switch (action->type) {
    case ActionType::ADD:
        success = deleteStudent(action->student.id, false);
        break;

    case ActionType::UPDATE:
        success = updateStudent(action->student, false);
        break;

    case ActionType::DELETE:
        success = addStudent(action->student, false);
        break;
    }

    if (success) {
        history.pop();
    }

    return success;
}

bool StudentManager::canUndo() const {
    return !history.isEmpty();
}

bool StudentManager::validateConsistency() const {
    std::vector<Student> students =
        studentList.getAllStudents();
    std::unordered_set<int> ids;

    for (const Student& student : students) {
        if (!ids.insert(student.id).second) {
            return false;
        }

        const Student* indexed =
            studentIndex.search(student.id);

        if (indexed == nullptr || indexed->id != student.id) {
            return false;
        }

        const Student* stored =
            studentList.searchStudent(student.id);

        if (indexed != stored) {
            return false;
        }
    }

    return true;
}

const Student* StudentManager::searchStudent(int id) const {
    return studentIndex.search(id);
}

void StudentManager::displayAll() const {
    studentList.displayAll();
}

std::vector<Student> StudentManager::getAllStudents() const {
    return studentList.getAllStudents();
}
