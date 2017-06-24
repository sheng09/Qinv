#include <cmath>
#include <string>
#include <cstdlib>
#include <vector>
#include "sac.h"

#ifndef __ATTEN__TRACE__
#define __ATTEN__TRACE__

#define PI 3.1415926535
#define TAPER_HAN(N,i) ( 0.5-0.5* (cos(PI/(N)*(i))) )

/*
An 1D array collection
*/
class tracef1d
{
public:
	tracef1d() : d_data(0) {  } //useless
	tracef1d(int len, float delta, float start);
	tracef1d(const std::string& filename, int tmark, float t1, float t2);
	tracef1d(const std::string& filename);
	virtual ~tracef1d();
	virtual int initSac(const std::string& filename, int tmark, float t1, float t2);
	virtual int initSac(const std::string& filename);
	virtual int initMem(int len, float delta, float start);

	virtual int alloc(int l);
	void del()  { if(!d_data.empty() ) d_data.clear(); }

	int   taper(int N);
	float floorCrossZero(float __time, int *idx);
	float ceilCrossZero (float __time, int *idx);
	float getDelta()               { return d_delta;        }
	virtual void setDelta(float d) { d_delta = d;           }
	float getStart()               { return d_start;        }
	float setStart(float s)        { d_start = s; return s; }

	void assign(float *src, int len); //do not change d_len
	void assign(float *srcbegin, float *srcend); //do not change d_len
	virtual float* getData()             { return d_data.data(); }
	virtual std::vector<float>& getDataVec() { return d_data; }
	SACHEAD& getSacHdr() { return d_sachdr; }
	void setHdr(SACHEAD& src) { d_sachdr = src; }

	virtual int    size()      { return d_len; }
	virtual int setSize(int l) { d_len = l; return l; }
	int UseSize()         { return d_useful; }
	int setUseSize(int l) { d_useful = l; return l; }
	int roundIdx(float t) { return roundf( (t-d_start)/d_delta ) ; }
	int floorIdx(float t) { return floorf( (t-d_start)/d_delta ) ; }
	int ceilIdx( float t) { return ceilf(  (t-d_start)/d_delta ) ; }
	int outTrace(     const std::string& filename);
	int outTimeAxis(  const std::string& filename);
	int outTimeTrace( const std::string& filename);

	std::string& getSacName() { return d_sacname; }
	float getTlagRef() {return d_t6 - d_t5; }
	float getTinc() {return d_t5; }
	float getTref() {return d_t6; }
	float setTinc(float f) {return d_t5 = f;}
	float setTref(float f) {return d_t6 = f;}
	void outSac(const std::string& filename) { write_sac(filename.c_str(), d_sachdr, getData()); }
	bool checkTime(float t) { if( t <= d_sachdr.e && t >= d_sachdr.b) return true; return false; }
	float getSacB() { return d_sachdr.b; }
	float getSacE() { return d_sachdr.e; }
private:
	std::string d_sacname;
	//float *d_data;
	SACHEAD             d_sachdr;
	std::vector<float>  d_data;
	int   d_len;
	int   d_useful;

	float d_delta;
	float d_start;
	float d_t5;
	float d_t6;
};

float* taperhanTrace(int N);
float* taperhanTraceOrd2(int N);
#endif