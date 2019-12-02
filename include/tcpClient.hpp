#include <string>
#include "clx/tcp.h"
// #include <boost/asio.hpp>


// namespace asio = boost::asio;
// using asio::ip::tcp;

using namespace std;

class TCPClient{
public:
    TCPClient(string host, int port);

    void connect();

    void send(string message);

    string recv();

    void move(string unit, string direct);

    void close();

private:
    // asio::io_service io_service;
    // tcp::socket socket;
    clx::tcp::socket s;
    string host;
    int port;
};
