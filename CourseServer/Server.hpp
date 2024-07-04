#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <string>
#include <stdexcept>
#include <tchar.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <functional>

#include "Request.hpp"

class Server {
private:
	struct sockaddr_in _addr;
	SOCKET _server;
	SOCKET _client;
public:
	Server(int port) {
		this->_server = socket(AF_INET, SOCK_STREAM, NULL);
		if (this->_server == SOCKET_ERROR || this->_server == INVALID_SOCKET) {
			throw std::runtime_error(std::string("Invalid Socket: ") + std::to_string(WSAGetLastError()));
		}
		
		this->_addr.sin_family = AF_INET;
		this->_addr.sin_port = htons(port);
		InetPton(AF_INET, _T("0.0.0.0"), &this->_addr.sin_addr);

		int bRes = bind(this->_server, (struct sockaddr*)&this->_addr, sizeof(this->_addr));
		if (bRes == SOCKET_ERROR) {
			throw std::runtime_error(std::string("Invalid Socket: ") + std::to_string(WSAGetLastError()));
		}
	};

	void serve(const int bufSize, std::function<void(Request)> on_request) {
		int res = listen(this->_server, AF_INET);

		while (true) {
			this->_client = accept(this->_server, NULL, NULL);
			if (this->_client == SOCKET_ERROR) {
				continue;
			}
			
			Request req(this->_client, bufSize);
			on_request(req);
			closesocket(this->_client);
		}
	}
};

#endif