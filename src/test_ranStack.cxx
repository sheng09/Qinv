#include "ranStack.hh"
#include <string>
#include <ctime>

int main(int argc, char *argv[])
{
	std::string sacname(argv[1]);
	int tmark = -3;
	float t1  = -5;
	float t2  = 60;
	long seed  = (long) clock();
	int nroot = 8;
	int nstack = 6666;
	float level = 0.1;
	ranStack tr(sacname,tmark,t1,t2,seed,nroot,nstack);
	tr.noiseStack(level);
	tr.outSac("tmp.test_ranStack.sac");
	return 0;
}