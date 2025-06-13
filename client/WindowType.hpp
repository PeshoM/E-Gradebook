#pragma once

#include <string>
#include <map>

enum class WindowType
{
    None,
    Menu,
    AddStudent,
    EditStudent,
    DeleteStudent,
    AddSubject,
    EditSubject,
    DeleteSubject,
    AddGrade,
    EditGrade,
    DeleteGrade,
    ViewStudents,
    ViewSubjects,
    ViewGrades,
    AvgGradeSubject,
    AvgGradeOverall,
    TopStudents,
    RemedialStudents,
    FailingStudents,
    BirthdayStudents
};

inline const std::map<WindowType, std::string> windowTypeToLabel = {
    {WindowType::Menu, "Menu"},
    {WindowType::AddStudent, "Add Student"},
    {WindowType::EditStudent, "Edit Student"},
    {WindowType::DeleteStudent, "Delete Student"},
    {WindowType::AddSubject, "Add Subject"},
    {WindowType::EditSubject, "Edit Subject"},
    {WindowType::DeleteSubject, "Delete Subject"},
    {WindowType::AddGrade, "Add Grade"},
    {WindowType::EditGrade, "Edit Grade"},
    {WindowType::DeleteGrade, "Delete Grade"},
    {WindowType::ViewStudents, "View Students"},
    {WindowType::ViewSubjects, "View Subjects"},
    {WindowType::ViewGrades, "View Grades"},
    {WindowType::AvgGradeSubject, "Avg Grade Subject"},
    {WindowType::AvgGradeOverall, "Avg Grade Overall"},
    {WindowType::TopStudents, "Top Students"},
    {WindowType::RemedialStudents, "Remedial Students"},
    {WindowType::FailingStudents, "Failing Students"},
    {WindowType::BirthdayStudents, "Birthday Students"}
};