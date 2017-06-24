#include "tracef1d.hh"
#include "sac.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "io_format.hh"

#include "test.hh"

tracef1d::tracef1d(int len, float delta, float start) {
	initMem(len, delta, start);
}
/*
	Init without trace data.
*/
int tracef1d::initMem(int len, float delta, float start) {
	alloc(len);
	setDelta(delta);
	setStart(start);
	setSize(len);
	return 0;
}
tracef1d::tracef1d(const std::string& filename, int tmark, float t1, float t2) {
	initSac(filename, tmark, t1, t2);
}
tracef1d::tracef1d(const std::string& filename) {
	initSac(filename);
}
/*
	Init from the sac file.
*/
int tracef1d::initSac(const std::string& filename, int tmark, float t1, float t2) {
	d_sacname = filename;
	//SACHEAD hdr;
	float *ptr = read_sac2(filename.c_str(), &d_sachdr, tmark, t1, t2);
	d_data.resize(d_sachdr.npts);
	d_data.assign(ptr, ptr+d_sachdr.npts);
	std::free(ptr); //

	setSize(d_sachdr.npts);
	setUseSize(d_sachdr.npts);

	setDelta(d_sachdr.delta);
	setTinc(d_sachdr.t5);
	setTref(d_sachdr.t6);
	setStart(d_sachdr.b);
	return 0;
}

int tracef1d::initSac(const std::string& filename) {
	d_sacname = filename;
	//SACHEAD hdr;
	float *ptr = read_sac(filename.c_str(), &d_sachdr);
	d_data.resize(d_sachdr.npts);
	d_data.assign(ptr, ptr+d_sachdr.npts);
	std::free(ptr); //

	setSize(d_sachdr.npts);
	setUseSize(d_sachdr.npts);

	setDelta(d_sachdr.delta);
	setTinc(d_sachdr.t5);
	setTref(d_sachdr.t6);
	setStart(d_sachdr.b);
	return 0;
}
/*
	Allocate mem for d_data.
*/
int tracef1d::alloc(int l) {
	//DPRINT("this %p, tracef1d::alloc(%d)", this, l);
	//d_data = (float*) (calloc(l+4, sizeof(float)));
	//DPRINT("this %p, tracef1d::d_data: %p->%p", this, d_data, d_data+l);
	d_data.resize(l);
	std::fill(d_data.begin(), d_data.end(), 0);
	setSize(l);
	setUseSize(l);
	return 0;
}
tracef1d::~tracef1d() {
	DPRINT("this: %p tracef1d::~tracef1d()", this);
	del();
	DPRINT("this: %p End of tracef1d::~tracef1d()", this);
}
void tracef1d::assign(float *src, int l) {
	//DPRINT("memset [%p %p)", getData(), getData() + sizeT() );
	//memset(getData(), 0, sizeT()*sizeof(float) );
	//DPRINT("this: %p, from: [%p %p) to: [%p %p)", this, src, src + l, getData(), getData()+l );
	//memcpy(getData(), src, l*sizeof(float) );
	std::fill(d_data.begin(), d_data.end(), 0.0);
	d_data.assign(src, src+l);
}
void tracef1d::assign(float *srcbegin, float *srcend) {
	std::fill(d_data.begin(), d_data.end(), 0.0);
	d_data.assign(srcbegin, srcend);
}
float tracef1d::floorCrossZero(float _t, int *idx) {
	int i = roundIdx(_t);
	while(d_data[i] == 0.0)
		--i;
	float v=d_data[i];
	for(; i>=0 && v*d_data[i]>0; --i)
		;
	*idx = i;
	return i*d_delta + d_start;
}
float tracef1d::ceilCrossZero(float _t, int *idx) {
	int i = roundIdx(_t);
	while(d_data[i] == 0.0)
		--i;
	float v=d_data[i];
	for(; i<d_useful && v*d_data[i]>0; ++i)
		;
	*idx = i;
	return i*d_delta + d_start;
}
int tracef1d::taper(int N) {
	if( N >= d_useful/2 ) {
		fprintf(stderr, "Warnning: the taper's length (%d) is larger then the half of the SAC trace's (%d)\n", N, d_useful/2 );
		N = d_useful/2-1;
		N = N>=0? N:0;
		fprintf(stderr, "          resize the taper length to (%d)\n", N);
	}
	float *_win = taperhanTrace(N);
	for(int i = 0; i<N;++i) {
		d_data[i]         *= _win[i];
		d_data[d_useful-i-1] *= _win[i];
	}
	return 0;
}
int tracef1d::outTrace(const std::string& filename) {
	outPlainFile out(filename);
	for(int i=0;i<d_useful;++i)
		out.outColf(d_data[i]);
	return 0;
}
int tracef1d::outTimeAxis(const std::string& filename) {
	float t0 = d_start;
	outPlainFile out(filename);
	for(int i=0;i<d_useful;++i, t0 += d_delta)
		out.outColf(t0);
	return 0;
}
int tracef1d::outTimeTrace(const std::string& filename){
	float t0 = d_start;
	outPlainFile out(filename);
	for(int i=0;i<d_useful;++i, t0 += d_delta)
		out.outCol2f(t0, d_data[i]);
	return 0;
}
float* taperhanTrace(int N) {
	static float _win[8192];
	static int size = 0;
	static int yes = 1;
	if(size != N || yes ) {
		for(int i = 0; i<N;++i) {
			_win[i] = TAPER_HAN(N, i);
		}
		size = N;
		yes = 0;
	}
	return _win;
}
float* taperhanTraceOrd2(int N) {
	static float _win[8192];
	static int size = 0;
	static int yes = 1;
	if(size != N || yes ) {
		for(int i = 0; i<N;++i) {
			_win[i] = TAPER_HAN(N, i);
			_win[i] *= _win[i];
		}
		size = N;
		yes = 0;
	}
	return _win;
}
