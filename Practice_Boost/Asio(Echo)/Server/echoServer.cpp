#include<boost/asio.hpp>
#include<iostream>

constexpr char SERVER_IP[] = "127.0.0.1";
constexpr unsigned short PORT_NUMBER = 9000;


int main() {

    // 커널에서 발생한 I/O 이벤트를 디스패치 해주는 Class
    // 해당 클래스를 통해 커널에서 발생한 네트워크상의 접속 받기, 접속하기, 데이터 받기, 데이터 보내기 이벤트를 알 수 있다.
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::endpoint endpoint{ boost::asio::ip::tcp::v4(),PORT_NUMBER };

    //클라이언트의 접속을 받는 클래스
    boost::asio::ip::tcp::acceptor acceptr{ io_service,endpoint };

    //TCP 소켓 생성 보낸 메시지를 주고 받아야 하기 때문에 Io_server를 할당
    boost::asio::ip::tcp::socket socket(io_service);

    //acceptor에 클라이언트용 socket 전달
    acceptr.accept(socket);

    std::cout << "클라이언트 접속\n";

    while (true) {

        //버퍼설정
        std::array<char, 128> buf{};

        //시스템에서 발생하는 에러 코드를 랩핑한 클래스
        boost::system::error_code error;

        // 클라이언트가 보낸 데이터를 받는다.(받은 크기 반환)
        // 클라이언트 접속이 끊겨지는 경우도 확인가능
        size_t len = socket.read_some(boost::asio::buffer(buf), error);


        if (error) {

            //연결이 끊어졌을 때 eof 오류 발생
            if (error == boost::asio::error::eof) {
                std::cout << "클라이언트와 연결이 끊어졌습니다.\n";
            }
            else {
                std::cout << "error No: " << error.value() << "Error Message: " << error.message() << "\n";

            }
            break;

        }
        std::cout << "클라이언트에서 받은 메시지:" << &buf[0] << "\n";

        //버퍼 설정
        char szMessage[128]{};
        sprintf_s(szMessage, 128 - 1, "Re:%s", &buf[0]);
        int mMsgLen = strnlen_s(szMessage, 128 - 1);

        boost::system::error_code ignored_error;
        
        //  메시지를 보낼 때 사용
        //  보낼 데이터량을 지정할 수 있음
        socket.write_some(boost::asio::buffer(szMessage, mMsgLen), ignored_error);
        std::cout << "클라이언트에 보낸 메시지" << szMessage << "\n";

    }
    getchar();

    return 0;

}