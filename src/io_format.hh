#include <fstream>
#include <iostream>
#include <string>

#ifndef __ATTEN__IO__FORMAT__
#define __ATTEN__IO__FORMAT__



class outPlainFile
{
public:
	outPlainFile(const std::string &filename) : d_ofs(filename.c_str(), std::ofstream::out) {
		//d_ofs.precision(18);
		//d_ofs.width(18);
		d_ofs.flags( std::ios::left);
	}
	~outPlainFile() { d_ofs.close(); }

	void outColf(float data) {
		d_ofs << data << '\n';
	}
	void outCol2f(float data1, float data2) {
		d_ofs << data1 << ' ' << data2 << '\n';
	}
	void outCol3f(float v1, float v2, float v3) {
		d_ofs << v1 << ' ' << v2 << ' ' << v3 << '\n';
	}
	void outChar(char c) {
		d_ofs << c << ' ';
	}
private:
	std::ofstream d_ofs;
};


#endif