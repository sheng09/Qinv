#include "test.hh"
#include "jobwin.hh"
#include <cstdio>
#include <string>
#include <cstdlib>

static char HMSG[] ="\n\
Usage: \n\
    %s -Ifilename [-Ctmarker/t1/t2] [-L[F]lag] [-Xmingap] [-Mmaxlength] [-Wtb/te] [-Ttaper] [-Ff1/f2] [-Ssmooth] [-Oprename] \n\
\n\
Find the best time window, and the best lag time, for the incident and the reflected phase. \n\
\n\
Options:\n\
    -I filename : sac file name\n\
        The \"t5\" and \"t6\" within sac header should be assigned for the arrival time of the \n\
        incident and the reflected phase, respectively. \n\
    -C tmarker/t1/t2 : cut for reading sac. (5/-3.0/7.0)\n\
        int tmarker,      time mark in sac header \n\
                    0-9 -> tn; -5 -> b; -3 -> o; -2 -> a \n\
                    others refer to t=0. \n\
        float   t1      begin time is tmarker + t1 \n\
        float   t2      end time is tmarker + t2 \n\
    -L  lag : the lag search range will be [ -lag+t6-t5, lag+t6-t5]. (0.5)\n\
   [-LF]lag : fixed lag time between the incident and the reflected phase.\n\
\n\
    -X mingap : the min gap (seconds) between the incident and reflected phases. (0.0)\n\
    -M maxlength : the max length (seconds)  for each phases. (99999.9) \n\
    -W tb/te : the window search range. (-1/1) \n\
        For the incident phase, the search range for its start is: [t5+tb, c1],\n\
        where the c1 corresponds to the last point that cross zero before t5.\n\
        And the search range for its end is [c2, t5+te], where the c2 corresponds\n\
        to the first point that after t5.\n\
\n\
    -T taper : the taper length(seconds). (0.1) \n\
    -F f1/f2 : the frequency band for inverting Fukushima's coefficient. (1.0/10.0) \n\
    -S smooth : the frequency smooth width(Hz). (0.6) \n\
    -O pre-name : the pre name for output. (-O is closed in default setting) \n\
        Three files will be generated: \n\
        	1. \"prenam.coef2d\", 2. \"prename.cor2d\", 3. \"prename.lag2d\". \n\
        For these files, the 1st and 2nd column are the start and the end of window  \n\
        options, respectively, and the 3rd column is value. \n\
        For \"prenam.coef2d\", the 3rd column is the Fukushima's ceofficient. \n\
        For \"prename.cor2d\", the 3rd column is the cross coefficent. \n\
        For \"prename.lag2d\", the 3rd column is the best lag time, corresponding to  \n\
        current window optiom. \n\
";

