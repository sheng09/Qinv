#include "test.hh"
#include "atten.hh"
#include <cmath>
#include "io_format.hh"

attenTrace::attenTrace(const std::string& filename, int tmarker, float t1, float t2, int fftSize_Time) :
		d_green(0), d_phasegreen(0), d_absgreen(0), d_qinv(0), d_ampRatio(0) 
{
	init(filename, tmarker, t1, t2, fftSize_Time);
}
int attenTrace::init(const std::string& filename, int tmarker, float t1, float t2, int fftSize_Time) {
	d_sac.initSac(filename, tmarker, t1, t2);
	fftSize_Time = deltaAlign(d_sac.getDelta(), fftSize_Time);
	d_src.init(fftSize_Time);
	d_src.setDelta(d_sac.getDelta() );
	d_rec.init(fftSize_Time);
	d_rec.setDelta(d_sac.getDelta() );
	d_sxy.init(fftSize_Time);
	d_sxy.setDelta(d_sac.getDelta() );           //
	d_fmax = 1.0 / d_sac.getDelta();
	d_df   = d_fmax / fftSize_Time;
	//
	setSize(fftSize_Time / 2 + 1); // if the sample rate is 100Hz, then the interval of [0,50]Hz is valid
	alloc();
	return 0;
}
int attenTrace::deltaAlign(float delta, int fftSize_Time) {
	#define DELTA 0.02
	#define LENMUL 4
	static int MUL[LENMUL] = {1,2,4,8};
	/*
	0.0025s   2048 points
	0.005s    1024 points
	0.01s     512 points
	0.02s     256 points
	*/
	int N = round(DELTA/delta);
	int i;
	for(i = 0; i < LENMUL && N != MUL[i] ; ++i)
		;
	if( i == LENMUL) {
		exit(-1);
		fprintf(stderr, "Err: sampling time invertal is not multuples of 0.005s. (%f), %s\n", delta, d_sac.getSacName().c_str() );
	}
	#undef DELTA
	#undef LENMUL
	return (fftSize_Time *N);
}
attenTrace::~attenTrace() {
	//DPRINT("this: %p attenTrace::~attenTrace()", this);
	//if(d_qinv) free(d_qinv);
	//if(d_green) free(d_green);
	//if(d_absgreen) free(d_absgreen);
	//if(d_phasegreen) free(d_phasegreen);
	//if(d_ampRatio) free(d_ampRatio);
	//DPRINT("this: %p  End of attenTrace::~attenTrace()", this);
	//DPRINT("atten.d_sac: %p", &d_sac);
	//DPRINT("atten.d_src: %p", &d_src);
	//DPRINT("atten.d_rec: %p", &d_rec);
	//DPRINT("atten.d_sxy: %p", &d_sxy);

	if(!d_qinv.empty())        d_qinv.clear();
	if(!d_green.empty())       d_green.clear();
	if(!d_absgreen.empty())    d_absgreen.clear();
	if(!d_phasegreen.empty())  d_phasegreen.clear();
	if(!d_ampRatio.empty())    d_ampRatio.clear();
}
int attenTrace::alloc() {
	//int N = d_len;
	d_qinv.resize(d_len);
	d_green.resize(d_len*2);
	d_absgreen.resize(d_len);
	d_phasegreen.resize(d_len);
	d_ampRatio.resize(d_len);

	//d_qinv       = (float*)          (calloc(d_len    +4, sizeof(float)) );
	//d_green      = (fftwf_complex *) (calloc(d_len*2  +4, sizeof(float)) );
	//d_absgreen   = (float*)          (calloc(d_len    +4, sizeof(float)) );
	//d_phasegreen = (float*)          (calloc(d_len    +4, sizeof(float)) );
	//d_ampRatio   = (float*)          (calloc(d_len    +4, sizeof(float)) );
	//return d_qinv;
	return 0;
}
int attenTrace::make(int i1, int i2, int il, float timelength) {
	int l = i2 - i1 + 1;
	float *src = d_sac.getData();
	float delta = d_sac.getDelta();
	d_lagtime = il * delta;
	d_src.assign(src+i1, l);
	d_rec.assign(src+i1+il, l);
	d_src.setUseSize(l);
	d_rec.setUseSize(l);

	d_src.setStart(d_sac.getStart() + delta*i1 );
	d_rec.setStart(d_sac.getStart() + delta*i2 );
	int N = round(timelength/d_sac.getDelta() );
	d_src.taper(N);
	d_rec.taper(N);
	return 0;
}
float attenTrace::cor() {
	float *tr1 = d_src.getData();
	float *tr2 = d_rec.getData();
	float v1 = 0.0, v2 =0.0, v12 = 0.0;
	for(int i=0;i<d_src.UseSize(); ++i) {
		v12 += (tr1[i]*tr2[i]);
		v1  += (tr1[i]*tr1[i]);
		v2  += (tr2[i]*tr2[i]);
	}
	v12 /= sqrt(v1*v2);
	return v12;
}
int attenTrace::runSpec() {
	//DPRINT("Part [d_src.dft]");
	d_src.dft();
	//DPRINT("Part [d_rec.dft]");
	d_rec.dft();
	//DPRINT("Part [d_sxy.cross]");
	d_sxy.cross(d_src, d_rec); // ?
	return 0;
}
int attenTrace::runPow() {
	d_src.power();
	d_rec.power();
	d_sxy.power2(); // ?
	return 0;
}
int attenTrace::smoothPow(float freqWith) {
	int L = ceil(freqWith / d_df);
	L = (L / 2) * 2 + 1;
	//DPRINT("smooth pow in attenTrace: %d", L);
	d_src.smoothPowN(L); // just smooth the d_pow, while d_spec do not change.
	d_rec.smoothPowN(L);
	d_sxy.smoothCrossPowN(L);
	return 0;
}
int attenTrace::runAmpRatio() {
	float *px = d_src.getPow();
	float *py = d_rec.getPow();
	for(int i =0; i< size(); ++i)
		d_ampRatio[i] = sqrt(py[i] / px[i]);
	return 0;
}
float attenTrace::coef(float f1, float f2) {
	int i1 = floor(f1/d_df);
	int i2 = ceil(f2/d_df);
	float d_coef = 0.0;
	float *sxy2 = d_sxy.getPow2();
	float *sxx  = d_src.getPow();
	float *syy  = d_rec.getPow();
	for(int i = i1; i<=i2; ++i) {
		d_coef += sxy2[i] / ( sxx[i]*syy[i] ) * sqrtf(sxx[i]);
		//printf("%d sxy:%f sxx:%f syy:%f\n", i, sxy2[i], sxx[i], syy[i]);
	}
	return d_coef;
}
int attenTrace::green() {
	/*     Fx conj(Fy)     conj(Fy)
	  G = ------------- = ----------
	       Fx conj(Fx)     conj(Fx)
	*/
	fftwf_complex* fx = d_src.getSpec();
	fftwf_complex* fy = d_rec.getSpec();
	float scale;
	fftwf_complex* __green = getGreen();
	for(int i=0;i<size();++i) {
		scale = 1.0/(fx[i][0]*fx[i][0] + fx[i][1]*fx[i][1]);
		__green[i][0] = (fy[i][0]*fx[i][0] + fy[i][1]*fx[i][1])*scale;
		__green[i][1] = (fy[i][0]*fx[i][1] - fy[i][1]*fx[i][0])*scale;
		d_absgreen[i] = sqrt( __green[i][0]*__green[i][0]+__green[i][1]*__green[i][1] );
		d_phasegreen[i] = atan2f(__green[i][1], __green[i][0]) * 180.0/PI;
	}
	return 0;
}
int attenTrace::qinv() {
	d_qinv[0] = 0.0;
	for(int i=1;i<size(); ++i) {
		d_qinv[i] = -logf(d_absgreen[i]) / (PI*d_lagtime*i*d_df);
	}
	return 0;
}
int attenTrace::outGreen(const std::string& filename){
	outPlainFile out(filename);
	fftwf_complex* __green = getGreen();
	for(int i=0; i<size(); ++i) {
		out.outCol2f(__green[i][0], __green[i][1]);
	}

	return 0;
}
int attenTrace::outGreenPhase(const std::string& filename){
	outPlainFile out(filename);
	for(int i=0; i<size(); ++i) {
		out.outColf(d_phasegreen[i]);
	}

	return 0;
}
int attenTrace::outGreenAmp(const std::string& filename){
	outPlainFile out(filename);
	for(int i=0; i<size(); ++i) {
		out.outColf(d_absgreen[i]);
	}

	return 0;
}
int attenTrace::outQinv(const std::string& filename){
	outPlainFile out(filename);
	for(int i=0; i<size(); ++i) {
		out.outColf(d_qinv[i]);
	}

	return 0;
}
int attenTrace::outFreqGreen(const std::string& filename){
	outPlainFile out(filename);
	fftwf_complex* __green = getGreen();
	for(int i=0; i<size(); ++i) {
		out.outCol3f(i*d_df, __green[i][0], __green[i][1]);
	}

	return 0;
}
int attenTrace::outFreqGreenPhase(const std::string& filename){
	outPlainFile out(filename);
	for(int i=0; i<size(); ++i) {
		out.outCol2f(i*d_df, d_phasegreen[i]);
	}

	return 0;
}
int attenTrace::outFreqGreenAmp(const std::string& filename){
	outPlainFile out(filename);
	for(int i=0; i<size(); ++i) {
		out.outCol2f(i*d_df, d_absgreen[i]);
	}

	return 0;
}
int attenTrace::outFreqQinv(const std::string& filename){
	outPlainFile out(filename);
	for(int i=0; i<size(); ++i) {
		out.outCol2f(i*d_df, d_qinv[i]);
	}

	return 0;
}
int attenTrace::outFreqAmpratio(const std::string& filename){
	outPlainFile out(filename);
	for(int i=0; i<size(); ++i) {
		out.outCol2f(i*d_df, d_ampRatio[i]);
	}

	return 0;
}
int attenTrace::outAmpratio(const std::string& filename){
	outPlainFile out(filename);
	for(int i=0; i<size(); ++i) {
		out.outColf(d_ampRatio[i]);
	}

	return 0;
}