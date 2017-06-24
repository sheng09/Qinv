#include "tracef1d.hh"
#include <fftw3.h>
#include <string>
#include <vector>

#ifndef __ATTEN___TRACE__SPEC__
#define __ATTEN___TRACE__SPEC__



float pazen_one(float x); // 0<= x <= 0.5
float* parzen_win(int L); // L must be an odd number

class tracef1d_Spec : public tracef1d
{
public:
	tracef1d_Spec() :d_spec(0), d_pow(0) {} //useless
	tracef1d_Spec(tracef1d &src); // just allocate the memory
	tracef1d_Spec(float *src, int Tlen, float b, float delta);    // result length is 2^N
	tracef1d_Spec(int Tlen);                                      // result length is 2^N
	virtual int    init(float *src, int Tlen, float b, float delta); // result length is 2^N
	virtual int    init(int Tlen); // only allocate  memory          // result length is 2^N
	virtual int    alloc(int fftsize); //fftsize is the length in time domain
	//allocate fftsize*sizeof(float) mem for time domain data, 
	// and (fftsize/2+1)*sizeof(fftwf_float) mem for frequency domain data
	// and (fftsize/2+1)*sizeof(float) mem for power
	virtual      ~tracef1d_Spec();
	float        getdf() {return d_df; }
	float        getfmax() {return d_fmax; }
	virtual void setDelta(float delta) { tracef1d::setDelta(delta); d_fmax = 1.0/delta; d_df = d_fmax / sizeT() ;}

	int dft();
	int power(); // Fx * conj(Fx)
	int mul(float scale); // spec *= scale; pow *= (scale^2)
	//Only smooth the middle trace, while keep the two ends.
	int smoothPow3(); // d_spec do not change, and d_pow is destroyed
	int smoothPowN(int L); //d_spec do not change, and d_pow is destroyed
	int sizeT() { return tracef1d::size(); }
	int sizeF() { return d_specLen;        }
	int setSizeT(int fftsize)  { tracef1d::setSize(fftsize)     ;  d_specLen = fftsize/2+1; return 0; } // d_len is twice of d_specLen
	int setSizeF(int fftsize2) { tracef1d::setSize(fftsize2*2-2);  d_specLen = fftsize2;    return 0; }
	virtual float*         getPow()  { return d_pow.data(); } // Fx * conj(Fx)
	virtual fftwf_complex* getSpec() { return (fftwf_complex*) (d_spec.data()); }
	virtual std::vector<float>& getPowVec() { return d_pow; }
	virtual std::vector<float>& getSpecVec() { return d_spec; }

	int outPow(     const std::string& filename); // Fx * conj(Fx)
	int outSpec(    const std::string& filename);
	int outFreqPow( const std::string& filename); // Fx * conj(Fx)
	int outFreqSpec(const std::string& filename);
private:
	virtual int size() {return 0;}
	virtual int setSize(int l) { return 0; }
private:
	int            d_specLen;

	//fftwf_complex  *d_spec;
	//float          *d_pow;
	std::vector<float> d_spec; // its size is twice of the d_pow's
	std::vector<float> d_pow;
	float d_df; // frequency interval
	float d_fmax; // max frequency
};

inline int get2Len(int N) {
	int i=2;
	while(i<N)
		i <<= 1;
	return i;
}

#endif