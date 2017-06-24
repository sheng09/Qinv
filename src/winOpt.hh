#include <string>
#include <cmath>
#include <vector>
#include "test.hh"


#ifndef __ATTEN__WIN__OPT__
#define __ATTEN__WIN__OPT__


class winOpt
{
public:
	winOpt();
	winOpt(float start, float delta,float t5, float t6, float winB1, float winB2, float winE1, float winE2, float dlag);
	virtual ~winOpt();
	virtual int init(float start, float delta,float t5, float t6, float winB1, float winB2, float winE1, float winE2, float dlag);
	int alloc();

	int sizePre() { return d_lenPre; }
	int sizeSuf() { return d_lenSuf; }
	int sizeLag() { return d_ilagmax - d_ilagmin; }
	int getMin_i1() { return d_i1min; }
	int getMax_i1() { return d_i1max; }
	int getMin_i2() { return d_i2min; }
	int getMax_i2() { return d_i2max; }
	int getMin_ilag() { return d_ilagmin; }
	int getMax_ilag() { return d_ilagmax; }
	float getDelta() { return d_delta; }
	float getMinB()   { return d_b1; }
	float getMaxB()   { return d_b2; }
	float getMinE()   { return d_e1; }
	float getMaxE()   { return d_e2; }
	float getMinLag() { return d_lag1; }
	float getMaxLag() { return d_lag2; }
	float getLagRef() { return d_lagref; }
	float* getPreAxis() { return d_pre.data(); }
	float* getSufAxis() { return d_suf.data(); }
	float* getCoef()    { return d_coef.data(); }
	float* getCor()     { return d_cor.data(); }
	float* getBestLag() { return d_bestlag.data(); }

	float findMaxCoef(float *pre, float *suf, float *lag);
	float findMaxCor(float *pre, float *suf, float *lag);
	int makeReal(); // if windows overlap, then asign coef to be zero

	void setBestLag2d(int index, float v) { d_bestlag[ index ] = v; }
	void setCor2d(    int index, float v) { d_cor[     index ] = v; }
	void setCoef2d(   int index, float v) { d_coef[    index ] = v; }

	void idx_v2m(int idx, int &ipre, int &isuf) { ipre = idx / d_lenSuf + d_i1min; isuf = idx - (ipre-d_i1min)*d_lenSuf + d_i2min; }
	int  idx_m2v(int ipre, int isuf)          { return (ipre- d_i1min )*d_lenSuf + isuf - d_i2min; }
	float valueCoef(int  ipre, int  isuf)     { return d_coef[idx_m2v(ipre, isuf)] ; }
	float valueCor( int  ipre,int  isuf)      { return d_cor[idx_m2v(ipre, isuf)] ; }
	float valueBestLag(int  ipre,int  isuf)   { return d_bestlag[idx_m2v(ipre, isuf)] ; }
	float valueCoef(float pre, float suf)     { return valueCoef( time2idxRound(pre), time2idxRound(suf) ) ;}
	float valueCor( float pre, float suf)     { return valueCoef( time2idxRound(pre), time2idxRound(suf) ) ;}
	float valueBestLag( float pre, float suf) { return valueBestLag( time2idxRound(pre), time2idxRound(suf) ) ;}

	float idx2time(int idx) {return idx*d_delta + d_start; }
	int time2idxFloor(float _t) { return  floor( (_t-d_start)/d_delta ) ; }
	int time2idxCeil( float _t) { return  ceil( (_t-d_start)/d_delta ) ; }
	int time2idxRound( float _t) { return  round( (_t-d_start)/d_delta ) ; }

	int outCoef3Col(    const std::string& filename);
	int outCor3Col(     const std::string& filename);
	int outBestLag3Col( const std::string& filename);
	int outPre(         const std::string& filename);
	int outSuf(         const std::string& filename);
	int outCoef(        const std::string& filename);
	int outCor(         const std::string& filename);
	int outBestLag(     const std::string& filename);
private:
	float d_start;
	float d_delta;
	float d_b1, d_b2;
	float d_e1, d_e2;
	float d_lag1, d_lag2, d_lagref;

	int d_i1max, d_i1min;
	int d_i2max, d_i2min;
	int d_ilagmax, d_ilagmin;

	int   d_lenPre, d_lenSuf;
	long  d_lenOpt;
	std::vector<float> d_pre;
	std::vector<float> d_suf;
	std::vector<float> d_coef;
	std::vector<float> d_cor;
	std::vector<float> d_bestlag;
};


#endif