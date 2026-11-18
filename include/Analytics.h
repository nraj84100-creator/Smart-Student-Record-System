#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "Student.h"

#include <string>
#include <vector>

class Analytics {
public:
    static std::string calculateGrade(const Student& student);
    static bool hasPassed(const Student& student);

    static void sortStudents(
        std::vector<Student> students,
        int choice
    );

    static void performanceReport(
        const std::vector<Student>& students
    );

    static void attendanceAlerts(
        const std::vector<Student>& students,
        double threshold = 75.0
    );
};

#endif
