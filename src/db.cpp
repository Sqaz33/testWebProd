#include "include/db.h"

#include <vector>
#include <iterator>
#include <exception>

#include "pqxx/pqxx"

#include "include/user.h"

void sql_tools::replace(std::string& str, const std::string& src, const std::string& dst) {
    for (size_t pos = str.find(src); pos != str.npos; pos = str.find(src, pos + dst.size())) {
        str.replace(pos, 1,  dst);
    }
}

using db::DB;

std::vector<user::User> DB::getUsers() {
    std::vector<user::User> users;
    try {
        pqxx::work tx(con);
        auto res = tx.stream<std::string, std::string>
            ("select \"login\", \"password\" from \"Users\" ");
        for (auto tpl : res) {
            users.emplace_back(tpl);
        }
    } 
    #ifdef DEBUG
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
        users.clear();
        throw;
    }
    #else
    catch (...) {users.clear();}
    #endif

    return users;
}

void DB::insertUserIfNotExists(const user::User& u) {
    try {
        std::string lg = u.login();
        sql_tools::replace(lg, "'", "''");
        std::string pw = u.password();
        sql_tools::replace(pw, "'", "''");

        pqxx::work tx(con);
        tx.exec0(
            "call insert_user_if_not_exists('" + lg + "', '" + pw + "')"
        );
        tx.commit();
    }
    #ifdef DEBUG
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
        throw;
    }
    #else
    catch (...) {}
    #endif
}

bool DB::isUserExists(const user::User& u) {
    bool isExists = false;
    try {
        std::string lg = u.login();
        sql_tools::replace(lg, "'", "''");
        std::string pw = u.password();
        sql_tools::replace(pw, "'", "''");

        pqxx::work tx(con);
        isExists = tx.query_value<bool>(
            "select is_user_exists('" + lg + "', '" + pw + "')"
        );
    }
    #ifdef DEBUG
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
        throw;
    }
    #else
    catch (...) {}
    #endif
    return isExists;
}