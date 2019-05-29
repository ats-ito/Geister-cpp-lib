#include "tcpClient.hpp"
#include "clx/tcp.h"


TCPClient::TCPClient(string host, int port)/*: socket(tcp::socket(io_service))*/{
    this->host = host;
    this->port = port;
    connect();
}

void TCPClient::connect(){
    s.connect(host, port);
}

string TCPClient::recv(){
    char buff[256];
    s.recv(buff);

    return string(buff);
}

void TCPClient::send(string message){
    s.send(message);
}

void TCPClient::move(string unit, string direct){
    send("MOV:" + unit + "," + direct + "\r\n");
}

void TCPClient::close(){
    s.close();
}
