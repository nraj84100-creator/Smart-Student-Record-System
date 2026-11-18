#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Student.h"
#include <vector>

struct Node {
    Student data;
    Node* next;

    explicit Node(const Student& student)
        : data(student), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList();
    ~LinkedList();

    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    Student* insertStudent(const Student& student);
    bool updateStudent(const Student& student);
    bool deleteStudent(int id);

    void displayAll() const;
    const Student* searchStudent(int id) const;
    std::vector<Student> getAllStudents() const;
    bool isEmpty() const;
};

#endif
