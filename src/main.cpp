#include <iostream>
#include <vector>

#include "crow.h"

#include "windows.h"

#include "include/db.h"
#include "include/user.h"

static bool userVerify(crow::request req, db::DB& db) {
    std::string auth = req.get_header_value("Authorization");
    if (auth.empty()) {
        return false;
    }

    std::string creds = auth.substr(6);
    std::string d_creds = crow::utility::base64decode(creds, creds.size()); 
    size_t f = d_creds.find(':');

    if (f == std::string::npos) {
        return false;
    }

    std::string uname = d_creds.substr(0, f);
    std::string pword = d_creds.substr(f + 1);
    auto u = user::User(uname, pword);
    return db.isUserExists(u);
}


int main() {
    SetConsoleOutputCP(CP_UTF8); 
    
    db::DB db("dbname=cpptest user=postgres password=1568");

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([&db](const crow::request& req) {
        std::string aut1 = req.get_header_value("Authorization");
        if (!aut1.empty()) {
            std::string creds = aut1.substr(6);
            std::string d_creds = crow::utility::base64decode(creds, creds.size()); 
        }


        bool auth = userVerify(req, db);
        if (!auth) {
            crow::response res;
            res.code = 401;
            res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
            res.write("<html><body><h1>Authentication Required</h1></body></html>");
            return res;
        }
        return crow::response("<html><body><h1>Welcome!</h1><p>You are successfully authenticated.</p></body></html>");
    }); 

    for (const auto& u : db.getUsers()) {
        std::cout << u << '\n';
    }

    app
    .bindaddr("192.168.0.102")
    .port(8001)
    .multithreaded()
    .run();

    std::cout << db.isUserExists(user::User("", "asdfasdf"));
}
