#include "test.hh"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <vector>

#include "qkeyqinvTrace.hh"
#ifndef __ATTEN__VERIFY__HS2Q___
#define __ATTEN__VERIFY__HS2Q___

class stackQinv
{
public:
	stackQinv() {};
	stackQinv(int ac, char *av[]);
	stackQinv(std::string filelst, int nline, float tau) { setTau(tau); init(filelst, nline); };
	~stackQinv() { DPRINT("stackQinv::decontructor: %p", this);	clear(); }
	int init(std::string filelst, int nline);
	int fill();
	int makeValid(float maxPhase, float maxAmp);
	int aveSub();
	int mergeConflict(); // not availale now
	int clear() ;
	qKeyqinvTrace* find_df(float df) ;
	int output1st(std::string filename) ;
	void setTau(float tau) {d_tau = tau; };
	float getTau() { return d_tau; }
	int run();
private:
	float d_tau;
	int d_nline;
	std::vector< std::string > d_prenames;
	std::vector< qKeyqinvTrace * > d_data;
	qinvTrace d_ak;


private: 
	float d_maxPhase, d_maxAmp;
	std::string d_outfilename;
};

#endif