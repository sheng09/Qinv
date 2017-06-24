#include "tracef1d_Spec.hh"
#include "tracef1d.hh"
#include <fftw3.h>
#include "test.hh"
#include <cstring>
#include "io_format.hh"

tracef1d_Spec::tracef1d_Spec(tracef1d &src) {
	init(src.getData(), src.size(), src.getStart(), src.getDelta() );
}
tracef1d_Spec::tracef1d_Spec(int Tlen) {
	DPRINT("this: %p, tracef1d_Spec::tracef1d_Spec(int %d)", this, Tlen);
	init(Tlen);
	//setFreq
}
int tracef1d_Spec::init(int Tlen) {
	DPRINT("this: %p, tracef1d_Spec::init(int %d)", this, Tlen);
	int fftsize = get2Len(Tlen);
	alloc(fftsize);
	setSizeT(fftsize);
	return 0;
}
tracef1d_Spec::tracef1d_Spec(float *src, int ll, float b, float delta) {
	init(src, ll, b, delta);
}
int tracef1d_Spec::init(float *src, int Tlen, float b, float delta) {
	DPRINT("this: %p, tracef1d_Spec::init(int %p, %d, %f, %f)", this, 
		src, Tlen, b, delta);
	int fftsize = get2Len(Tlen);
	alloc(fftsize);
	setSizeT(fftsize);
	assign(src, Tlen); // NOTE, it is Tlen, but not l
	setStart(b);
	setDelta(delta);
	return 0;
}
/*
	fftsize is the length in time domain
*/
int tracef1d_Spec::alloc(int fftsize) {
	d_pow.resize( fftsize/2+1);
	d_spec.resize(fftsize+2);
	std::fill(d_pow.begin(),  d_pow.end(), 0);
	std::fill(d_spec.begin(), d_spec.end(), 0);
	//DPRINT("this: %p, tracef1d_Spec::alloc(int %d)", this, fftsize);
	//d_spec = (fftwf_complex*) (calloc(fftsize+2 + 8, sizeof(float)) );
	//DPRINT("this: %p, tracef1d_Spec::d_spec: %p->%p", this, d_spec, d_spec+fftsize+2);
	//d_pow  = (float*) (calloc( fftsize/2+1 + 4, sizeof(float)) );
	//DPRINT("this: %p, tracef1d_Spec::d_pow: %p->%p", this, d_pow, d_pow+fftsize/2+1);
	return tracef1d::alloc(fftsize);
}

tracef1d_Spec::~tracef1d_Spec() {
	if( !d_pow.empty() ) d_pow.clear();
	if( !d_spec.empty() ) d_spec.clear();
	//DPRINT("this: %p tracef1d_Spec::~tracef1d_Spec()", this);
	//DPRINT("freeing %p", d_spec);
	//if(d_spec) free(d_spec);
	//DPRINT("freed %p", d_spec);
	//DPRINT("freeing %p", d_pow);
	//if(d_pow) free(d_pow);
	//DPRINT("freed %p", d_pow);
	//DPRINT("this: %p tracef1d_Spec::~tracef1d_Spec()", this);
}

int tracef1d_Spec::dft() {
	fftwf_plan p;
	p = fftwf_plan_dft_r2c_1d(sizeT(), getData(), getSpec(), FFTW_ESTIMATE);
	fftwf_execute(p);
	fftwf_destroy_plan(p);
	return 0;
}
int tracef1d_Spec::power() {
	int l2 = sizeT();
	fftwf_complex* spec = getSpec();
	l2 *= l2;
	int N = sizeF();
	for(int i =0; i<N; ++i) {
		d_pow[i] = spec[i][0]/l2*spec[i][0] + spec[i][1]/l2*spec[i][1];
	}
	return 0;
}
int tracef1d_Spec::mul(float scale) {
	float scale2 =scale * scale;
	int N = sizeF();
	fftwf_complex* spec = getSpec();
	for(int i =0; i<N; ++i) {
		d_pow[i] *= scale2;
		spec[i][0] *= scale;
		spec[i][1] *= scale;
	}
	return 0;
}
int tracef1d_Spec::smoothPow3() {
	int N = sizeF()-1;
	for(int i =1; i<N; ++i) {
		d_pow[i] = d_pow[i-1] * 0.0645 +
					d_pow[i]  * 0.8710 +
					d_pow[i+1]* 0.0645;
	}
	return 0;
}
int tracef1d_Spec::smoothPowN(int L) { // L must be an odd
	const float *pwin = parzen_win(L);
	float v;
	int L2 = L/2;
	int N = sizeF() - L2;
	for(int i= L2; i<N; ++i) {
		v = 0.0;
		int iii = i-L2;
		for(int jdx = 0; jdx<L; ++jdx) {
			//DPRINT("index:%d += [%d]x%f", i, iii+jdx, pwin[jdx]);
			v += d_pow[iii+jdx] * pwin[jdx];
		}
		d_pow[i] = v;
	}
	return 0;
}

int tracef1d_Spec::outPow(const std::string& filename) {
	int N = sizeF();
	outPlainFile out(filename);
	for(int i=0;i<N;++i)
		out.outColf(d_pow[i]);
	return 0;
}
int tracef1d_Spec::outSpec(const std::string& filename) {
	int N = sizeF();
	outPlainFile out(filename);
	fftwf_complex* spec = getSpec();
	for(int i=0;i<N;++i)
		out.outCol2f( spec[i][0], spec[i][1]);
	return 0;
}
int tracef1d_Spec::outFreqPow(const std::string& filename) {
	int N = sizeF();
	outPlainFile out(filename);
	for(int i=0;i<N;++i)
		out.outCol2f(d_df*i, d_pow[i]);
	return 0;
}
int tracef1d_Spec::outFreqSpec(const std::string& filename) {
	int N = sizeF();
	outPlainFile out(filename);
	fftwf_complex* spec = getSpec();
	for(int i=0;i<N;++i)
		out.outCol3f(d_df*i, spec[i][0], spec[i][1]);
	return 0;
}

float pazen_one(float x) { // 0<= x <= 0.5
	//Reference
	// https://reference.wolfram.com/language/ref/ParzenWindow.html
	float x2 = x*x;
	float x3 = x*x2;
	float v;
	if(x<=0.25)	    return 48.0*x3 -24*x2 + 1.0;
	else if(x<=0.5) {
		v = 2.0*x-1.0;
		return -2*v*v*v;
	} else return 0.0;
}
float* parzen_win(int L) {
	static int _size = 0;
	static float _win[8192];
	//DPRINT("L:%d size:%d", L, _size);
	if(_size != L) {
		int N = L - 1;
		int mid = N / 2;
		_win[mid] = 1.0;
		float sum = 1.0;
		for(int i=1;i<=mid;++i) {
			_win[mid+i] = pazen_one( ((float)(i))/L);
			_win[mid-i] = _win[mid+i];
			sum += (2.0*_win[mid+i]);
		}
		for(int i=0;i<L;++i) {
			//DPRINT("%d(%d) %f", i, L, _win[i]);
			_win[i] /=sum;
		}
		_size = L;
	}
	return _win;
}