#include <iostream>
#include "Database.hpp"
#include "models/student.hpp"
#include "models/subject.hpp"
#include <sstream>

Database::Database() : conn(nullptr), connected(false)
{
}

Database::~Database()
{
    disconnect();
}

bool Database::connect(const nanodbc::connection &conn_string)
{
    try
    {
        conn = new nanodbc::connection(conn_string);
        connected = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to connect to database: " << e.what() << std::endl;
        return false;
    }
}

void Database::disconnect()
{
    if (conn)
    {
        delete conn;
        conn = nullptr;
    }
    connected = false;
}

bool Database::execute(const std::string &query)
{
    if (!connected)
    {
        std::cerr << "Not connected to database" << std::endl;
        return false;
    }
    try
    {
        nanodbc::execute(*conn, query);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Query failed: " << e.what() << std::endl;
        return false;
    }
}

bool Database::create_tables()
{
    if (!connected)
    {
        std::cerr << "Not connected to the database." << std::endl;
        return false;
    }

    try
    {
        execute(R"(
            IF OBJECT_ID('students', 'U') IS NULL
            CREATE TABLE students (
                id INT IDENTITY(1,1) PRIMARY KEY,
                number_in_class INT NOT NULL UNIQUE,
                full_name NVARCHAR(100) NOT NULL,
                date_of_birth DATE NOT NULL
            )
        )");

        execute(R"(
            IF OBJECT_ID('subjects', 'U') IS NULL
            CREATE TABLE subjects (
                id INT IDENTITY(1,1) PRIMARY KEY,
                name NVARCHAR(100) NOT NULL UNIQUE,
                teacher NVARCHAR(100) NOT NULL,
                room_number NVARCHAR(50) NOT NULL
            )
        )");

        execute(R"(
            IF OBJECT_ID('grades', 'U') IS NULL
            CREATE TABLE grades (
                id INT IDENTITY(1,1) PRIMARY KEY,
                student_id INT NOT NULL,
                subject_id INT NOT NULL,
                date DATE NOT NULL,
                grade_value FLOAT NOT NULL,
                FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
                FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE
            )
        )");

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to create tables: " << e.what() << std::endl;
        return false;
    }
}

bool Database::add_student(const Student &student)
{
    std::string query =
        "INSERT INTO students (number_in_class, full_name, date_of_birth) "
        "VALUES (" +
        std::to_string(student.number_in_class) + ", '" +
        student.full_name + "', '" + student.date_of_birth + "')";

    return execute(query);
}

std::vector<Student> Database::get_students()
{
    std::vector<Student> students;

    if (!connected)
    {
        std::cerr << "Not connected to database" << std::endl;
        return students;
    }

    try
    {
        nanodbc::result result = nanodbc::execute(*conn, "SELECT id, number_in_class, full_name, date_of_birth FROM students");

        while (result.next())
        {
            Student s;
            s.id = result.get<int>(0);
            s.number_in_class = result.get<int>(1);
            s.full_name = result.get<std::string>(2);
            s.date_of_birth = result.get<std::string>(3);

            students.push_back(std::move(s));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to get students: " << e.what() << std::endl;
    }

    return students;
}

bool Database::update_student(int id, const int &number_in_class, const std::string &name, const std::string &email)
{
}

bool Database::delete_student(int id)
{
}

bool Database::add_subject(const Subject &subject)
{
    std::string query =
        "INSERT INTO subjects (name, teacher, room_number) "
        "VALUES ('" +
        subject.name + "', '" +
        subject.teacher + "', '" + subject.room_number + "')";
    std::cout << query << std::endl;
    return execute(query);
}

std::vector<Subject> Database::get_subjects()
{
    std::vector<Subject> subjects;

    if (!connected)
    {
        std::cerr << "Not connected to database" << std::endl;
        return subjects;
    }

    try
    {
        nanodbc::result result = nanodbc::execute(*conn, "SELECT id, name, teacher, room_number FROM subjects");

        while (result.next())
        {
            Subject subj;
            subj.id = result.get<int>("id");
            subj.name = result.get<std::string>("name");
            subj.teacher = result.get<std::string>("teacher");
            subj.room_number = result.get<std::string>("room_number");
            subjects.push_back(subj);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Query failed: " << e.what() << std::endl;
    }

    return subjects;
}

bool Database::add_grade(const Grade &grade)
{
    std::ostringstream query;
    query << "INSERT INTO grades (student_id, subject_id, date, grade_value) VALUES ("
          << grade.student_id << ", "
          << grade.subject_id << ", '"
          << grade.date << "', "
          << grade.grade_value << ");";

    return execute(query.str());
}

std::vector<Grade> Database::get_grades()
{
    std::vector<Grade> grades;

    if (!connected)
    {
        std::cerr << "Not connected to database" << std::endl;
        return grades;
    }

    try
    {
        nanodbc::result result = nanodbc::execute(*conn, "SELECT student_id, subject_id, date, grade_value FROM grades");

        while (result.next())
        {
            Grade g;
            g.student_id = result.get<int>("student_id");
            g.subject_id = result.get<int>("subject_id");
            g.date = result.get<std::string>("date");
            g.grade_value = result.get<float>("grade_value");
            std:: cout << "student_id: " << g.student_id << ", grade: " << g.grade_value << ", time: " << g.date << std::endl;

            grades.push_back(g);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Query failed: " << e.what() << std::endl;
    }

    return grades;
}
