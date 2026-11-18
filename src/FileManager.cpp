#include "FileManager.h"

#include <cmath>
#include <cerrno>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#include <direct.h>
#include <sys/stat.h>
#else
#include <sys/stat.h>
#endif

using namespace std;

namespace {
const string HEADER =
    "ID,Name,Department,Semester,Mark1,Mark2,Mark3,Attendance";

bool pathExists(const string& path) {
#ifdef _WIN32
    struct _stat info {};
    return _stat(path.c_str(), &info) == 0;
#else
    struct stat info {};
    return stat(path.c_str(), &info) == 0;
#endif
}

bool ensureDirectory(const string& path) {
    if (path.empty() || pathExists(path)) {
        return true;
    }

#ifdef _WIN32
    return _mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

string parentDirectory(const string& path) {
    size_t separator = path.find_last_of("/\\");
    return separator == string::npos ? string() : path.substr(0, separator);
}

void renameFile(const string& from, const string& to) {
    if (rename(from.c_str(), to.c_str()) != 0) {
        throw runtime_error("could not rename data file");
    }
}

string escapeCSV(const string& value) {
    if (value.find_first_of(",\"\r\n") == string::npos) {
        return value;
    }

    string result = "\"";

    for (char c : value) {
        result += c == '"' ? "\"\"" : string(1, c);
    }

    return result + '"';
}

bool parseCSV(const string& line, vector<string>& fields) {
    fields.clear();
    string field;
    bool inQuotes = false;
    bool quoted = false;
    bool closedQuote = false;

    for (char c : line) {
        if (inQuotes) {
            if (c == '"') {
                inQuotes = false;
                closedQuote = true;
            } else {
                field += c;
            }
        } else if (c == ',') {
            fields.push_back(field);
            field.clear();
            quoted = false;
            closedQuote = false;
        } else if (c == '"' && !quoted &&
                   field.empty() && !closedQuote) {
            inQuotes = true;
            quoted = true;
        } else if (c == '"' && closedQuote) {
            field += '"';
            inQuotes = true;
            closedQuote = false;
        } else if (closedQuote || c == '"') {
            return false;
        } else {
            field += c;
        }
    }

    if (inQuotes) {
        return false;
    }

    fields.push_back(field);
    return true;
}

bool parseInt(const string& text, int& value) {
    try {
        size_t position = 0;
        int result = stoi(text, &position);

        if (position != text.size()) {
            return false;
        }

        value = result;
        return true;
    } catch (const exception&) {
        return false;
    }
}

bool parseDouble(const string& text, double& value) {
    try {
        size_t position = 0;
        double result = stod(text, &position);

        if (position != text.size() || !isfinite(result)) {
            return false;
        }

        value = result;
        return true;
    } catch (const exception&) {
        return false;
    }
}

bool validText(const string& value) {
    return value.find_first_not_of(" \t\r\n") != string::npos;
}

bool parseStudent(const vector<string>& fields, Student& student) {
    if (fields.size() != 8) {
        return false;
    }

    if (!parseInt(fields[0], student.id) ||
        student.id <= 0 ||
        !parseInt(fields[3], student.semester) ||
        student.semester < 1 ||
        student.semester > 8) {
        return false;
    }

    student.name = fields[1];
    student.department = fields[2];

    if (!validText(student.name) || !validText(student.department)) {
        return false;
    }

    for (int i = 0; i < 3; i++) {
        if (!parseDouble(fields[4 + i], student.marks[i]) ||
            student.marks[i] < 0 ||
            student.marks[i] > 100) {
            return false;
        }
    }

    return parseDouble(fields[7], student.attendance) &&
           student.attendance >= 0 &&
           student.attendance <= 100;
}
}

bool FileManager::saveStudents(
    const StudentManager& manager,
    const string& filename
) {
    try {
        const string temporary = filename + ".tmp";
        const string backup = filename + ".bak";

        if (!ensureDirectory(parentDirectory(filename))) {
            cerr << "Cannot create data directory.\n";
            return false;
        }

        if (pathExists(backup)) {
            cerr << "Backup file exists: " << backup
                 << "\nCheck it before saving.\n";
            return false;
        }

        ofstream file(temporary, ios::out | ios::trunc);

        if (!file.is_open()) {
            cerr << "Cannot open temporary data file.\n";
            return false;
        }

        file << HEADER << '\n';

        for (const Student& student : manager.getAllStudents()) {
            file << student.id << ','
                 << escapeCSV(student.name) << ','
                 << escapeCSV(student.department) << ','
                 << student.semester;

            for (double mark : student.marks) {
                file << ',' << mark;
            }

            file << ',' << student.attendance << '\n';
        }

        file.flush();

        if (!file.good()) {
            cerr << "Error writing student records.\n";
            return false;
        }

        file.close();

        if (file.fail()) {
            cerr << "Error closing temporary file.\n";
            return false;
        }

        bool hadOriginal = pathExists(filename);

        if (hadOriginal) {
            renameFile(filename, backup);
        }

        try {
            renameFile(temporary, filename);
        } catch (const std::exception&) {
            if (hadOriginal && pathExists(backup) &&
                !pathExists(filename)) {
                renameFile(backup, filename);
            }
            throw;
        }

        if (hadOriginal) {
            remove(backup.c_str());
        }

        return true;
    } catch (const exception& error) {
        cerr << "Save failed: " << error.what() << '\n';
        return false;
    }
}

bool FileManager::loadStudents(
    StudentManager& manager,
    const string& filename
) {
    try {
        const string backup = filename + ".bak";

        if (pathExists(backup)) {
            cerr << "\nWARNING: Recovery backup detected!\n";
            cerr << "Backup file: " << backup << '\n';
            cerr << "Do not overwrite your saved records.\n";
            cerr << "Inspect the backup and restore it manually "
                 << "before continuing.\n";
            return false;
        }

        if (!pathExists(filename)) {
            cout << "No saved records found."
                 << " Starting with an empty system.\n";
            return true;
        }

        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "Cannot open student data file.\n";
            return false;
        }

        string line;

        if (!getline(file, line)) {
            cerr << "Student file has no header.\n";
            return false;
        }

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line != HEADER) {
            cerr << "Invalid CSV header.\n";
            return false;
        }

        int rowNumber = 1;
        int loaded = 0;

        while (getline(file, line)) {
            rowNumber++;

            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            vector<string> fields;
            Student student{};

            if (!parseCSV(line, fields) ||
                !parseStudent(fields, student)) {
                cerr << "Invalid data on CSV row "
                     << rowNumber << '\n';
                return false;
            }

            if (!manager.addStudent(student, false)) {
                cerr << "Duplicate ID or insertion failure"
                     << " on row " << rowNumber << '\n';
                return false;
            }

            loaded++;
        }

        if (file.bad()) {
            cerr << "Error while reading student data.\n";
            return false;
        }

        cout << loaded << " student record(s) loaded.\n";
        return true;
    } catch (const exception& error) {
        cerr << "Load failed: " << error.what() << '\n';
        return false;
    }
}
