#include <iostream>
#include <vector>

#include "crow.h"

#include "windows.h"

#include "include/db.h"
#include "include/user.h"

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    
    db::DB db("dbname=cpptest user=postgres password=1568");

    crow::SimpleApp app;

    

    CROW_ROUTE(app, "/")([]() {
        crow::response res;
        res.set_static_file_info_unsafe("D:/testWebProd/src/statics/index.html");
        return res;
    }); 

    CROW_ROUTE(app, "/script.js")([]() {
        crow::response res;
        res.set_static_file_info_unsafe("D:/testWebProd/src/statics/script.js");
        return res;
    }); 

    CROW_ROUTE(app, "/style.css")([]() {
        crow::response res;
        res.set_static_file_info_unsafe("D:/testWebProd/src/statics/style.css");
        return res;
    }); 
    
    CROW_ROUTE(app, "/pic.png")([]() {
        crow::response res;
        res.set_static_file_info_unsafe("D:/testWebProd/src/statics/pic.png");
        return res;
    }); 



    app.bindaddr("192.168.0.102")
    .port(8000)
    .multithreaded()
    .run();
}
