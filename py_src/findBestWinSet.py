#!/usr/bin/env python
import subprocess
import sys, getopt

HMSG = " \n\
Usage:  \n\
    %s -A filelist [-O infooutput]  [-C tmarker/t1/t2] [-L [F]lag] \n\
       [-X mingap] [-M maxlengh] [-W tb/te] [-T taper] [-F f1/f2] \n\
       [-S smooth] [-P prename]\n\
 \n\
Find the best time window, and the best lag time, for the incident and the reflected phase.  \n\
 \n\
Options: \n\
    -A filelst : sac file name \n\
        The \"t5\" and \"t6\" within sac header should be assigned for the arrival time of the  \n\
        incident and the reflected phase, respectively.  \n\
    -O infooutput : \n\
        Output file that contains all the informations. \n\
\n\
    -C tmarker/t1/t2 : cut for reading sac. (5/-3.0/7.0) \n\
        int tmarker,      time mark in sac header  \n\
                    0-9 -> tn; -5 -> b; -3 -> o; -2 -> a  \n\
                    others refer to t=0.  \n\
        float   t1      begin time is tmarker + t1  \n\
        float   t2      end time is tmarker + t2  \n\
    -L  lag : the lag search range will be [ -lag+t6-t5, lag+t6-t5]. (0.5) \n\
   [-LF]lag : fixed lag time between the incident and the reflected phase. \n\
    -M maxlength : the max length (seconds)  for each phases. (99999.9)\n\
    -X mingap : the min gap between the incident and reflected phases. (0.0) \n\
    -W tb/te : the window search range. (-1/1)  \n\
    -T taper : the taper length(seconds). (0.1)  \n\
    -F f1/f2 : the frequency band for inverting Fukushima's coefficient. (1.0/10.0)  \n\
    -S smooth : the frequency smooth width(Hz). (0.6)  \n\
    -P prename : the prename for output info files including \".coef\", \".cor\", etc. \n\
    -H display this message.\n\
"

def main(argv):
	__OPTS = 'C:L:X:W:T:M:F:S:'
	dict_opt = {i:'' for i in __OPTS if i is not ':' }
	#print(dict_opt)
	inputlst = ''
	outputlst = ''
	prepreinfo = ''
	try:
		opts, args = getopt.getopt(argv, __OPTS+'A:O:P:'+'Hh')
			#['inlst=','cut=','lag=','mingap=','winopt=', 'taper=', 'freq=','smooth=','out=','help','HELP'])
	except getopt.GetoptError:
		print('err opts')
		sys.exit(-1)
	for opt, arg in opts:
		if opt in ('-H', '-h', '--help', '--HELP'):
			print(HMSG % (sys.argv[0]))
			sys.exit(1);
		elif opt in ('-A'):
			inputlst = arg
		elif opt in ('-O'):
			outputlst = arg
		elif opt in ('-P'):
			prepreinfo = arg
		elif opt[1] in dict_opt:
			dict_opt[opt[1]] = arg
		else:
			print('unrecognized opt: '+arg)
			exit(-1)

	foutput = open(outputlst, 'w')
	command = ['findBestWin']
	command += ['-'+key+value for key,value in dict_opt.items() if value]
	for line in open(inputlst).readlines():
		filename = line.strip()
		if filename:
			print(filename + ' processing ...')
			InfoPrename = prepreinfo + filename.split("/")[-1] + '.infowin2d'
			#print(InfoPrename)
			p = subprocess.Popen( command + ['-I'+filename] + ['-O'+InfoPrename], stdout = subprocess.PIPE)
			p.wait()
			foutput.write( '%s, %s, %s \n' % (filename, InfoPrename, p.stdout.read().replace('\n',', ')) )

if __name__ == "__main__":
	if(len(sys.argv) <= 1):
		print(HMSG % (sys.argv[0]))
		exit(1)
	main(sys.argv[1:])


# ./findBestWinSet.py -A tmp  -C 5/-5/7 -L F1.28 -X 0.2 -W -1/1 -T 0.05 -F 1/10 -S 1.5 -O ooo