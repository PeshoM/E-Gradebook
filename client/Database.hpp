#pragma once

#include <nanodbc/nanodbc.h>
#include <string>

class Database
{
public:
    Database();
    ~Database();

    bool connect(const nanodbc::connection &conn_string);
    void disconnect();
    bool create_tables();
    bool execute(const std::string &query);
    bool add_student(const int &number_in_class, const std::string &name, const std::string &date_of_birth);
    bool get_students();
    bool update_student(int id, const int &number_in_class, const std::string &name, const std::string &email);
    bool delete_student(int id);

private:
    nanodbc::connection *conn;
    bool connected;
};
