#ifndef SRC_INCLUDE_DB_H
#define SRC_INCLUDE_DB_H

#include <memory>
#include <vector>

#include "pqxx/pqxx"

#include "user.h"

#define DEBUG

namespace db {

class DB {
public:
    DB(const char* option) : con(option) {}

    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;

    DB(DB&& other) :
        con(std::move(other.con))
    {}
    
    DB& operator=(DB&& other) {
        con = std::move(other.con);
    }

public:
    std::vector<user::User> getUsers() ;
    void insertUserIfNotExists(const user::User& u);
    bool isUserExists(const user::User& u);

private:
    pqxx::connection con;
};

}

#endif // SRC_INCLUDE_DB_H