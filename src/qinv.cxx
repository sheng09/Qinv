#include "jobQinv.hh"
#include <fftw3.h>
int main(int argc, char *argv[]) {
	jobQinv job(argc, argv);
	job.run();
	fftwf_cleanup();
	return 0;
}