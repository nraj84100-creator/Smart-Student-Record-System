
#ifndef STUDENT_H
#define STUDENT_H

#include <array>
#include <string>

struct Student {
    int id;
    std::string name;
    std::string department;
    int semester;
    std::array<double, 3> marks;
    double attendance;
};

Student inputStudent();
void displayStudent(const Student& student);
double calculatePercentage(const Student& student);

#endif
