#include "tracef1d_Spec.hh"
#include <fftw3.h>
#include <vector>

#ifndef __ATTEN__CROSS_SPEC__
#define __ATTEN__CROSS_SPEC__


class crossSpec  : public tracef1d_Spec
{
public:
	crossSpec() : d_crossSpec(0), d_crossPow2(0) {}
	crossSpec(int fftSize_Time);
	crossSpec(tracef1d_Spec &sp1, tracef1d_Spec &sp2);
	virtual int init(int l);
	virtual int init(tracef1d_Spec &sp1, tracef1d_Spec &sp2);
	virtual ~crossSpec();
	virtual int alloc(int fftSize_Time); //fftSize_Time is the length in time domain

	virtual float*         getPow2()  { return d_crossPow2.data(); } // Fx * conj(Fx)
	virtual fftwf_complex* getSpec() { return (fftwf_complex*) (d_crossSpec.data()); }
	virtual std::vector<float>& getPowVec() { return d_crossPow2; }
	virtual std::vector<float>& getSpecVec() { return d_crossSpec; }


	fftwf_complex* cross(tracef1d_Spec &sp1, tracef1d_Spec &sp2);
	float* power2();
	int smoothCrossPow3();
	int smoothCrossPowN(int L);
	int outPow2(         const std::string& filename); // Fx * Fy * conj(Fy* Fx)
	int outFreqPow2(     const std::string& filename);
	int outCrossSpec(    const std::string& filename); // Fx * conj(Fy)
	int outFreqCrossSpec(const std::string& filename);
private:
	virtual float* getPow()  { return NULL; }
	//fftwf_complex *d_crossSpec; // Fx * conj(Fy)
	//float         *d_crossPow2; // Fx * Fy * conj(Fy* Fx)
	std::vector<float> d_crossSpec;
	std::vector<float> d_crossPow2;
};

#endif