#include "q_job.hh"

#ifndef __ATTEN__JOB__FIND__WIN__
#define __ATTEN__JOB__FIND__WIN__

class jobWin : public qjob
{
public:
	jobWin() : d_fixLagTime(false), d_minGap(0.0), d_maxLength(99999.9) {}
	jobWin(int ac, char * av[]);
	jobWin(const std::string& sacfile, int tmark, float t1, float t2, float winB1, float winE2, float dlag);
	~jobWin() { DPRINT("this: %p jobWin::~jobWin()", this); }

	virtual int init(const std::string& sacfile, int tmark, float t1, float t2, float winB1, float winE2, float dlag);
	virtual int init(float taperLen, float width,float f1, float f2);
	virtual int run();
	int bp(float f1, float f2);
	int fillWin();
	int findBestWin();
	int outBestWinInfo(bool _out);
	int output();
	void setOutput(bool _out, const std::string& filepre) { d_outInfo = _out; d_outContour = true; d_prename = filepre; }
	void setOutput(bool _out) { d_outInfo = _out; d_outContour = false; }
	bool setFixLag(bool _set) { return d_fixLagTime = _set; }
	void FixLagTime(float lll) { setFixLag(true); d_LagTimeFixRef = lll; }
	int MakeFixLagIdx() { return d_IdxLagTimeFixRef = round(d_LagTimeFixRef/d_attentrace.d_sac.getDelta() );}
	void setMinGap(float g) { d_minGap = g; }
	int MakeMinGap() { return d_idxMinGap = round( d_minGap/d_attentrace.d_sac.getDelta() ) ; }
	void setMaxLen(float g) { d_maxLength = g; }
	int MakeMaxLength() { return d_idxMaxLength = round( d_maxLength/d_attentrace.d_sac.getDelta() ); }
private:
	float d_taperLen;
	float d_freqsmoothwidth;
	float d_loFreq, d_hiFreq;
	float d_bestpre, d_bestsuf, d_bestlag;

	bool  d_outInfo, d_outContour;

	bool  d_fixLagTime;
	float d_LagTimeFixRef;
	int   d_IdxLagTimeFixRef;

	float d_minGap;
	int   d_idxMinGap;

	float d_maxLength;
	int d_idxMaxLength;

	std::string d_prename;
};

#endif
