#pragma once

#include <nanodbc/nanodbc.h>
#include "models/student.hpp"
#include "models/subject.hpp"
#include "models/grade.hpp"
#include "models/gradeEntry.hpp"
#include "responses/gradesByClassNumber.hpp"
#include <string>

struct RemedialExamStudent
{
    Student student;
    std::string subject_name;
    float average_grade;
};

class Database
{
public:
    Database();
    ~Database();

    bool connect(const nanodbc::connection &conn_string);
    void disconnect();
    bool create_tables();
    bool execute(const std::string &query);
    bool add_student(const Student &student);
    std::vector<Student> get_students();
    bool update_student(int id, const int &number_in_class, const std::string &name, const std::string &email);
    bool add_subject(const Subject &subject);
    std::vector<Subject> get_subjects();
    bool update_subject(int id, const std::string &name, const std::string &teacher, const std::string &room_number);
    bool delete_subject(int id);
    bool delete_student(int id);
    bool add_grade(const Grade &grade);
    std::vector<GradeByClassNumberEntity> get_grades_by_class_number(int class_number);
    bool update_grade(int id, float grade_value);
    bool delete_grade(int id);
    float get_subject_average(int class_number, const std::string &subject_name);
    float get_overall_average(int class_number);

    std::vector<Student> get_top_students();
    std::vector<RemedialExamStudent> get_remedial_exam_students();
    std::vector<Student> get_failing_students();
    std::vector<Student> get_birthday_students();

private:
    nanodbc::connection *conn;
    bool connected;
};
