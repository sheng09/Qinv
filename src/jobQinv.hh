#include <string>
#include "q_job.hh"

#ifndef __ATTEN__JOB__Q__INV__
#define __ATTEN__JOB__Q__INV__



class jobQinv : public qjob
{
public:
	jobQinv(int ac, char * av[]);
	jobQinv(const std::string& sacfile, int tmark, float t1sac, float t2sac, 
		float t1, float t2, float lag, float taperLen, float width, int fftsize);
	//jobQinv(std::string sacfile, int tmark, float t1sac, float t2sac);
	virtual ~jobQinv() {};
	virtual int run();

	int setOutput(const std::string& _name)   {d_prename = _name; return 0; }
	virtual int init(float taperLen, float width);

private:
	std::string d_prename;
	float d_taperLen;
	float d_freqsmoothwidth;

	float d_t1, d_t2, d_t3, d_t4;
	float d_lag;

	int   d_idx1, d_idx2, d_idx3, d_idx4;
	int   d_idxLag;
};


#endif