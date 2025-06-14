#include <iostream>
#include "Database.hpp"
#include "models/student.hpp"
#include "models/subject.hpp"

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
        std::cerr << "Query failed: 123" << e.what() << std::endl;
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
        nanodbc::result result = nanodbc::execute(*conn, "SELECT number_in_class, full_name, date_of_birth FROM students");

        while (result.next())
        {
            Student s;
            s.number_in_class = result.get<int>(0);
            s.full_name = result.get<std::string>(1);
            s.date_of_birth = result.get<std::string>(2);

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
        nanodbc::result result = nanodbc::execute(*conn, "SELECT name, teacher, room_number FROM subjects");

        while (result.next())
        {
            Subject subj;
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

bool Database::delete_student(int id)
{
}