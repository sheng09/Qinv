#include "tracef1d.hh"
#include "vector"

#ifndef __POLARIZE
#define __POLARIZE

#define PI 3.1415926535

class parMot
{
public:
	parMot(int ac, char *av[]);
	parMot(	const std::string& Nname, 
			const std::string& Ename, 
			const std::string& Zname, 
			const std::string& Rname, 
			const std::string& Tname,float wb, float we);
	int init();
	~parMot() {};
	int rotate2gcp();
	int p_diag1();
	int outRT();
	int outInfo();
	int run();

private:
	float d_wb, d_we;
	float d_Rcos;
	float d_Rsin;
	float d_baz;
	//float d_Angle;
	std::string d_Nname;
	std::string d_Ename;
	std::string d_Zname;
	std::string d_Rname;
	std::string d_Tname;
	tracef1d d_n;
	tracef1d d_e;
	tracef1d d_z;
	tracef1d d_r;
	tracef1d d_t;
};

#endif