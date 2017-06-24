#include "jobwin.hh"
#include <fftw3.h>
int main(int argc, char  *argv[])
{
	//std::string sacfile = "../vel.2015.059.03.19.30.0000.TJ.NHZ.00.SHT.D.SAC";
	std::string sacfile = "../exam/data/VEL.SHT.SAC";
	int tmark = 5;
	float t1 = -4, t2 = 10, dlag = 0.5, winB = -1.5, winE = 1.5, taperLen = 0.05, f1 = 1.0, f2 = 10, smooothlength = 1.5;

	jobWin job(sacfile, tmark, t1, t2, winB, winE, dlag);
	job.FixLagTime(1.30);
	job.setMinGap(0.2);
	job.setMaxLen(1.2);
	job.init(taperLen, smooothlength, f1, f2);
	job.setOutput(true, "tmp.test_findBestWin");
	job.run();
	fftwf_cleanup();
	return 0;
}