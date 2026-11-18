#include "BST.h"

BST::BST() : root(nullptr) {}

BST::~BST() {
    destroyTree(root);
}

void BST::destroyTree(BSTNode* node) {
    if (node == nullptr) {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

BSTNode* BST::insertNode(
    BSTNode* node, int id, Student* student
) {
    if (node == nullptr) {
        return new BSTNode(id, student);
    }

    if (id < node->id) {
        node->left = insertNode(node->left, id, student);
    } else if (id > node->id) {
        node->right = insertNode(node->right, id, student);
    }

    return node;
}

bool BST::insert(int id, Student* student) {
    if (student == nullptr || student->id != id) {
        return false;
    }

    if (search(id) != nullptr) {
        return false;
    }

    root = insertNode(root, id, student);
    return true;
}

const Student* BST::searchNode(
    const BSTNode* node, int id
) const {
    if (node == nullptr) {
        return nullptr;
    }

    if (id == node->id) {
        return node->student;
    }

    if (id < node->id) {
        return searchNode(node->left, id);
    }

    return searchNode(node->right, id);
}

const Student* BST::search(int id) const {
    return searchNode(root, id);
}

BSTNode* BST::findMin(BSTNode* node) const {
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }

    return node;
}

BSTNode* BST::deleteNode(BSTNode* node, int id) {
    if (node == nullptr) {
        return nullptr;
    }

    if (id < node->id) {
        node->left = deleteNode(node->left, id);
    } else if (id > node->id) {
        node->right = deleteNode(node->right, id);
    } else {
        if (node->left == nullptr) {
            BSTNode* rightChild = node->right;
            delete node;
            return rightChild;
        }

        if (node->right == nullptr) {
            BSTNode* leftChild = node->left;
            delete node;
            return leftChild;
        }

        BSTNode* successor = findMin(node->right);
        node->id = successor->id;
        node->student = successor->student;
        node->right = deleteNode(node->right, successor->id);
    }

    return node;
}

bool BST::remove(int id) {
    if (search(id) == nullptr) {
        return false;
    }

    root = deleteNode(root, id);
    return true;
}
