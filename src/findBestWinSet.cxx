#include <vector>
#include "jobwinSet.hh"
#include <fftw3.h>

int main(int argc, char *argv[])
{
	jobwinSet jobs(argc, argv);
	jobs.run();
	fftwf_cleanup();
	return 0;
}