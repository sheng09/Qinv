#include "q_job.hh"


qjob::qjob(const std::string& sacfile, int tmark, float t1, float t2)
	: d_fft_size(0), d_sacfilename(sacfile) {
	initQjob(sacfile, tmark, t1, t2);
}
qjob::qjob(const std::string& sacfile, int tmark, float t1, float t2,
	float winB1, float winE2, float dlag)
	: d_fft_size(0), d_sacfilename(sacfile)
{
	initQjob(sacfile, tmark, t1, t2, winB1, winE2, dlag);
}

int qjob::initQjob(const std::string& sacfile, int tmark, float t1, float t2, float winB1, float winE2, float dlag) {
	d_attentrace.init(sacfile, tmark, t1, t2, d_fft_size);

	int itmp;
	float start = d_attentrace.d_sac.getStart();
	float delta = d_attentrace.d_sac.getDelta();
	float t5 = d_attentrace.d_sac.getTinc();
	float t6 = d_attentrace.d_sac.getTref();
	float winB2 = d_attentrace.d_sac.floorCrossZero( t5, &itmp ) - t5;
	DPRINT("floorCrossZero %f", winB2);
	float winE1 = d_attentrace.d_sac.ceilCrossZero(  t5, &itmp ) - t5;
	DPRINT("ceilCrossZero %f", winE1);

	d_winopt.init(start, delta, t5, t6, winB1, winB2, winE1, winE2, dlag);
	//d_winopt.alloc();
	return 0;
}
int qjob::initQjob(const std::string& sacfile, int tmark, float t1, float t2) {
	d_attentrace.init(sacfile, tmark, t1, t2, d_fft_size);
	return 0;
}
