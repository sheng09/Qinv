#include "jobQinv.hh"
#include "test.hh"

static char HMSG[] = "\n\
Usage: \n\
    %s -Ifilename [-Ctmarker/t1/t2] -Llag -Wtb/te [-Ttaper] [-Ff1/f2] [-Ssmooth] [-Ffftsize] [-Oprename]\n\
\n\
Estimate the frequency dependent Q value from the the incident and the reflected phase. \n\
\n\
Options:\n\
    -I filename : sac file name\n\
    -C tmarker/t1/t2 : cut for reading sac. (5/-3.0/7.0)\n\
        int tmarker,      time mark in sac header \n\
                    0-9 -> tn; -5 -> b; -3 -> o; -2 -> a \n\
                    others refer to t=0. \n\
        float   t1      begin time is tmarker + t1 \n\
        float   t2      end time is tmarker + t2 \n\
    -L lag : the lag time between the incident and the reflected surface. \n\
    -W tb/te : the time window for the incident phase.  \n\
        \"tb\" abd \"te\" here is the absolut time value. \n\
    -T taper : the taper length(seconds). (0.1) \n\
    -S smooth : the frequency smooth width(Hz). (0.6) \n\
    -F fftsize : the length for FFT calculation. (256)\n\
    -O pre-name : the pre name for output. (-O is closed in default setting) \n\
        A set of filenames will be generated with the pre name of \"pre-name\" \n\
";

jobQinv::jobQinv(int ac, char * av[]) : d_lag(0.0) {
	std::string sacfile;
	int tmark   = 5;
	float t1sac = -3;
	float t2sac = 7;
	float t1    = 12.7;
	float t2    = 13.5;
	float lag   = 1.3;
	float taperLen = 0.1;
	float width    = 0.6;
	char *output;
	int fftsize = 256;
	if(ac == 1) {
		fprintf(stdout, HMSG, av[0]);
		exit(1);
	}
	for(int iopt = 1; iopt< ac; ++iopt) {
		if(av[iopt][0] == '-') {
			if(av[iopt][1] == 'I') {
				sacfile = std::string( &(av[iopt][2]) );
			} else if(av[iopt][1] == 'C') {
				sscanf(av[iopt], "-C%d/%f/%f", &tmark, &t1sac, &t2sac);
			} else if  (av[iopt][1] == 'W') {
				sscanf(av[iopt],  "-W%f/%f", &t1, &t2);
			} else if (av[iopt][1] == 'L') {
				sscanf(av[iopt],  "-L%f", &lag);
			} else if  (av[iopt][1] == 'T') {
				sscanf(av[iopt],  "-T%f", &taperLen);
			} else if  (av[iopt][1] == 'S') {
				sscanf(av[iopt],  "-S%f", &width);
			} else if  (av[iopt][1] == 'F') {
				sscanf(av[iopt],  "-F%d", &fftsize);
			} else if  (av[iopt][1] == 'O') {
				output = &(av[iopt][2]);
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
	//#define FFTSIZE 256
	setFFTsize( get2Len(fftsize) );
	//#undef FFTSIZE
	DPRINT("%s", output);
	DPRINT("%s", sacfile.c_str());
	initQjob(sacfile, tmark, t1sac, t2sac);
	d_t1 = t1;
	d_t2 = t2;
	d_lag = lag;
	init(taperLen, width);
	setOutput(output);
}

jobQinv::jobQinv(const std::string& sacfile, int tmark, float t1sac, float t2sac, 
	float t1, float t2, float lag,
	float taperLen, float width, int fftsize)
	: d_t1(t1), d_t2(t2), d_lag(lag)
{
	//#define FFTSIZE 256
	setFFTsize(get2Len(fftsize));
	//#undef FFTSIZE
	initQjob(sacfile, tmark, t1sac, t2sac);
	init(taperLen, width);
}

int jobQinv::init(float taperLen, float width) {
	d_t3 = d_t1 + d_lag;
	d_t4 = d_t2 + d_lag;
	d_idx1 = d_attentrace.d_sac.floorIdx(d_t1);
	d_idx2 = d_attentrace.d_sac.ceilIdx(d_t2);
	d_idx3 = d_attentrace.d_sac.floorIdx(d_t3);
	d_idx4 = d_attentrace.d_sac.ceilIdx(d_t4);
	d_idxLag = d_idx3 - d_idx1;

	d_taperLen        = taperLen;
	d_freqsmoothwidth = width   ;
	return 0;
}

int jobQinv::run() {
	d_attentrace.make(d_idx1, d_idx2, d_idxLag, d_taperLen);
	d_attentrace.runSpec();
	d_attentrace.green();
	d_attentrace.qinv();
	
	d_attentrace.outFreqGreen(d_prename+".FreqGreen"); // out green
	d_attentrace.outFreqQinv(d_prename+".FreqQinv"); // out qinv
	d_attentrace.outFreqGreenAmp(d_prename+".FreqGreenamp"); // out green abs
	d_attentrace.outFreqGreenPhase(d_prename+".FreqGreenphase"); // out green phase

	d_attentrace.runPow();
	d_attentrace.d_src.outFreqPow(d_prename+".inc.FreqPow"); // out the spectrum of trace 1
	d_attentrace.d_src.outTimeTrace(d_prename+".inc.TimeTrace"); // out the trace 1
	d_attentrace.d_rec.outFreqPow(d_prename+".ref.FreqPow"); // out tht spectrum of trace 2
	d_attentrace.d_rec.outTimeTrace(d_prename+".ref.TimeTrace"); // out the trace 2
	//d_attentrace.d_src.outPow(d_prename+".inc.FreqPow"); // out the spectrum of trace 1

	d_attentrace.smoothPow(d_freqsmoothwidth);
	d_attentrace.runAmpRatio();
	d_attentrace.outFreqAmpratio(d_prename+".smooth.FreqAmpRatio"); // out amp ratio
	return 0;
}
