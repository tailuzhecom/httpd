#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <sstream>

#define PORT 11213

using namespace std;
//todo
class HttpHandler {
public:
    HttpHandler() {
        if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            cout << "create socket error" << endl;
        memset(&s_addr, 0, sizeof(s_addr));
        s_addr.sin_family = AF_INET;
        s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        s_addr.sin_port = htons(PORT);
        socklen_t len = sizeof(s_addr);
        if(bind(sock_fd, (sockaddr*)&s_addr, len) < 0) {
            cout << "bind error" << endl;
            cout << errno << endl;
        }
        getsockname(sock_fd, (sockaddr*)&s_addr, &len);
        cout << "Httpd is running on port : " << s_addr.sin_port << endl;
        if(listen(sock_fd, 10) < 0)
            cout << "listen error" << endl;
    }
    //response the request
    void response(int status, int cli_fd) {
        socklen_t len;
        len = sizeof(sockaddr);
        char buf[4096];
        string GET_STR, POST_STR;
        GET_STR = "HTTP/1.0 201 Ok\r\n"
                "Content-Type: text/html\r\n"
                "\r\n"
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset=\"utf-8\">"
                "<title>Httpd</title>"
                "</head>"
                "<body>"
                "<h1 style=\"background-color:red;\">Httpd</h1>"
                "<p>Welcome to my Httpd.</p>"
                "</body>"
                "</html>";
            if(status == 1)
                send(cli_fd, GET_STR.c_str(), GET_STR.size(), 0);
    }
    //parse the request message
    int messageParse(string request_messge) {
        stringstream s_stream(request_messge);
        string str;
        s_stream >> str;
        if(str == "GET")
            return 1;
        else
            return 2;
    }
    //httpd loop
    void loop() {
        int cli_fd;
        char buf[4096];
        socklen_t len = sizeof(s_addr);
        while(1) {
            if((cli_fd = accept(sock_fd, (sockaddr*)&s_addr, &len)) > 0) {
                recv(cli_fd, buf, sizeof(buf), 0);
                cout << buf << endl;
                int status = messageParse(buf);
                response(status, cli_fd);
                close(cli_fd);
            }
            else {
                cout << "accept error" << endl;
            }
        }
    }
    ~HttpHandler() {
        close(sock_fd);
    }
private:
    typedef enum {
        GET, POST
    }request_status;
    int sock_fd;
    sockaddr_in s_addr;
    string content;
};

int main() {
    HttpHandler test_handler;
    test_handler.loop();
    return 0;
}