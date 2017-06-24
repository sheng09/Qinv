#include "parMot.hh"
#include <string>

int main(int argc, char const *argv[]) {
	std::string n = "../exam/data/10_2.005.t"; // assumed N component
	std::string e = "../exam/data/10_2.005.r"; // assumed E component
	std::string z = "../exam/data/10_2.005.z";
	std::string r = "tmp.test_polarize.R";
	std::string t = "tmp.test_polarize.T";
	float wb = 8.2;
	float we = 9.0;
	parMot sac(n,e,z,r,t, wb,we);
	sac.run();
	return 0;
}