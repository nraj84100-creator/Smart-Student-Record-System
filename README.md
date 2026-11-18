# Smart Student Record Management System

A C++17 mini project for managing student academic records using a singly
linked list, binary search tree, and custom undo stack.

## Features

1. Add Student
2. View All Students
3. Search Student by ID
4. Update Student
5. Delete Student
6. Sort Students
7. Academic Performance Report
8. Attendance Alerts
9. Undo Last Change

## Data Structures

- **Singly Linked List:** Dynamic storage of student records.
- **Binary Search Tree:** Student ID indexing and searching.
- **Stack:** Undo history for add, update, and delete operations.

## Technologies

- C++17
- GCC / G++
- Visual Studio Code
- CSV file handling

## Compilation

From the project root:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Student.cpp src/LinkedList.cpp src/BST.cpp src/StudentManager.cpp src/FileManager.cpp src/Analytics.cpp src/UndoStack.cpp -o student_system.exe
```

Run on Windows PowerShell:

```powershell
.\student_system.exe
```

## Testing

Compile and run the integration tests:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -O0 -g -Iinclude -o integration_tests.exe tests/integration_tests.cpp src/Student.cpp src/LinkedList.cpp src/BST.cpp src/StudentManager.cpp src/FileManager.cpp src/Analytics.cpp src/UndoStack.cpp
.\integration_tests.exe
```

The tests cover CRUD operations, BST deletion cases, undo operations,
academic analytics, CSV persistence, consistency checking, and repeated
operations over 100 records.

## Data Storage

Records are saved in `data/students.csv` and loaded automatically at startup.
Undo history exists only during the current program session. A leftover
`.bak` recovery file causes startup to stop so that saved data can be reviewed
manually.

## Academic Policies

The demonstration grading system uses three subjects, a minimum mark of 40
per subject, and a default attendance alert threshold of 75%. These are
configurable project assumptions, not official university academic rules.

## Limitations and Future Enhancements

- Console-based, single-user application
- Unbalanced BST
- Non-persistent undo history
- Three subjects per student
- No authentication or simultaneous multi-user editing
- Future options include a GUI, AVL tree, database integration, authentication,
  variable subject counts, and advanced analytics

## Academic Purpose

Developed as a Data Structures using C++ mini project for Second Year B.Tech,
Semester III, Ajeenkya D Y Patil University, Pune.
