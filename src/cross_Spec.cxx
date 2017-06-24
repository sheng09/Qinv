#include "cross_Spec.hh"
#include "test.hh"
#include "io_format.hh"

crossSpec::crossSpec(int fftSize_Time) : d_crossSpec(0), d_crossPow2(0) {
	DPRINT("this: %p, crossSpec::crossSpec(%d)", this, fftSize_Time);
	init(fftSize_Time);
}
int crossSpec::init(int fftSize_Time) {
	DPRINT("this: %p, crossSpec::init(%d)", this, fftSize_Time);
	alloc(fftSize_Time);
	return 0;
}
crossSpec::crossSpec(tracef1d_Spec &sp1, tracef1d_Spec &sp2) : d_crossSpec(0), d_crossPow2(0) {
	init(sp1, sp2);
}

int crossSpec::init(tracef1d_Spec &sp1, tracef1d_Spec &sp2) {
	alloc(sp1.sizeT() );
	cross(sp1, sp2);
	return 0;
}
crossSpec::~crossSpec() {
	if( !d_crossSpec.empty() ) d_crossSpec.clear();
	if( !d_crossPow2.empty() ) d_crossPow2.clear();
	//DPRINT("this %p crossSpec::~crossSpec()", this);
	//DPRINT("freeing ... %p", d_crossSpec);
	//if(d_crossSpec) free(d_crossSpec);
	//DPRINT("freed %p", d_crossSpec);
	//DPRINT("freeing ... %p", d_crossPow2);
	//if(d_crossPow2) free(d_crossPow2);
	//DPRINT("freed %p", d_crossPow2);
	//DPRINT("this %p End of crossSpec::~crossSpec()", this);
}
/*
	fftSize_Time is the length in time domain
*/
int crossSpec::alloc(int fftSize_Time) {
	DPRINT("this: %p, crossSpec::alloc(%d)", this, fftSize_Time);
	setSizeT(fftSize_Time);
	d_crossSpec.resize(fftSize_Time+2);
	std::fill( d_crossSpec.begin(), d_crossSpec.end(), 0 );
	d_crossPow2.resize(fftSize_Time/2+1);
	std::fill( d_crossPow2.begin(), d_crossPow2.end(), 0 );
	//d_crossSpec = (fftwf_complex*) (calloc(fftSize_Time+2, sizeof(float)) );
	//d_crossPow2 = (float*) (calloc(fftSize_Time/2+1, sizeof(float)) );
	return 0;
}
fftwf_complex* crossSpec::cross(tracef1d_Spec &sp1, tracef1d_Spec &sp2) {
	fftwf_complex *tr1 = sp1.getSpec();
	fftwf_complex *tr2 = sp2.getSpec();
	//float l2 = sp1.sizeT() * sp2.sizeT();
	fftwf_complex* cSpec = this->getSpec();
	//l2 *= l2;
	//fprintf(stderr, "tr1  sizeT: %d, sizeF: %d \n",sp1.sizeT(), sp1.sizeF() );
	//fprintf(stderr, "this sizeT: %d, sizeF: %d \n",    sizeT(),     sizeF() );
	for(int i=0;i<sp1.sizeF();++i) {
		cSpec[i][0] = tr1[i][0]*tr2[i][0] + tr1[i][1]*tr2[i][1];
		cSpec[i][1] = tr1[i][1]*tr2[i][0] - tr1[i][0]*tr2[i][1];
		//d_crossPow2[i] = d_crossSpec[i][0]*d_crossSpec[i][0] + d_crossSpec[i][1]*d_crossSpec[i][1];
	}
	return cSpec;
}
float* crossSpec::power2() {
	float l4 = sizeT();
	l4 *= l4;
	l4 *= l4;
	fftwf_complex* cSpec = this->getSpec();
	float* __pow2 = getPow2();
	for(int i=0;i<sizeF();++i) {
		__pow2[i] = cSpec[i][0]*cSpec[i][0]/l4 + cSpec[i][1]*cSpec[i][1]/l4; // NOTE, here is len^4
	}
	return __pow2;
}
int crossSpec::smoothCrossPow3() {
	float* __pow2 = getPow2();
	for(int i =1; i<sizeF()-1; ++i) {
        __pow2[i] = __pow2[i-1] * 0.0645 +
                    __pow2[i]  * 0.8710 +
                    __pow2[i+1]* 0.0645;
    }
    return 0;
}
int crossSpec::smoothCrossPowN(int L) {
	const float *pwin = parzen_win(L);
	float v;
	int L2 = L/2;
	float* __pow2 = getPow2();
	for(int i= L2; i<sizeF()-L2; ++i) {
		v = 0.0;
		int iii = i-L2;
		for(int jdx = 0; jdx<L; ++jdx) {
			//DPRINT("index:%d += [%d]x%f", i, iii+jdx, pwin[jdx]);
			v += __pow2[iii+jdx] * pwin[jdx];
		}
		__pow2[i] = v;
	}
	return 0;
}

int crossSpec::outPow2(const std::string& filename) {
	outPlainFile out(filename);
	float* __pow2 = getPow2();
	for(int i=0;i<sizeF();++i) {
		out.outColf(__pow2[i]);
	}
	return 0;
}
int crossSpec::outFreqPow2(const std::string& filename) {
	float* __pow2 = getPow2();
	outPlainFile out(filename);
	float df = getdf();
	for(int i=0;i<sizeF();++i) {
		out.outCol2f(df*i, __pow2[i]);
	}
	return 0;
}
int crossSpec::outCrossSpec(const std::string& filename) {
	outPlainFile out(filename);
	fftwf_complex* cSpec = this->getSpec();
	for(int i=0;i<sizeF();++i) {
		out.outCol2f(cSpec[i][0], cSpec[i][1]);
	}
	return 0;
}
int crossSpec::outFreqCrossSpec(const std::string& filename) {
	outPlainFile out(filename);
	fftwf_complex* cSpec = this->getSpec();
	float df = getdf();
	for(int i=0;i<sizeF();++i) {
		out.outCol3f(i*df, cSpec[i][0], cSpec[i][1]);
	}
	return 0;
}
