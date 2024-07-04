
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "MimeTypes.h"
#include "Server.hpp"
#include "Request.hpp"

#define SERVER_PORT 8080
#define BUFMAX 1024 * 1000

#pragma comment(lib, "ws2_32")


int main() {
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "Winsock failed " << std::endl;
		return 1;
	}
	std::cout << "Socket API started" << std::endl;
	
	try {
		Server srv(8080);
		srv.serve(BUFMAX, [&](Request req) {
			
			
			std::string fname = "./static" + req.path;
			std::ifstream file(fname, std::ios::binary);

			if (file.good() && req.method == "GET") {
				std::stringstream stream;
				stream << file.rdbuf();
				std::string str = stream.str();
				std::cout << str << std::endl;
				const char* mimeType = MimeTypes::getType(fname.c_str());

				req.write("200 OK", mimeType, str.size(), str);
			}
			else {
				std::string msg = "hello, world";
				req.write("200 OK", "text/plain", msg.size(), msg);
			}
		});
	}
	catch (const std::runtime_error& e) {
		std::cout << "Caught exception:" << e.what() << std::endl;
	}
	WSACleanup();
	return 0;
}