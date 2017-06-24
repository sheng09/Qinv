#include "la_trace.hh"

#ifdef __cplusplus
#include <cmath>
#else
#include <math.h>
#endif



#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 



/*
	Return the average value
	Input:
		trace: the data and its size
		len:
	Return:
		the average value
	*/
float ave(float *trace, long len) {
	long i;
	float ave = 0.0;
	for( i = 0; i< len; ++i) {
		ave += trace[i] / len;
	}
	return ave;
}
/*
	Remove the mean value
	Input:
		trace: the data and its size
		len:
	Output:
		the rmeaned data overlap the original data.
	Note:
		the original data is destroyed
*/
int rmean(float *trace, long len) {
	long i;
	float value = ave(trace, len);
	for(i = 0; i < len; ++i)
		trace[i] -= value;
	return 0;
}
/*
	Linear Regression
	Input:
		trace: the data and its size
		len:
	Output:
		k: the slope
		b: the intercept
*/
int linReg(float *trace, long len, float *k, float *b) {
	float xmean = (0+len)*len/2.0;
	float ymean = ave(trace, len);
	float dx = 0.0;
	float k1 = 0.0, k2 = 0.0;
	long i;
	for(i = 0; i < len; ++i) {
		dx = i - xmean;
		k1 += (trace[i] - ymean) * dx;
		k2 += dx * dx;
	}
	*k = k1 / k2;
	*b = ymean - (*k) * xmean;
	return 0;
}
/*
	Remove the linear trend
	Input:
		trace: the data and its size
		len:
	Output:
		the detrended data overlap the original data.
	Note:
		the original data is destroyed
*/
int detrend(float *trace, long len) {
	float k, b;
	linReg(trace, len, &k, &b);
	long i;
	for(i = 0; i< len; ++i)
		trace[i] -= (i*k + b);
	return 0;
}





#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */
