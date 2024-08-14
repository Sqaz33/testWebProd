#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <tuple>
#include <vector>

#define DEBUG

#include <pqxx/pqxx>

#include "windows.h"

class Employee;
std::ostream& operator<<(std::ostream& out, const Employee& emp);

class Employee {
public:
    Employee() = default;

    Employee(std::tuple<int, int> row) {
        id = std::get<0>(row);
        salary = std::get<1>(row);
    }

    Employee(const Employee& other):
        id(other.id),
        salary(other.salary)
    {}

    Employee(Employee&& other):
        id(other.id),
        salary(other.salary)
    {
        other.id = -1;
    }

    Employee& operator=(const Employee& other) {
        id = other.id;
        salary = other.salary;
        return *this;
    }

    Employee& operator=(Employee&& other) {
        id = other.id;
        salary = other.salary;
        other.id = -1;
        return *this;
    }

friend std::ostream& operator<<(std::ostream& out, const Employee& emp);

virtual ~Employee() {}

private:
    int id = -1;
    int salary;
};

std::ostream& operator<<(std::ostream& out, const Employee& emp) {
    std::cout << emp.id << "->" << emp.salary;
    return out;
}

class DB {
public:
    DB(const char* options) : con(options) {
        tx = std::make_unique<pqxx::work>(con);
    }

    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;

    DB(DB&& other) : 
        con(std::move(other.con)),
        tx(std::move(other.tx))
    {};

    DB& operator=(DB&& other) {
        con = std::move(other.con);
        tx = std::move(other.tx);
    }

public:
    std::vector<Employee> getEmployeesFromBD() {
        std::vector<Employee> emp;
        try {
            auto res = tx->stream<int, int>("select numb, salary from Employee");
            for (auto tpl : res) {
                emp.push_back(Employee(tpl));
            }
        }
        #ifdef DEBUG
        catch (const std::exception& e) {
            std::cout << e.what() << '\n';
            throw;
        }
        #else
        catch (...) { }
        #endif
        return emp;
    }

    Employee getEmployeeFromID(int ID) {
        Employee emp;
        try {
            auto q = tx->query_n<int, int>(
                        1,
                        "select numb, salary "
                        "from Employee "
                        "where numb = " + pqxx::to_string(ID)
                    );
            emp = Employee(*q.begin());
        } 
        #ifdef DEBUG
        catch (const std::exception& e) {
            std::cout << e.what() << '\n';
            throw;
        }
        #else
        catch (...) { }
        #endif
        return emp;
    }

    std::vector<std::vector<int>> getArrsFromArrTable() {
        std::vector<std::vector<int>> res;
        try {
            std::vector<int> v;
            for (auto a : 
                    tx->stream<std::string>("select arr from arr_table")) 
            {
                v.clear();
                pqxx::array<int, 2> arr(std::get<0>(a), con);
                std::copy(arr.cbegin(), arr.cend(), std::back_inserter(v));
                res.push_back(std::move(v));
            }
        } 
        #ifdef DEBUG
        catch (const std::exception& e) {
            res.clear();
            std::cout << e.what() << '\n';
            throw;
        }
        #else
        catch (...) { 
            res.clear();
        }
        #endif
        return res;
    }

private:
    pqxx::connection con;
    std::unique_ptr<pqxx::work> tx;
};



int main() {
    SetConsoleOutputCP(CP_UTF8); 
    
    DB db("dbname=cpptest user=postgres password=1568");

    // for (auto emp : db.getEmployeesFromBD()) {
    //     std::cout << emp << '\n';
    // }


    // Employee emp = db.getEmployeeFromID(1987);

    // std::cout << emp << '\n';

    auto arr = db.getArrsFromArrTable();

    for (const auto& v : arr) {
        for (const auto& n : v) {
            std::cout << n << ' ';
        }
        std::cout << '\n';
    }


}
