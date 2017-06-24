#include "qkeyqinvTrace.hh"
#include "io_format.hh"

#include <cmath>
#include <cstring>
int qinvTrace::alloc(int _N) {
	int N = _N;
	d_valid.resize(N);
	d_fs.resize(N);
	d_Greenamp.resize(N);
	d_Greenphase.resize(N);
	d_Qinv.resize(N);
	d_incPow.resize(N);
	d_refPow.resize(N);
	return 0;
}
int qinvTrace::init(std::string name, int _N){
	d_filename = name;
	std::string FileNamefreqGreenamp   =  name + ".FreqGreenamp" ;
	std::string FileNamefreqGreenphase =  name + ".FreqGreenphase" ;
	std::string FileNamefreqQinv       =  name + ".FreqQinv" ;
	std::string FileNameincFreqPow     =  name + ".inc.FreqPow" ;
	std::string FileNamerefFreqPow     =  name + ".ref.FreqPow" ;

	FILE *fpin_FileNamefreqGreenamp   = openReadFile(FileNamefreqGreenamp  );
	FILE *fpin_FileNamefreqGreenphase = openReadFile(FileNamefreqGreenphase);
	FILE *fpin_FileNamefreqQinv       = openReadFile(FileNamefreqQinv      );
	FILE *fpin_FileNameincFreqPow     = openReadFile(FileNameincFreqPow    );
	FILE *fpin_FileNamerefFreqPow     = openReadFile(FileNamerefFreqPow    );

	int N = _N;
	d_valid.resize(N);
	d_fs.resize(N);
	d_Greenamp.resize(N);
	d_Greenphase.resize(N);
	d_Qinv.resize(N);
	d_incPow.resize(N);
	d_refPow.resize(N);
	readFileCol( fpin_FileNamefreqGreenamp  , 0, N,  d_fs, d_Greenamp);
	readFileCol2(fpin_FileNamefreqGreenphase, 0, N,  d_Greenphase   );
	readFileCol2(fpin_FileNamefreqQinv      , 0, N,  d_Qinv         );
	readFileCol2(fpin_FileNameincFreqPow    , 0, N,  d_incPow       );
	readFileCol2(fpin_FileNamerefFreqPow    , 0, N,  d_refPow       );

	fclose(fpin_FileNamefreqGreenamp  );
	fclose(fpin_FileNamefreqGreenphase);
	fclose(fpin_FileNamefreqQinv      );
	fclose(fpin_FileNameincFreqPow    );
	fclose(fpin_FileNamerefFreqPow    );
	return 0;
}
int qinvTrace::makeValid(float maxPhase, float maxAmp) {
	DPRINT("this: %p qinvTrace::makeValid(%f, %f)", this, maxPhase, maxAmp);
	std::fill(d_valid.begin(), d_valid.end(), 0);
	int N = d_fs.size();
	for(int i=0;i<N;++i) {
		if(d_Greenamp[i] < maxAmp && fabsf(d_Greenphase[i]) < maxPhase && d_Qinv[i] < 1.0 && d_Qinv[i] > 0.0) {
			//DPRINT("valid amp:%f phase:%f", d_Greenamp[i], d_Greenphase[i]);
			d_valid[i] = 1;
		}
	}
	return 0;
}
int qinvTrace::clear() {
	DPRINT("qinvTrace::clear %p", this);
	d_valid.clear();
	d_fs.clear();
	d_Greenamp.clear();
	d_Greenphase.clear();
	d_Qinv.clear();
	d_incPow.clear();
	d_refPow.clear();
	return 0;
}
int qinvTrace::outValidFreqQinv(std::string filename) {
	outPlainFile out(filename);
	for(unsigned int i=0;i<d_fs.size(); ++i) {
		if(d_valid[i] && d_Qinv[i] <= 1.0)
			out.outCol2f(d_fs[i], d_Qinv[i] );
		else
			out.outChar('>');
			out.outCol2f(d_fs[i], d_Qinv[i] );
	}
	return 0;
}


int qKeyqinvTrace::clear() {
	DPRINT("qKeyqinvTrace::clear %p", this);
	for(std::vector<qinvTrace*>::iterator iter = d_value.begin(); iter!= d_value.end(); ++iter) {
		delete (*iter);
	}
	d_value.clear();
	return 0;
}
int qKeyqinvTrace::makeValid(float maxPhase, float maxAmp) {
	DPRINT("this: %p qKeyqinvTrace::makeValid(%f, %f)", this, maxPhase, maxAmp);
	for(std::vector<qinvTrace*>::iterator iter = d_value.begin(); iter != d_value.end(); ++iter) {
		(*iter)->makeValid( maxPhase,  maxAmp);
	}
	return 0;
}
int qKeyqinvTrace::ave() {
	d_stack.alloc( d_value[0]->size() );
	d_stack.d_fs.assign( d_value[0]->d_fs.begin(), d_value[0]->d_fs.end() );
	std::fill(d_stack.d_Greenamp.begin(), d_stack.d_Greenamp.end(), 0.0 );
	std::fill(d_stack.d_valid.begin(), d_stack.d_valid.end(), 0 );
	DPRINT("%p %d", this, (int) (d_value.size()) );
	for(std::vector<qinvTrace*>::iterator iter = d_value.begin();  iter != d_value.end(); ++iter) {
			for(int i=0;i<d_stack.size();++i) {
				 d_stack.d_Greenamp[i] += (*iter)->d_valid[i] != 0 ? logf( (*iter)->d_Greenamp[i] ) : 0;
				 d_stack.d_valid[i] += (*iter)->d_valid[i];
				 //if(d_stack.d_valid[i] != 0 && fabsf(d_stack.d_fs[i] -0.585938) < 1.0e-3 ) {
				 //	fprintf(stderr, "%s f: %f amp: %f sumlogamp: %f qinv: %f\n", (*iter)->d_filename.c_str()  ,
				 //		d_stack.d_fs[i], 
				 //		(*iter)->d_Greenamp[i], d_stack.d_Greenamp[i],
				 //		(*iter)->d_Qinv[i]);
				 //}
		}
	}
	float tau = d_key.getTau();
	#define PI 3.1415926535
	for(int i=0;i<d_stack.size();++i) {
		//if(d_stack.d_valid[i] != 0 && fabsf(d_stack.d_fs[i] -0.585938) < 1.0e-3 ) {
		//	fprintf(stderr, "%f  %d \n", d_stack.d_Greenamp[i], d_stack.d_valid[i]);
		//}
		d_stack.d_Greenamp[i] = d_stack.d_valid[i] == 0 ? 0 : expf( d_stack.d_Greenamp[i] / d_stack.d_valid[i] );
		//fprintf(stderr, "%f %f %f\n", d_stack.d_fs[i], d_stack.d_Greenamp[i], tau );
		d_stack.d_Qinv[i] = d_stack.d_Greenamp[i]>0.0 ? -logf(d_stack.d_Greenamp[i]) / PI / d_stack.d_fs[i] / tau : 0.0;
		//if(d_stack.d_valid[i] != 0 && fabsf(d_stack.d_fs[i] -0.585938) < 1.0e-3 ) {
		//	fprintf(stderr, "%f  %d  %f f: %f tau: %f\n", d_stack.d_Greenamp[i], d_stack.d_valid[i], d_stack.d_Qinv[i], d_stack.d_fs[i], tau );
		//}
	}
	#undef PI
	return 0;
}
int   qKeyqinvTrace::outputStackSingleFreq(std::string filename) {
	return d_stack.outValidFreqQinv(filename);
}
