#pragma once
#define BOOST_ASIO_NO_DEPRECATED
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<queue>
#include<iostream>
#include<vector>
#include"Session.h"
#include"../protocol_generated.h"


using namespace boost;
using namespace ChatServerProtocol;


class CSession;

constexpr   DWORD32       PORT_NUMBER   = 9000;
constexpr   DWORD32       MAX_CLIENT    = 30;

class CChatServer {

private:
    asio::io_context            ioConText_;
    asio::ip::tcp::acceptor     acceptor_;
    std::vector<CSession*>      sessionList_;
    std::queue<int>             sessionQueue_;

public:
    ~CChatServer()      = default;
    
    CChatServer() :
        ioConText_{},
        acceptor_(ioConText_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), PORT_NUMBER)),
        sessionList_{},
        sessionQueue_{}
    {
        for (DWORD32 i = 0; i < MAX_CLIENT; ++i) {
            sessionList_.emplace_back(
                new CSession{ i,ioConText_.get_executor().context() ,this });
        }
    }

public:


    inline void Run() {
    
        std::cout << "Start Chat Server\n";

        PostAccept();

        ioConText_.run();

    }


    inline void PostAccept() {
    
        static int sessionID{ 0 };


        int nSessionID = sessionID++;

        acceptor_.async_accept(sessionList_[nSessionID]->GetSocket(),
            boost::bind(&CChatServer::ProcessAccept, this, sessionList_[nSessionID], asio::placeholders::error));

    }

    inline void ProcessAccept(CSession* pSession, const boost::system::error_code& error) {

        if (error) {
            std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;


        }
        else {
            std::cout << "클라이언트 접속 성공: Session ID" << pSession->GetSessionID() << "\n";


            flatbuffers::FlatBufferBuilder builder{};

            //데이터 직렬화
            auto packet = Createsc_loginOk_packet(builder, sizeof(sc_loginOk_packet),pSession->GetSessionID());
            builder.Finish(packet);

          
            std::cout << "Send Packet Size: "<<builder.GetSize() << "\n";

            pSession->PostSend(false, builder.GetSize(), builder.GetBufferPointer());

            PostAccept();

        }
    
    }

};