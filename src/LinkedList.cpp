#include "LinkedList.h"

#include <iostream>

using namespace std;

LinkedList::LinkedList() : head(nullptr) {}

LinkedList::~LinkedList() {
    Node* current = head;

    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head = nullptr;
}

bool LinkedList::isEmpty() const {
    return head == nullptr;
}

const Student* LinkedList::searchStudent(int id) const {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.id == id) {
            return &current->data;
        }

        current = current->next;
    }

    return nullptr;
}

Student* LinkedList::insertStudent(const Student& student) {
    if (searchStudent(student.id) != nullptr) {
        return nullptr;
    }

    Node* newNode = new Node(student);

    if (head == nullptr) {
        head = newNode;
        return &newNode->data;
    }

    Node* current = head;

    while (current->next != nullptr) {
        current = current->next;
    }

    current->next = newNode;
    return &newNode->data;
}

bool LinkedList::updateStudent(const Student& student) {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.id == student.id) {
            current->data = student;
            return true;
        }

        current = current->next;
    }

    return false;
}

bool LinkedList::deleteStudent(int id) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->data.id == id) {
            if (previous == nullptr) {
                head = current->next;
            } else {
                previous->next = current->next;
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

void LinkedList::displayAll() const {
    if (isEmpty()) {
        cout << "\nNo student records available.\n";
        return;
    }

    Node* current = head;
    int count = 1;

    while (current != nullptr) {
        cout << "\nStudent Record #" << count << '\n';
        displayStudent(current->data);

        current = current->next;
        count++;
    }

    cout << "\nTotal Students: " << count - 1 << '\n';
}

vector<Student> LinkedList::getAllStudents() const {
    vector<Student> students;
    Node* current = head;

    while (current != nullptr) {
        students.push_back(current->data);
        current = current->next;
    }

    return students;
}
