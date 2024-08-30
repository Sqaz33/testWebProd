#ifndef CROW_TEST_HTTP_SERVER_H
#define CROW_TEST_HTTP_SERVER_H

#include <memory>
#include <string>
#include <unordered_set>

#include "boost/asio.hpp"
#include "crow.h"

#include "db.h"
#include "user.h"


namespace crow_test_http_server {

class CrowTestHTTPServer {
public:
    CrowTestHTTPServer(
        std::shared_ptr<db::DB> db,
        const std::string& serverIP, 
        std::uint16_t serverPort
    ) : 
          db(db)
        , serverIP(serverIP)
        , serverPort(serverPort)
    {
        setupRoutes();
    }

    void serverRun() {
        app
        .bindaddr(serverIP)
        .port(serverPort)
        .multithreaded()
        .run();
    }

private:
    crow::SimpleApp app;
    std::unordered_set<unsigned> authUsers;
    std::shared_ptr<db::DB> db;
    std::string serverIP;
    std::uint16_t serverPort;

private:
    void setupRoutes() {
        CROW_ROUTE(app, "/")([]() {
            crow::response res;
            res.set_static_file_info_unsafe("D:/testWebProd/src/statics/index.html");
            return res;
        }); 

        CROW_ROUTE(app, "/reg") ([](){
            crow::response res;
            res.set_static_file_info_unsafe("D:/testWebProd/src/statics/reg.html");
            return res;
        });


        CROW_ROUTE(app, "/reg.js")([]() {
            crow::response res;
            res.set_static_file_info_unsafe("D:/testWebProd/src/statics/reg.js");
            return res;
        }); 

        CROW_ROUTE(app, "/reg")
        .methods(crow::HTTPMethod::Post)
        ([&](const crow::request& req) {
            unsigned hostIP = boost::asio::ip::make_address_v4(req.remote_ip_address).to_uint();
            bool res = registerUserByJsonPostReq(req);
            if (res) {
                authUsers.insert(hostIP);
            }
            return res ? "yes" : "no";
        }); 

        CROW_ROUTE(app, "/auth") ([](){
            crow::response res;
            res.set_static_file_info_unsafe("D:/testWebProd/src/statics/auth.html");
            return res;
        });

        CROW_ROUTE(app, "/auth.js")([]() {
            crow::response res;
            res.set_static_file_info_unsafe("D:/testWebProd/src/statics/auth.js");
            return res;
        }); 

        CROW_ROUTE(app, "/auth")
        .methods(crow::HTTPMethod::Post)
        ([&](const crow::request& req) {
            unsigned hostIP = boost::asio::ip::make_address_v4(req.remote_ip_address).to_uint();
            bool res = authUsers.count(hostIP);
            if (!res) {
                res = verifyUserByJsonPostReq(req);
                if (res) {
                    authUsers.insert(hostIP);
                }
            }
            return res ? "yes" : "no";
        }); 

        CROW_ROUTE(app, "/secret") ([&](const crow::request& req){
            unsigned hostIP = boost::asio::ip::make_address_v4(req.remote_ip_address).to_uint();
            if (!authUsers.count(hostIP)) {
                return crow::response("no secret for you");
            }
            crow::response res;
            res.set_static_file_info_unsafe("C:/Users/mset6/Downloads/Telegram Desktop/i.webp");
            return res;
        });
    }

    bool verifyUserByJsonPostReq(const crow::request& req) {
        auto json_data = crow::json::load(req.body);
        if (!json_data) {
            return false;
        }
        std::string login = json_data["login"].s();
        std::string pword = json_data["pword"].s();
        return db->isUserExists(user::User(login, pword));
    }

    bool registerUserByJsonPostReq(const crow::request& req) {
        auto json_data = crow::json::load(req.body);
        if (!json_data) {
            return false;
        }
        std::string login = json_data["login"].s();
        std::string pword = json_data["pword"].s();
        db->insertUserIfNotExists(user::User(login, pword));
        return true;
    }

};

} // namespace crow_test_http_server

#endif // CROW_TEST_HTTP_SERVER_H