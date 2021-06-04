#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

const char SERVER_IP[]					= "127.0.0.1";
constexpr unsigned short PORT_NUMBER	= 9000;

class TCP_Client{
private:

	boost::asio::io_context&		io_service_;
	boost::asio::ip::tcp::socket	socket_;

	int								nSeqNumber_;
	std::array<char, 128>			receiveBuffer_;
	std::string						writeMessage_;

	void PostWrite(){
		if (socket_.is_open() == false){
			return;
		}

		if (nSeqNumber_ > 7){
			socket_.close();
			return;
		}

		++nSeqNumber_;

		char szMessage[128] = { 0, };
		sprintf_s(szMessage, 128 - 1, "%d - Send Message", nSeqNumber_);

		writeMessage_ = szMessage;

		boost::asio::async_write(socket_, boost::asio::buffer(writeMessage_),
			boost::bind(&TCP_Client::handleWrite, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));


		PostReceive();
	}

	void PostReceive()
	{
		memset(&receiveBuffer_, '\0', sizeof(receiveBuffer_));

		socket_.async_read_some(boost::asio::buffer(receiveBuffer_),
			boost::bind(&TCP_Client::handleReceive, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

	}


	void handleConnect(const boost::system::error_code& error){

		if (error){
			std::cout << "connect failed : " << error.message() << std::endl;
		}

		else{
			std::cout << "connected" << std::endl;
			PostWrite();
		}
	}

	void handleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/){}

	void handleReceive(const boost::system::error_code& error, size_t bytes_transferred){

		if (error){
			if (error == boost::asio::error::eof){
				std::cout << "서버와 연결이 끊어졌습니다" << std::endl;
			}
			else{
				std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
			}
		}

		else{
			const std::string strRecvMessage = receiveBuffer_.data();
			std::cout << "클라이언트에서 받은 메시지: " << strRecvMessage << ", 받은 크기: " << bytes_transferred << std::endl;

			PostWrite();
		}
	}


public:
	TCP_Client(boost::asio::io_context& io_service)
		: io_service_(io_service),
		socket_(io_service),
		nSeqNumber_(0)
	{}

	void Connect(boost::asio::ip::tcp::endpoint& endpoint){
	
		//비동기 Connect(접속할 서버주소, 접속 완료 후 호출할 함수)
		socket_.async_connect(endpoint,boost::bind(&TCP_Client::handleConnect,this,boost::asio::placeholders::error));
	}

};



int main(){
	boost::asio::io_context io_service;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(SERVER_IP), PORT_NUMBER);

	TCP_Client client(io_service);

	client.Connect(endpoint);

	io_service.run();


	std::cout << "네트웍 접속 종료" << std::endl;

	getchar();
	return 0;
}