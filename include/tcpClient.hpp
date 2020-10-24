#include <string>
#include <thread>
#include <chrono>
#define USE_ASIO
#ifdef USE_ASIO
#define ASIO_STANDALONE
#include "asio.hpp"
#else
#include "clx/tcp.h"
#endif

class TCPClient{
public:
    TCPClient(std::string host, int port):
#ifdef USE_ASIO
    sock(io_service),
#endif
    host(host),
    port(port)
    {
    }

    void connect(const size_t retryCount = 1){
#ifdef USE_ASIO
        asio::ip::tcp::resolver resolver(io_service);
        asio::ip::tcp::resolver::query query(host, "http");
        auto ip = resolver.resolve(query)->endpoint().address();
#endif

        for (size_t counter = 1; counter <= 5; ++counter){
            try{
#ifdef USE_ASIO
                sock.connect(asio::ip::tcp::endpoint(ip, port));
#else
                sock.connect(host, port);
#endif
                return;
            }
            catch(const std::exception& error){
                if (counter == retryCount){
                    std::cerr << error.what() << std::endl;
                    exit(EXIT_FAILURE);
                }
                close();
                std::cerr << "connect is failure. retry: " << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::microseconds(100000*counter));
            }
        }
    }

    void send(std::string message){
#ifdef USE_ASIO
        asio::error_code error;
        asio::write(sock, asio::buffer(message), error);

        if (error) {
            std::cout << "send failed: " << error.message() << std::endl;
            exit(EXIT_FAILURE);
        }
#else
        sock.send(message);
#endif
    }

    std::string recv(){
#ifdef USE_ASIO
        asio::streambuf receive_buffer;
        asio::error_code error;
        asio::read_until(sock, receive_buffer, "\r\n", error);
        
        if (error && error != asio::error::eof) {
            std::cout << "receive failed: " << error.message() << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            if (receive_buffer.size()){
                const char* data = asio::buffer_cast<const char*>(receive_buffer.data());
                return data;
            }
            else{
                std::cout << "receive empty" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
#else
        char buff[256];
        sock.recv(buff);
        return buff; 
#endif
    }

    void move(std::string unit, std::string direct){
        send("MOV:" + unit + "," + direct + "\r\n");
    }

    void close(){
#ifdef USE_ASIO
        while(sock.is_open()){
#endif
            sock.close();
#ifdef USE_ASIO
        }
#endif
    }

private:
#ifdef USE_ASIO
    asio::io_service io_service;
    asio::ip::tcp::socket sock;
#else
    clx::tcp::socket sock;
#endif
    std::string host;
    int port;
};
