#include "atten.hh"
#include "winOpt.hh"
#include <string>
#include <cmath>
#include "test.hh"

#ifndef __ATTEN__Q__JOB__
#define __ATTEN__Q__JOB__

#define LEN 8192

class qjob
{
public:
	//qjob() {}
	qjob() : d_fft_size(0) {};
	qjob(const std::string& sacfile, int tmark, float t1, float t2);
	qjob(const std::string& sacfile, int tmark, float t1, float t2, float winB, float winE, float dlag);
	virtual ~qjob() { DPRINT("this: %p qjob::~qjob()", this); };
	virtual int initQjob(const std::string& sacfile, int tmark, float t1, float t2, float winB, float winE, float dlag);
	virtual int initQjob(const std::string& sacfile, int tmark, float t1, float t2);
	virtual int run() { return 0;};
	int setFFTsize(int l) { return d_fft_size = l; } // set the fft size, must be 2^N

public:
	int d_fft_size;
	std::string d_sacfilename;
	attenTrace  d_attentrace;
	winOpt      d_winopt;

	/*
	                 t5              t6
	                 |               |
	 b------+--------x---------+-----x------
	        |        |         |
            |<------>|<------->|
             -d_winB   d_winE
	*/
	//float d_winB; //
	//float d_winE; //
	//float d_DeltaLagtime; // the lag length is [t6-t5-d_DeltaLagtime, t6-t5+d_DeltaLagtime]
};

#endif