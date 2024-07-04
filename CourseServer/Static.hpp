#ifndef _STATIC_HPP
#define _STATIC_HPP

#include <WinSock2.h>
#include <fstream>
#include <string>
#include <sstream>

#include "MimeTypes.h"

class Static {
private:
	std::ofstream os;
	std::ifstream is;
public:

	std::string get(std::string path) {
		std::stringstream stream;
		std::string str;
		this->is.open(path, std::ios::binary);
		if (!this->is.good()) {
			return "";
		}
		stream << this->is.rdbuf();
		str = stream.str();
		this->is.close();

		return str;
	}
};

#endif
