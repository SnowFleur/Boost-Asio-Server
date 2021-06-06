#pragma once
#define BOOST_ASIO_NO_DEPRECATED

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include<iostream>
#include<queue>
#include"../protocol_generated.h"

using namespace boost;
using namespace ChatServerProtocol;

constexpr   DWORD32     PORT_NUMBER             = 9000;
constexpr   DWORD32     MAX_RECEIVE_BUF_SIZE    = 1028;

class CChatClient {
private:
    asio::io_context                            ioConText_;

    asio::ip::tcp::socket                       socket_;

    DWORD32                                     sessionID_;

    std::array<char, MAX_RECEIVE_BUF_SIZE>      receiveBuffer_;

    int                                         receiveBufferMark_;

    std::deque<char*>                           sendQueue_;

public:
    CChatClient() :
        ioConText_{},
        socket_{ioConText_},
        sessionID_{ 0 },
        receiveBuffer_{},
        receiveBufferMark_{},
        sendQueue_{}
    {
        

    }

public:

    inline void Connect(boost::asio::ip::tcp::endpoint endpoint) {
    
    
        socket_.async_connect(endpoint,
            boost::bind(&CChatClient::ProcessConnect, this, asio::placeholders::error));
    }

    inline void PostReceive() {
        socket_.async_read_some(boost::asio::buffer(receiveBuffer_), boost::bind(&CChatClient::ProcessReceive, this,
            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

    }

    inline void ProcessReceive(const boost::system::error_code& error, size_t bytes_transferred) {

        if (error) {
            std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
        }

        else {
            std::cout << "Receive Packet Size: " << bytes_transferred << "\n";

            auto packet = Getsc_loginOk_packet(&receiveBuffer_);

            std::cout << "ID: " << packet->session_id() << "\n";
            PostReceive();
        }

    }

    inline void ProcessConnect(const boost::system::error_code& error) {

        if (error) {
            std::cout << "서버 접속 실패. error No: " << error.value() << " error Message: " << error.message() << std::endl;
        }
        else {
            std::cout << "서버 접속 성공" << std::endl;
            PostReceive();
        }

    }


    inline void Run() {

        std::cout << "Start Chat Server\n";

        Connect(asio::ip::tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), PORT_NUMBER));

        ioConText_.run();
    }

};