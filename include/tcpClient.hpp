#include <string>
#include "clx/tcp.h"
// #include <boost/asio.hpp>


// namespace asio = boost::asio;
// using asio::ip::tcp;

using namespace std;

class TCPClient{
public:
    TCPClient(string host, int port){
        this->host = host;
        this->port = port;
        connect();
    }

    void connect(){
        s.connect(host, port);
    }

    void send(string message){
        s.send(message);
    }

    string recv(){
        char buff[256];
        s.recv(buff);

        return string(buff);
    }

    void move(string unit, string direct){
        send("MOV:" + unit + "," + direct + "\r\n");
    }

    void close(){
        s.close();
    }

private:
    // asio::io_service io_service;
    // tcp::socket socket;
    clx::tcp::socket s;
    string host;
    int port;
};
