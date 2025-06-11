#ifndef GRADE_HPP
#define GRADE_HPP

#include <string>

struct Grade {
    int studentId;
    int subjectId;
    std::string date;
    float gradeValue;
};

#endif