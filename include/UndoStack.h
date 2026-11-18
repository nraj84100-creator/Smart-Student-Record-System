#ifndef UNDO_STACK_H
#define UNDO_STACK_H

#include "Student.h"

enum class ActionType {
    ADD,
    UPDATE,
    DELETE
};

struct UndoAction {
    ActionType type;
    Student student;
};

struct UndoNode {
    UndoAction action;
    UndoNode* next;

    explicit UndoNode(const UndoAction& value)
        : action(value), next(nullptr) {}
};

class UndoStack {
private:
    UndoNode* topNode;

public:
    UndoStack();
    ~UndoStack();

    UndoStack(const UndoStack&) = delete;
    UndoStack& operator=(const UndoStack&) = delete;

    void push(const UndoAction& action);
    const UndoAction* peek() const;
    void pop();
    bool isEmpty() const;
    void clear();
};

#endif
