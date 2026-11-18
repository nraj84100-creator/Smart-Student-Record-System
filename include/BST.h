#ifndef BST_H
#define BST_H

#include "Student.h"

struct BSTNode {
    int id;
    Student* student;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int studentId, Student* record)
        : id(studentId), student(record),
          left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;

    BSTNode* insertNode(BSTNode* node, int id, Student* student);
    const Student* searchNode(const BSTNode* node, int id) const;
    BSTNode* deleteNode(BSTNode* node, int id);
    BSTNode* findMin(BSTNode* node) const;
    void destroyTree(BSTNode* node);

public:
    BST();
    ~BST();

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    bool insert(int id, Student* student);
    const Student* search(int id) const;
    bool remove(int id);
};

#endif
