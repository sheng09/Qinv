#include "winOpt.hh"
#include <cstdlib>
#include <cstdio>
#include "io_format.hh"
winOpt::winOpt() : d_pre(0), d_suf(0), d_coef(0), d_cor(0), d_bestlag(0) {}
winOpt::winOpt(float start, float delta,float t5, float t6, float winB1, float winB2,
		float winE1, float winE2, float dlag)
	: d_pre(0), d_suf(0), d_coef(0), d_cor(0), d_bestlag(0)
{
	init(start, delta, t5, t6, winB1, winB2, winE1, winE2, dlag);
}

int winOpt::init(float start, float delta,float t5, float t6, float winB1, float winB2, float winE1, float winE2, float dlag) {
	/*
                 t5              t6
                 |               |
	 b------+--------x---------+-----x------
	*/
	d_start = start;
	d_delta = delta;
	d_b1 = t5 + winB1; d_b2 = t5 + winB2;
	d_e1 = t5 + winE1; d_e2 = t5 + winE2;
	if(d_b1>d_b2) d_b2 = d_b1;  // DEV 2017/06/16
	if(d_e2<d_e1) d_e1 = d_e2;
	if(d_b2 >= d_e1) {
		fprintf(stderr, "Err: the start of the window is larger than that of the end!\n" );
		exit(-1);
	}
	d_lagref = t6 - t5;
	d_lag1 = d_lagref - dlag; d_lag2 = d_lagref + dlag;
	//////////////////////////////////////
	d_i1min =   time2idxFloor(d_b1); //floor( (d_b1- d_start) /d_delta );
	d_i1max =   time2idxFloor(d_b2); //floor( (d_b2- d_start) /d_delta );
	d_i2min =   time2idxCeil(d_e1);  //ceil( (d_e1- d_start) /d_delta );
	d_i2max =   time2idxCeil(d_e2);  //ceil( (d_e2- d_start) /d_delta );
	d_ilagmin = floor( (d_lag1) / d_delta );
	d_ilagmax = ceil(  (d_lag2) / d_delta );

	d_lenPre = d_i1max - d_i1min + 1;
	d_lenSuf = d_i2max - d_i2min + 1;
	d_lenOpt = d_lenPre * d_lenSuf;
	alloc();
	for(int i =0;i<d_lenPre; ++i)
		d_pre[i] = d_b1 + i * delta;
	for(int i =0;i<d_lenSuf; ++i)
		d_suf[i] = d_e1 + i * delta;
	return 0;
}
winOpt::~winOpt() {
	if(!d_pre.empty())      d_pre.clear();
	if(!d_suf.empty())      d_suf.clear();
	if(!d_coef.empty())     d_coef.clear();
	if(!d_cor.empty())      d_cor.clear();
	if(!d_bestlag.empty())  d_bestlag.clear();
	//if(d_pre)      free(d_pre)    ;
	//if(d_suf)      free(d_suf)    ;
	//if(d_coef)     free(d_coef)   ;
	//if(d_cor)      free(d_cor)    ;
	//if(d_bestlag)  free(d_bestlag);
}
int winOpt::alloc() {
	d_pre.resize( d_lenPre );
	d_suf.resize( d_lenSuf );
	d_coef.resize( d_lenOpt );
	d_cor.resize( d_lenOpt );
	d_bestlag.resize( d_lenOpt );
	//d_pre      = (float*)     (calloc(d_lenPre, sizeof(float)) );
	//d_suf      = (float*)     (calloc(d_lenSuf, sizeof(float)) );
	//d_coef     = (float*)    (calloc(d_lenOpt, sizeof(float)) );
	//d_cor      = (float*)     (calloc(d_lenOpt, sizeof(float)) );
	//d_bestlag  = (float*) (calloc(d_lenOpt, sizeof(float)) );
	return 0;
}
float winOpt::findMaxCoef(float *pre, float *suf, float *lag) {
	float v = d_coef[0];
	int imax = 0;
	int ipre, isuf;
	for(int i=1; i<d_lenOpt; ++i) {
		if(d_coef[i] > v && d_cor[i] > 0.5 ) {
			imax = i;
			v = d_coef[i];
		}
	}
	*lag = d_bestlag[imax];
	idx_v2m(imax, ipre, isuf);
	*pre = idx2time(ipre);
	*suf = idx2time(isuf);
	return v;
}
float winOpt::findMaxCor(float *pre, float *suf, float *lag) {
	float v = d_cor[0];
	int imax = 0;
	int ipre, isuf;
	for(int i=1; i<d_lenOpt; ++i) {
		if(d_cor[i] > v ) {
			imax = i;
			v = d_cor[i];
		}
	}
	*lag = d_bestlag[imax];
	idx_v2m(imax, ipre, isuf);
	*pre = idx2time(ipre);
	*suf = idx2time(isuf);
	return v;
}
int winOpt::makeReal() {
	int ipre, isuf;
	for(int i =0; i<d_lenOpt; ++i) {
		idx_v2m(i, ipre, isuf);
		if( ceil(d_bestlag[i] / d_delta)+ipre < isuf )
			d_coef[i] = 0.0;
	}
	return 0;
}
int winOpt::outCoef3Col(const std::string& filename) {
	outPlainFile out(filename);
	int index = 0;
	for(int ipre = 0; ipre<d_lenPre; ++ipre)
		for(int isuf = 0; isuf<d_lenSuf; ++isuf)
			out.outCol3f(d_pre[ipre], d_suf[isuf], d_coef[index++] );
	
	return 0;
}
int winOpt::outCor3Col(const std::string& filename) {
	outPlainFile out(filename);
	int index = 0;
	for(int ipre = 0; ipre<d_lenPre; ++ipre)
		for(int isuf = 0; isuf<d_lenSuf; ++isuf)
			out.outCol3f(d_pre[ipre], d_suf[isuf], d_cor[index++] );
	
	return 0;
}
int winOpt::outBestLag3Col(const std::string& filename) {
	outPlainFile out(filename);
	int index = 0;
	for(int ipre = 0; ipre<d_lenPre; ++ipre)
		for(int isuf = 0; isuf<d_lenSuf; ++isuf)
			out.outCol3f(d_pre[ipre], d_suf[isuf], d_bestlag[index++] );
	
	return 0;
}
int winOpt::outPre(const std::string& filename) {
	outPlainFile out(filename);
	for(int ipre = 0; ipre<d_lenPre; ++ipre)
		out.outColf(d_pre[ipre]);
	
	return 0;
}
int winOpt::outSuf(const std::string& filename) {
	outPlainFile out(filename);
	for(int isuf = 0; isuf<d_lenSuf; ++isuf)
		out.outColf(d_suf[isuf]);
	
	return 0;
}
int winOpt::outCoef(const std::string& filename) {
	outPlainFile out(filename);
	for(int i=0;i<d_lenOpt; ++i)
		out.outColf(d_coef[i]);
	
	return 0;
}
int winOpt::outCor(const std::string& filename) {
	outPlainFile out(filename);
	for(int i=0;i<d_lenOpt; ++i)
		out.outColf(d_cor[i]);
	
	return 0;
}
int winOpt::outBestLag(const std::string& filename) {
	outPlainFile out(filename);
	for(int i=0;i<d_lenOpt; ++i)
		out.outColf(d_bestlag[i]);
	
	return 0;
}