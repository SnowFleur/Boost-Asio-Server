#define BOOST_ASIO_NO_DEPRECATED
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<queue>
#include<iostream>
#include<vector>
#include"Session.h"

using namespace boost;

class CChatServer {

private:
    asio::io_context            ioConText_;
    asio::ip::tcp::acceptor     acceptor_;
    std::vector<CSession*>      sessionList_;
    std::queue<int>             sessionQueue_;

public:

    CChatServer()       = default;
    ~CChatServer()      = default;

public:



    inline void Run() {
    

    }


    inline void PostAccept() {
    
    }

    inline void ProcessAccept() {
    
    
    }


};