jobWin::jobWin(int ac, char *av[]) 
	:	d_bestpre(0.0), d_bestsuf(0.0), d_bestlag(0.0),
		d_outInfo(true), d_outContour(false), d_fixLagTime(false), d_minGap(0.0), d_maxLength(99999.9) {
	int tmark = 5;
	float t1 = -3, t2 = 7, dlag = 0.5, winB = -1, winE = 1, taperLen = 0.1, f1 = 1.0, f2 = 1.0, smooothlength = 0.6;
	char *pfilename;
	float _mingap =0.0;
	float _maxlength = 99999.9;
	std::string sacfile;
	std::string outputPre;
	if(ac == 1) {
		fprintf(stdout, HMSG, av[0]);
		exit(1);
	}
	for(int iopt=1; iopt<ac; ++iopt) {
		if(av[iopt][0] == '-') {
			if(av[iopt][1] == 'C') {
				sscanf(av[iopt], "-C%d/%f/%f", &tmark, &t1, &t2);
			} else if (av[iopt][1] == 'L') {
				if(av[iopt][2] == 'F') {
					sscanf(av[iopt], "-LF%f", &dlag);
					FixLagTime(dlag);
				} else {
					sscanf(av[iopt],  "-L%f", &dlag);
				}
			} else if  (av[iopt][1] == 'W') {
				sscanf(av[iopt],  "-W%f/%f", &winB, &winE);
			} else if  (av[iopt][1] == 'T') {
				sscanf(av[iopt],  "-T%f", &taperLen);
			} else if  (av[iopt][1] == 'I') {
				pfilename = &(av[iopt][2]);
				sacfile = std::string(pfilename);
			} else if  (av[iopt][1] == 'F') {
				sscanf(av[iopt],  "-F%f/%f", &f1, &f2);
			} else if  (av[iopt][1] == 'S') {
				sscanf(av[iopt],  "-S%f", &smooothlength);
			} else if  (av[iopt][1] == 'O') {
				outputPre = std::string(( &av[iopt][2]) );
				setOutput(true, outputPre);
			} else if  (av[iopt][1] == 'X') {
				sscanf(av[iopt],  "-X%f", &_mingap);
			} else if  (av[iopt][1] == 'M') {
				sscanf(av[iopt],  "-M%f", &_maxlength);
			} else if  (av[iopt][1] == 'H' || av[iopt][1] == 'h') {
				fprintf(stdout, HMSG, av[0]);
				exit(1);
			} else {
				fprintf(stderr, "Unrecognized opt: %s\n", av[iopt]);
				exit(-1);
			}
		} else {
			fprintf(stderr, "Unrecognized opt: %s\n", av[iopt]);
			exit(-1);
		}
	}
	#define FFTSIZE 256
	setFFTsize(FFTSIZE);
	#undef FFTSIZE
	setMinGap(_mingap);
	setMaxLen(_maxlength);
	DPRINT("%s %d %f %f", sacfile.c_str(), tmark, t1 ,t2);
	initQjob(sacfile, tmark, t1, t2, winB, winE, dlag);
	init(taperLen, smooothlength, f1, f2);
}

jobWin::jobWin(const std::string& sacfile, int tmark, float t1, float t2, float winB1, float winE2, float dlag)
	: d_bestpre(0.0), d_bestsuf(0.0), d_bestlag(0.0),
	d_outInfo(true), d_outContour(false), d_fixLagTime(false), d_minGap(0.0), d_maxLength(99999.9)
{
	#define FFTSIZE 256
	setFFTsize(FFTSIZE);
	#undef FFTSIZE
	initQjob(sacfile, tmark, t1, t2, winB1, winE2, dlag);
}
int jobWin::init(const std::string& sacfile, int tmark, float t1, float t2, float winB1, float winE2, float dlag) {
	#define FFTSIZE 256
	setFFTsize(FFTSIZE);
	#undef FFTSIZE
	initQjob(sacfile, tmark, t1, t2, winB1, winE2, dlag);
	return 0;
}
int jobWin::init(float taperLen, float width,float f1, float f2) {
	d_taperLen        = taperLen;
	d_freqsmoothwidth = width   ;
	d_loFreq          =f1;
	d_hiFreq          =f2;
	return 0;
}

