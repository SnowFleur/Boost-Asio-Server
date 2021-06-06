#include"ChatServer.h"

void CSession::PostReceive() {

    socket_.async_read_some(boost::asio::buffer(receiveBuffer_), boost::bind(&CSession::ProcessReceive, this,
        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));


}

void CSession::ProcessReceive(const boost::system::error_code& error, size_t bytes_transferred) {

    if (error) {

        if (error == boost::asio::error::eof) {
            std::cout << "클라이언트와 연결이 끊어졌습니다" << std::endl;
        }

        else {
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


void CSession::PostSend(const bool bImmediately, const int nSize, unsigned char* pData) {

    unsigned char* pSendData{ nullptr };

    if (bImmediately == false) {

        pSendData = new unsigned char[nSize];
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
    boost::asio::async_write(socket_, boost::asio::buffer(pSendData, nSize), boost::bind(&CSession::ProcessSend, this,
        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CSession::ProcessSend(const boost::system::error_code& error, size_t bytes_transferred) {

    delete[] sendQueue_.front();
    sendQueue_.pop_front();

    if (sendQueue_.empty() == false) {

        unsigned char* pData = sendQueue_.front();

    }

}
