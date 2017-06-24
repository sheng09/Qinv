#include "test.hh"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>

#ifndef __ATTEN__QKEYQINVTRACE____
#define __ATTEN__QKEYQINVTRACE____


//
class qinvTrace {
public:
	qinvTrace() : d_filename("") {}
	qinvTrace(std::string name, int _N) :d_filename(name) { init(name, _N); }
	~qinvTrace() { DPRINT("qinvTrace::decontructor: %p", this);	clear(); };
	int init(std::string name, int _N);
	int alloc(int N);
	float getdf() { return (d_fs[1] - d_fs[0]); }
	int makeValid(float maxPhase, float maxAmp);
	int clear() ;
	int size() {return d_fs.size(); }

	int outValidFreqQinv(std::string filename) ;
	//std::vector<int>  * getvalid() { return &d_valid; }
	//std::vector<float>* getamp() { return &d_Greenamp; }
	//std::vector<float>* getfs()  { return &d_fs; }

	std::string d_filename;
	std::vector<int>    d_valid;
	std::vector<float>  d_fs;
	std::vector<float>  d_Greenamp;
	std::vector<float>  d_Greenphase;
	std::vector<float>  d_Qinv;
	std::vector<float>  d_incPow;
	std::vector<float>  d_refPow;
};


typedef struct struct_qKey {
	float d_tau;
	float d_df;
	//int   d_id;
	void setTau(float tau) { d_tau = tau; }
	float getTau() { return d_tau; }
	float getdf() { return d_df; }
	//int   getcount() {return count; }
	bool compareDf(float v) {
		if( d_df - v <= 1.0e-6 ) return true;
		return false;
	}
} qKey;
/*
	A mapping set that all vector<qinvTrace*> share a same sampling interval.

			 +---------------------+
	qKey --> | vector<qinvTrace*>  |
			 +---------------------+
			 |  [qinvTrace 1]      |
			 |  [qinvTrace 2]      |
			 |  [qinvTrace 3]      |
			 |   ...               |
			 |  [qinvTrace N]      |
			 +---------------------+
*/
class qKeyqinvTrace{
public:
	qKeyqinvTrace() {}
	qKeyqinvTrace(float df) { d_key.d_df = df; }
	~qKeyqinvTrace() { DPRINT("qKeyqinvTrace::decontructor: %p", this);	clear(); }
	int clear();
	void setTau(float tau) { d_key.setTau(tau);}
	float getdf() { return d_key.getdf(); }
	bool  compareDf(float v) { return d_key.compareDf(v); }
	int   makeValid(float maxPhase, float maxAmp);
	int   ave();
	int   size() {return d_value.size(); }
	int   traceLen() { return d_value[0]->size(); }
	void  push(qinvTrace* ptr) { d_value.push_back(ptr); }
	int   outputStackSingleFreq(std::string filename);
private:
	qKey d_key;
	std::vector<qinvTrace*> d_value;
	qinvTrace d_stack;
};


typedef FILE * FILEPTR;
inline void readCol2(FILEPTR &fp, float &x, float &y) {
	fscanf(fp, "%f %f", &x, &y);
	//if( succ !=2 ) {
	//fprintf(stderr, "%f %f\n", x, y );
	//	fprintf(stderr, "Err: fail to read 2 float values.\n" );
	//	exit(-1);
	//}
}
inline int jumpFile(FILEPTR &fp, int &N) {
	//fprintf(stderr, "jump: %d\n", N);
	#define LEN 8192
	static char tmp[LEN];
	for(int i=0;i<N;++i) {
		if(!fgets(tmp, LEN,fp) ) {
			fprintf(stderr, "Err: fail to jump %d lines.\n", N);
			exit(-1);
		}
	}
	#undef LEN
	return 0;
}
inline void readFileCol(FILEPTR &fp, int jump, int nline, std::vector<float> &vecx, std::vector<float> &vecy ) {
	jumpFile(fp, jump);
	for(int i=0;i<nline;++i) {
		readCol2(fp, vecx[i], vecy[i]);
	}
}
inline void readFileCol2(FILEPTR &fp, int jump, int nline, std::vector<float> &vec) {
	jumpFile(fp, jump);
	static float tmp;
	for(int i=0;i<nline;++i) {
		readCol2(fp, tmp, vec[i]);
	}
}
inline void readFileCol1(FILEPTR &fp, int jump, int nline, std::vector<float> &vec) {
	jumpFile(fp, jump);
	static float tmp;
	for(int i=0;i<nline;++i) {
		readCol2(fp, vec[i], tmp);
	}
}
inline char* rmNewLine(char *str, int N) {
	for(int i=N-1; i>=0;--i) {
		if(str[i] == '\n') {
			str[i] = '\0';
			break;
		}
	}
	return str;
}
inline FILE* openReadFile(std::string name) {
	FILE *fp = fopen(name.c_str(), "r");
	if(fp == NULL) {
		fprintf(stderr, "Err: open %s for read.\n", name.c_str());
		exit(-1);
	}
	return fp;
}
inline FILE* openWriteFile(std::string name) {
	FILE *fp = fopen(name.c_str(), "w");
	if(fp == NULL) {
		fprintf(stderr, "Err: open %s for write.\n", name.c_str());
		exit(-1);
	}
	return fp;
}

#endif
