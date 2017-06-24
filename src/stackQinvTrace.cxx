#include "stackQinvTrace.hh"
#include <cmath>
#include <cstring>

static char HMSG[] ="\
%s -Afilelst -Nnline -Ttau -Mmaxamp/maxPhase -OqinOut \n\
\n\
";

stackQinv::stackQinv(int ac, char *av[]) {
	char *filelst = NULL;
	char *fileout = NULL;
	int nline = 0;
	float maxPhase = 90, maxAmp = 1.0;
	float tau = 1.3;
	for(int iopt = 1;iopt<ac;++iopt) {
		if(av[iopt][0] == '-') {
			if (av[iopt][1] == 'A' ) {
				filelst = &( av[iopt][2] );
			} else if ( av[iopt][1] == 'N' ) {
				sscanf(av[iopt], "-N%d", &nline);
			} else if ( av[iopt][1] == 'M' ) {
				sscanf(av[iopt], "-M%f/%f", &maxAmp, &maxPhase);
			} else if ( av[iopt][1] == 'O' ) {
				fileout = & ( av[iopt][2] );
			} else if ( av[iopt][1] == 'T' ) {
				sscanf(av[iopt], "-T%f", &tau);
			} else if ( av[iopt][1] == 'H' || av[iopt][1] == 'h' ) {
				fprintf(stderr, HMSG, av[0]);
				exit(1);
			} else {
				fprintf(stderr, "Err: unrecognized arg %s.\n", av[iopt]);
				exit(-1);
			}
		}
	}
	if( ac == 1 || filelst == NULL || fileout == NULL || nline == 0 ) {
		fprintf(stderr, HMSG, av[0]);
		exit(1);
	}
	setTau(tau);
	init( filelst, nline );
	d_outfilename = std::string( fileout );
	d_maxPhase = maxPhase;
	d_maxAmp = maxAmp;
	DPRINT("input:    %s", filelst);
	DPRINT("nlines:   %d", nline);
	DPRINT("maxAmp:   %f", d_maxAmp);
	DPRINT("maxPhase: %f", d_maxPhase);
	DPRINT("output:   %s", d_outfilename.c_str());
}

int stackQinv::run() {
	DPRINT("%p stackQinv::run()", this);
	DPRINT("maxAmp:   %f", d_maxAmp);
	DPRINT("maxPhase: %f", d_maxPhase);
	DPRINT("output:   %s", d_outfilename.c_str());
	fill();
	makeValid(d_maxPhase, d_maxAmp);
	DPRINT("%p before aveSub()", this);
	aveSub();
	DPRINT("%p after aveSub()", this);
	output1st(d_outfilename);
	return 0;
}

int stackQinv::clear() {
	DPRINT("stackQinv::clear %p", this);
	for(std::vector< qKeyqinvTrace * >::iterator iter = d_data.begin(); iter != d_data.end(); ++iter) {
		delete (*iter);
	}
	d_data.clear();
	d_prenames.clear();
	return 0;
}
int stackQinv::init(std::string filelst, int nline) {
	d_nline = nline;
	FILE *fp = fopen(filelst.c_str(), "r");
	#define LEN 8192
	char tmp[LEN];
	memset( tmp, 0, LEN*sizeof(char) );
	while(fgets(tmp, LEN,fp)) {
		//std::string* p_str = new std::string(tmp);
		d_prenames.push_back( rmNewLine(tmp, LEN) );
		memset( tmp, 0, LEN*sizeof(char) );
	}
	#undef LEN
	fclose(fp);
	return 0;
}
int stackQinv::fill() {
	for(std::vector< std::string >::iterator iter = d_prenames.begin(); iter != d_prenames.end(); ++iter) {
		qinvTrace *ptrace = new qinvTrace( *iter, d_nline );
		DPRINT("new: %p", ptrace);
		float df = ptrace->getdf();
		qKeyqinvTrace *ptr_vecqinv = find_df(df);
		if( ptr_vecqinv == NULL ) {
			ptr_vecqinv  = new qKeyqinvTrace(df);
			ptr_vecqinv->setTau(getTau());
			DPRINT("new: %p", ptr_vecqinv);
			d_data.push_back(ptr_vecqinv);
		}
		ptr_vecqinv->push(ptrace);
	}
	return 0;
}
int stackQinv::makeValid(float maxPhase, float maxAmp) {
	DPRINT("this: %p stackQinv::makeValid(%f, %f)", this, maxPhase, maxAmp);
	for(std::vector< qKeyqinvTrace * >::iterator iter = d_data.begin(); iter !=d_data.end(); ++iter) {
		//fprintf(stderr, "%p %p\n", this, (*iter) );
		(*iter)->makeValid(maxPhase, maxAmp);
	}
	return 0;
}
int stackQinv::aveSub() {
	DPRINT("%p stackQinv::aveSub() %d", this, (int) (d_data.size()) );
	for(std::vector< qKeyqinvTrace * >::iterator iter = d_data.begin(); iter != d_data.end(); ++iter) {
		DPRINT("%p %p", this, (*iter) );
		(*iter)->ave();
	}
	return 0;
}
int stackQinv::mergeConflict() {
	// ?
	fprintf(stderr, "Err: stackQinv::mergeConflict() is not availale now\n" );
	exit(-1);
	return 0;
}
qKeyqinvTrace* stackQinv::find_df(float df) {
	for(std::vector< qKeyqinvTrace * >::iterator iter = d_data.begin(); iter != d_data.end(); ++iter) {
		if( (*iter)->compareDf(df)) return *iter;
	}
	return NULL;
}
int stackQinv::output1st(std::string filename) {
	return d_data[0]->outputStackSingleFreq(filename);
}
