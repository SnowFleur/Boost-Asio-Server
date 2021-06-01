#include<boost/asio.hpp>
#include<iostream>

constexpr char SERVER_IP[] = "127.0.0.1";
constexpr unsigned short PORT_NUMBER = 9000;


int main() {

    boost::asio::io_service io_service;

    boost::asio::ip::tcp::endpoint endpoint{ boost::asio::ip::tcp::v4(),PORT_NUMBER };

    boost::asio::ip::tcp::acceptor acceptr{ io_service,endpoint };

    boost::asio::ip::tcp::socket socket(io_service);

    acceptr.accept(socket);

    std::cout << "클라이언트 접속\n";

    while (true) {

        std::array<char, 128> buf{};

        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);


        if (error) {

            if (error == boost::asio::error::eof) {
                std::cout << "클라이언트와 연결이 끊어졌습니다.\n";
            }
            else {
                std::cout << "error No: " << error.value() << "Error Message: " << error.message() << "\n";

            }
            break;

        }
        std::cout << "클라이언트에서 받은 메시지:" << &buf[0] << "\n";
        char szMessage[128]{};
        sprintf_s(szMessage, 128 - 1, "Re:%s", &buf[0]);
        int mMsgLen = strnlen_s(szMessage, 128 - 1);

        boost::system::error_code ignored_error;
        socket.write_some(boost::asio::buffer(szMessage, mMsgLen), ignored_error);
        std::cout << "클라이언트에 보낸 메시지" << szMessage << "\n";
    }
    getchar();

    return 0;

}