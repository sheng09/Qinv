
#include "tracef1d.hh"
#include "tracef1d_Spec.hh"
#include "cross_Spec.hh"
#include <string>
#include <vector>

#ifndef __ATTEN__ATTEN__
#define __ATTEN__ATTEN__

class attenTrace
{
public:
	attenTrace() : d_green(0), d_phasegreen(0), d_absgreen(0), d_qinv(0), d_ampRatio(0) {}
	attenTrace(const std::string& filename, int tmarker, float t1, float t2, int fftSize_Time);
	virtual ~attenTrace();
	virtual int init(const std::string& filename, int tmarker, float t1, float t2, int fftSize_Time);
	int alloc();
	int deltaAlign(float delta, int fftSize_Time);

	int size() {return d_len; }
	int setSize(int fftsize_Freq) { return d_len = fftsize_Freq; }
	int make(int i1, int i2, int il, float timelength);
	float cor();  // cross coefficient
	int runSpec(); // calculate spectrum and cross spectrum
	int runPow();  // calculate the POWER and cross POWER, and Amplitude ratio
	int smoothPow(float freqWidth); //destroy the pow spectrum for d_src, d_rec, d_sxy
	int runAmpRatio(); //amplitude ratio (sqrt of the ratio of the smoothed power)
	float coef(float f1, float f2); // the fukushima coef
	int green(); // the Hs(f)
	int qinv();  // the Q_inv(f)

	int outGreen(     const std::string& );
	int outGreenPhase(const std::string& );
	int outGreenAmp(  const std::string& );
	int outQinv(      const std::string& );

	int outFreqGreen(     const std::string& );
	int outFreqGreenPhase(const std::string& );
	int outFreqGreenAmp(  const std::string& );
	int outFreqQinv(      const std::string& );
	int outFreqAmpratio(  const std::string& );
	int outAmpratio(      const std::string& ); //amplitude ratio (sqrt of the ratio of the smoothed power)

	fftwf_complex* getGreen() { return (fftwf_complex*) (d_green.data()); }
public:
	tracef1d      d_sac;
	tracef1d_Spec d_src;
	tracef1d_Spec d_rec;
	crossSpec     d_sxy;

	int            d_len;
	//fftwf_complex *d_green;      // conj(Fy) / conj(Fx)
	//float         *d_phasegreen; // in Degree -180, 180
	//float         *d_absgreen;
	//float         *d_qinv;
	//float         *d_ampRatio; // smoothamp(fy) / smoothamp(fx)

	std::vector<float> d_green;           // conj(Fy) / conj(Fx)
	std::vector<float> d_phasegreen;      // in Degree -180, 180
	std::vector<float> d_absgreen;
	std::vector<float> d_qinv;
	std::vector<float> d_ampRatio;        // smoothamp(fy) / smoothamp(fx)

	// from the test, Sheng find that ampratio do not change before and after smooth operation

	float d_df, d_fmax;
	//float d_coef;

	int d_idx1, d_idx2, d_ilag;
	float d_lagtime;
};

#endif