
#include <iostream>

#include "Server.hpp"
#include "Request.hpp"
#include "Static.hpp"

#define SERVER_PORT 8080
#define BUFMAX 1024 * 1000

#pragma comment(lib, "ws2_32")

int main() {
	WSADATA wsa;
	Static st;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "Winsock failed " << std::endl;
		return 1;
	}
	std::cout << "Socket API started" << std::endl;
	
	try {
		Server srv(8080);
		srv.serve(BUFMAX, [&](Request req) {

			std::string fname = "./static" + req.path;
			if (req.method == "GET") {
				std::string file = st.get(fname);
				if (file == "") {
					req.write("404 Not Found", "File doesn't exist or is empty");
					return;
				}
				req.write("200 OK", MimeTypes::getType(fname.c_str()), file.size(), file);
				return;
			}

			std::string msg = "Not found";
			req.write("404 Not Found", "text/plain", msg.size(), msg);
			return;
		});
	}
	catch (const std::runtime_error& e) {
		std::cout << "Caught exception:" << e.what() << std::endl;
	}
	WSACleanup();
	return 0;
}