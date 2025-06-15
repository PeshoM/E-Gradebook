#pragma once

#include <string>
#include <map>

enum class WindowType
{
    None,
    Menu,
    AddStudent,
    AddSubject,
    AddGrade,
    ViewStudents,
    ViewSubjects,
    ViewGrades,
    TopStudents,
    RemedialStudents,
    FailingStudents,
    BirthdayStudents
};

inline const std::map<WindowType, std::string> windowTypeToLabel = {
    {WindowType::Menu, "Menu"},
    {WindowType::AddStudent, "Add Student"},
    {WindowType::AddSubject, "Add Subject"},
    {WindowType::AddGrade, "Add Grade"},
    {WindowType::ViewStudents, "View Students"},
    {WindowType::ViewSubjects, "View Subjects"},
    {WindowType::ViewGrades, "View Grades"},
    {WindowType::TopStudents, "Top Students"},
    {WindowType::RemedialStudents, "Remedial Students"},
    {WindowType::FailingStudents, "Failing Students"},
    {WindowType::BirthdayStudents, "Birthday Students"}
};