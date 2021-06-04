#define BOOST_ASIO_NO_DEPRECATED

#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<iostream>

const char SERVER_IP[] = "127.0.0.1";
constexpr unsigned short PORT_NUMBER = 9000;


class Session {
private:
    boost::asio::ip::tcp::socket    socket_;
    std::string                     writeMessage_;
    std::array<char, 128>           receiveBuffer_;


    void HandleWrite(const boost::system::error_code& /*error*/, size_t /*bytesTransFereed*/) {

    }

    void HandleReceive(const boost::system::error_code& error, size_t bytesTransFereed) {

        if (error) {

            if (error == boost::asio::error::eof) {
                std::cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�.\n";
            }
            else {
                std::cout << "error No: " << error.value() << "Error Message: " << error.message() << "\n";
            }
        }
        else {
            const std::string strRecvMessage = receiveBuffer_.data();

            std::cout << "Ŭ���̾�Ʈ���� ���� �޽���: " << strRecvMessage << ", ���� ũ��: " << bytesTransFereed << "\n";

            char szMessage[128]{};
            sprintf_s(szMessage, 128 - 1, "Re:%s", strRecvMessage.c_str());
            writeMessage_ = szMessage;

            boost::asio::async_write(socket_, boost::asio::buffer(writeMessage_),
                boost::bind(&Session::HandleWrite, this, boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

            PostReceive();
        }

    }

public:
    Session(boost::asio::io_context& io_service) :
        socket_(io_service),
        writeMessage_{},
        receiveBuffer_{}
    {}


    inline boost::asio::ip::tcp::socket& GetSocket() { return socket_; }

    void PostReceive() {

        memset(&receiveBuffer_, 0, sizeof(receiveBuffer_));

        socket_.async_read_some(boost::asio::buffer(receiveBuffer_),
            boost::bind(&Session::HandleReceive, this,
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

};


class TCP_Server {
private:
    int                             nSeqNumber_;
    boost::asio::ip::tcp::acceptor  acceptor_;
    Session* pSession_;

    void StartAccept() {
        std::cout << "Ŭ���̾�Ʈ ���� ��� .....\n";


        pSession_ = new Session((boost::asio::io_context&)acceptor_.get_executor().context());

        //�۾��� �Ϸ�Ǹ� ȣ��� �Լ��� bind
        acceptor_.async_accept(pSession_->GetSocket(), boost::bind(&TCP_Server::HandleAccept, this, pSession_, boost::asio::placeholders::error));

    }

    void HandleAccept(Session* pSession, const boost::system::error_code& error) {

        if (!error) {
            std::cout << "Ŭ���̾�Ʈ ���� ����\n";

            //Start Async Receive
            pSession->PostReceive();
        }
    }

public:
    TCP_Server(boost::asio::io_context& io_service) :
        acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
        pSession_{ nullptr }
    {
        StartAccept();

    }
    ~TCP_Server() {
        if (pSession_ != nullptr) {
            delete pSession_;
        }
    }

};




int main() {

    //�񵿱� �Լ��� ��û�۾�(���� �ޱ�, ������ �ޱ�, ������ ������)�� �Ϸ�Ǹ� ������ �Ϸ� �Լ��� ȣ���ϴ� ���� 
    //io_service --> io_context�̴�.
    boost::asio::io_context io_service{};

    TCP_Server server(io_service);

    // main���� ���ѷ���
    // �񵿱� �Լ��۾��� ������ �񵿱� �Լ��� ����� �Լ� ȣ��
    io_service.run();

    std::cout << "��Ʈ��ũ ����\n";


    getchar();

    return 0;
}