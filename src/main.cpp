#include <memory> 

#include "include/db.h"
#include "include/user.h"
#include "include/crowTestHTTPServer.h"


int main() {
    SetConsoleOutputCP(CP_UTF8); 
    
    crow_test_http_server::CrowTestHTTPServer server(
        std::make_shared<db::DB>("dbname=cpptest user=postgres password=1568"),
        std::string("192.168.0.102"),
        8001
    );

    server.serverRun();
}
