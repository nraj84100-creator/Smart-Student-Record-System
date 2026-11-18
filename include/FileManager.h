#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "StudentManager.h"

#include <string>

class FileManager {
public:
    static bool saveStudents(
        const StudentManager& manager,
        const std::string& filename
    );

    static bool loadStudents(
        StudentManager& manager,
        const std::string& filename
    );
};

#endif
