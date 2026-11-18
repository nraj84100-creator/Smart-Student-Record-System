#include "UndoStack.h"

UndoStack::UndoStack() : topNode(nullptr) {}

UndoStack::~UndoStack() {
    clear();
}

void UndoStack::push(const UndoAction& action) {
    UndoNode* newNode = new UndoNode(action);
    newNode->next = topNode;
    topNode = newNode;
}

const UndoAction* UndoStack::peek() const {
    if (topNode == nullptr) {
        return nullptr;
    }

    return &topNode->action;
}

void UndoStack::pop() {
    if (topNode == nullptr) {
        return;
    }

    UndoNode* oldTop = topNode;
    topNode = topNode->next;
    delete oldTop;
}

bool UndoStack::isEmpty() const {
    return topNode == nullptr;
}

void UndoStack::clear() {
    while (!isEmpty()) {
        pop();
    }
}
