#pragma once
#define BOOST_ASIO_NO_DEPRECATED

#include<boost/asio.hpp>
#include<boost/bind.hpp>

using namespace boost;

constexpr int MAX_RECEIVE_BUF_SIZE = 1028;

class CChatServer;

class CSession {
private:
    asio::ip::tcp::socket                       socket_;

    DWORD32                                     sessionID_;

    std::array<char,MAX_RECEIVE_BUF_SIZE>       receiveBuffer_;

    int                                         receiveBufferMark_;

    std::deque<unsigned char*>                  sendQueue_;

    CChatServer*                                pChatServer_;
public:

    CSession(const DWORD32 sessionID, boost::asio::io_context& io_context, CChatServer* pChatServer) :
        socket_(io_context),
        sessionID_(sessionID),
        receiveBuffer_{},
        receiveBufferMark_(0),
        sendQueue_{},
        pChatServer_(pChatServer)
    {}

public:
    inline  asio::ip::tcp::socket& GetSocket() { return socket_; }
    inline  DWORD32 GetSessionID()const { return sessionID_; }
  
    void PostReceive();

    void ProcessReceive(const boost::system::error_code& error, size_t bytes_transferred);

    void PostSend(const bool bImmediately, const int nSize, unsigned char* pData);

    void ProcessSend(const boost::system::error_code& error, size_t bytes_transferred);

};