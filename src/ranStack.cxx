#include "ranStack.hh"
#include <iostream>
#include <cmath>
ranStack::ranStack(const std::string& filename, int tmark, float t1, float t2,long seed, int nroot, int nstack)
	: tracef1d(filename, tmark, t1, t2)
{
	setSeed(seed);
	setNRoot(nroot);
	setNStack(nstack);
}

float ranStack::findSacMax() {
	float max= 0.0;
	std::vector<float>& data = getDataVec();
	for(std::vector<float>::iterator iter = data.begin(); iter!=data.end(); ++iter) {
		if(max < fabsf(*iter))
			max = fabsf(*iter);
	}
	return max;
}

int  ranStack::noiseStack(float noiseLevel) {
	float max = findSacMax() * noiseLevel;
	float sum, v, vabs, nabs;
	std::vector<float>& data = getDataVec();
	for(std::vector<float>::iterator iter = data.begin(); iter!=data.end(); ++iter) {
		sum = 0.0;
		for(int stack=0; stack<d_nStack; ++stack) {
			float r = ranMax(&d_seed, max);
			v = (*iter) -  r;
			vabs = fabsf( v );
			//std::cout << "ori: " << *iter << " --> " << vabs << ", max:" << max << '\n';
			nabs = expf( logf(vabs)/d_nRoot );
			if( v > 0.0f )
				v += nabs;
			else
				v -= nabs;
		}
		//std::cout << v << ", "<<*iter << '\n';
		v /= d_nRoot;
		*iter = powf(fabsf( v ), d_nRoot - 1.0) * v;
	}
	return 0;
}