int jobWin::run() {
	fillWin();
	findBestWin();
	outBestWinInfo(d_outInfo);
	DPRINT( "outBestWinInfo is DONE %d", d_outContour );
	if(d_outContour) {
		output();
	}
	DPRINT("jobWin::run() is DONE")
	return 0;
}
int jobWin::fillWin(){
	int i1min = d_winopt.getMin_i1();
	int i1max = d_winopt.getMax_i1();
	int i2min = d_winopt.getMin_i2();
	int i2max = d_winopt.getMax_i2();
	int ilagmin = d_winopt.getMin_ilag();
	int ilagmax = d_winopt.getMax_ilag();
	int i1, i2, ilag;
	float tmpcor, maxcor;
	int ibestlag;
	float delta = d_attentrace.d_sac.getDelta();
	int index = 0;

	MakeMinGap();
	MakeFixLagIdx();
	MakeMaxLength();
	//fprintf(stderr, "%f %d\n", d_maxLength, d_idxMaxLength);
	for(i1=i1min; i1<=i1max; ++i1) {
		for(i2=i2min; i2<=i2max; ++i2) {
			if( i1 + d_idxMaxLength <= i2 ) {
				d_winopt.setCor2d(index,  0.0);
				d_winopt.setCoef2d(index, 0.0);
				d_winopt.setBestLag2d(index, 0.0);
				++index;
				continue;
			}
			//DPRINT("%d %d", i1, i2);
			//////////////////////////////////////////////////
			// find best lag
			if(!d_fixLagTime) {
				ibestlag = 0;
				maxcor = -2.0;
				for(ilag = ilagmin, ibestlag=ilagmin; ilag<=ilagmax; ++ilag) {
					if(  ((i1 + ilag - d_idxMinGap) <= i2) ) {
						d_winopt.setCor2d(index,  0.0);
						d_winopt.setCoef2d(index, 0.0);
						d_winopt.setBestLag2d(index, 0.0);
						continue;
					}
					d_attentrace.make(i1, i2, ilag, d_taperLen);
					tmpcor = d_attentrace.cor();
					if(tmpcor>maxcor) {
						maxcor = tmpcor;
						ibestlag = ilag;
					}
				}
				d_attentrace.make(i1, i2, ibestlag, d_taperLen);
			} else {
				ibestlag = d_IdxLagTimeFixRef;
				d_attentrace.make(i1, i2, ibestlag, d_taperLen);
				maxcor = d_attentrace.cor();
			}
			if( i1+ibestlag - d_idxMinGap <= i2 ) {
				d_winopt.setCor2d(index,  0.0);
				d_winopt.setCoef2d(index, 0.0);
				d_winopt.setBestLag2d(index, 0.0);
				++index;
				continue;
			}
			d_winopt.setBestLag2d(index, ibestlag * delta );    //------
			d_winopt.setCor2d(index, maxcor);                   //------
			//////////////////////////////////////////////////
			// cal the fukushima's coef
			d_attentrace.runSpec();
			d_attentrace.runPow();
			d_attentrace.smoothPow(d_freqsmoothwidth);
			float vcoef =  d_attentrace.coef(d_loFreq, d_hiFreq);
			d_winopt.setCoef2d(index,  vcoef ); //------
			++index;//------
		}
	}
	DPRINT("End of FillWin");
	return 0;
}
int jobWin::findBestWin() {
	d_winopt.findMaxCoef(&d_bestpre, &d_bestsuf, &d_bestlag);
	return 0;
}
int jobWin::outBestWinInfo(bool _out) {
	if(!_out) return 0;
	fprintf(stdout, "SACHDR t5: %f , t6: %f , lag %f , dt: %f\n", 
			d_attentrace.d_sac.getTinc(), d_attentrace.d_sac.getTref(),
			d_winopt.getLagRef(), d_attentrace.d_sac.getDelta()
		);
	fprintf(stdout, "PreOpt[ %f %f ](%d), SufOpt[ %f %f ](%d), LagOpt[ %f %f ](%d)\n", 
			d_winopt.getMinB()   ,
			d_winopt.getMaxB()   ,
			d_winopt.sizePre()   ,
			d_winopt.getMinE()   ,
			d_winopt.getMaxE()   ,
			d_winopt.sizeSuf()   ,
			d_winopt.getMinLag() ,
			d_winopt.getMaxLag() ,
			d_winopt.sizeLag()
		);
	fprintf(stdout, "Best pre: %f , suf: %f , lag: %f\n", d_bestpre, d_bestsuf, d_bestlag);
	return 0;
}
int jobWin::output( ) {
	DPRINT( "d_winopt.outCoef3Col()" );
	d_winopt.outCoef3Col(    d_prename+".coef2d");
	DPRINT( "d_winopt.outCor3Col()" );
	d_winopt.outCor3Col(     d_prename+".cor2d" );
	DPRINT( "d_winopt.outBestLag3Col()" );
	d_winopt.outBestLag3Col( d_prename+".lag2d" );
	DPRINT( "d_winopt.outPre()" );
	d_winopt.outPre(         d_prename+".pres");
	DPRINT( "d_winopt.outSuf()" );
	d_winopt.outSuf(         d_prename+".sufs");
	return 0;
	DPRINT( "jobWin::output( ) is DONE");
}

