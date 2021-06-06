#pragma once
#define BOOST_ASIO_NO_DEPRECATED

#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include"ChatServer.h"

using namespace boost;

constexpr int MAX_RECEIVE_BUF_SIZE = 1028;

class CSession {
private:
    asio::ip::tcp::socket                       socket_;

    DWORD32                                     sessionID_;

    std::array<char,MAX_RECEIVE_BUF_SIZE>       receiveBuffer_;

    int                                         receiveBufferMark_;

    std::deque<char*>                           sendQueue_;

    CChatServer*                                pChatServer_;

public:
    
    inline  asio::ip::tcp::socket& GetSocket() { return socket_; }

public:
    CSession(const DWORD32 sessionID, boost::asio::io_context& io_context, CChatServer* pChatServer) :
        socket_(io_context),
        sessionID_(sessionID),
        receiveBuffer_{},
        receiveBufferMark_(0),
        sendQueue_{},
        pChatServer_(pChatServer)
    {}

    inline void PostReceive() {
    
        socket_.async_read_some(boost::asio::buffer(receiveBuffer_), boost::bind(&CSession::ProcessReceive, this,
            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));


    }

    inline void ProcessReceive(const boost::system::error_code& error, size_t bytes_transferred) {
    
        if (error){

            if (error == boost::asio::error::eof){
                std::cout << "클라이언트와 연결이 끊어졌습니다" << std::endl;
            }

            else{
                std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
            }

            //m_pServer->CloseSession(m_nSessionID);
        }
        else {

            memcpy(&receiveBuffer_, receiveBuffer_.data(), bytes_transferred);

            int nPacketData = receiveBufferMark_ + bytes_transferred;
            int nnReadData = 0;






            //Re Receive
            PostReceive();
        
        }
    
    }


    inline void PostSend(const bool bImmediately, const int nSize, char* pData) {
    
        char* pSendData{ nullptr };

        if (bImmediately == false) {

            pSendData = new char[nSize];
            memcpy(pSendData, pData, nSize);

            sendQueue_.push_back(pSendData);

        }

        else {
            pSendData = pData;
        }

        if (bImmediately == false && sendQueue_.size() > 1) {
            return;
        }

        //socket write는 다 전송이 안되도 리턴이 될 수 있음 그래서 전역 write 사용
        boost::asio::async_write(socket_, boost::asio::buffer(pSendData, nSize),boost::bind(&CSession::ProcessSend, this,
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    inline void ProcessSend(const boost::system::error_code& error, size_t bytes_transferred) {

        delete[] sendQueue_.front();
        sendQueue_.pop_front();

        if (sendQueue_.empty() == false) {

            char* pData = sendQueue_.front();

        }

    }


};