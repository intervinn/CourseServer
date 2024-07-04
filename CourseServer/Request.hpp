
#ifndef _REQUEST_HPP
#define _REQUEST_HPP

#include <WinSock2.h>
#include <regex>

class Request {
public:
	std::string method;
	std::string path;
	std::string raw;
	SOCKET socket;
public:
	Request(SOCKET s, const int size) {
		this->socket = s;
		this->raw = this->receive(size);
		this->method = this->search_regex(this->raw, "(POST|GET)");
		this->path = this->search_regex(this->raw, "(\/[a-z.]+)");
	}

	void write(std::string status, std::string contentType, int contentLength, std::string body) {
		std::string res = "HTTP/1.1 " + status + "\nContent-Type: " + contentType + "\nContent-Length:" + std::to_string(contentLength) + "\n\n" + body;
		send(this->socket, res.c_str(), res.size(), NULL);
	}
private:
	std::string receive(const int size) {
		std::vector<char> buf(size);
		std::string res;

		int bytes;
		do {
			bytes = recv(this->socket, &buf[0], buf.size(), 0);
			if (bytes == -1) {
				return "";
			};
			if (bytes == 0) break;
			res.append(buf.cbegin(), buf.cend());
			if (bytes < size) break;
		} while (1);
		return res;
	}

	std::string search_regex(std::string input, std::string pattern) {
		std::regex r(pattern);
		std::smatch match;

		if (std::regex_search(input, match, r)) {
			return match[0];
		}
		return "";
	}
};

#endif