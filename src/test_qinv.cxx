#include "jobQinv.hh"
#include <fftw3.h>
int main(int argc, char const *argv[])
{
	float t1 = 12.7;
	float t2 = 13.5;
	float lag = 1.3;
	float taperLen = 0.1;
	float width = 1.5;

	jobQinv job("../exam/data/VEL.SHT.SAC",	5, -3, 5, t1, t2, lag, taperLen, width, 512);
	job.setOutput("tmp.test_qinv");
	job.run();

	fftwf_cleanup();
	return 0;
}