#!/usr/bin/env python3
import sys, os
import subprocess, getopt
import glob

HMSG = "\n\
Usage:\n\
    %s -A bestwin.info -D directory -M maxAmp/maxPhase -O filename\n\
\n\
Verify all the inverted Q value, and stack.\n\
\n\
Options:\n\
    -A bestwin.info : the output file of \'findBestWinSet.py\'.\n\
    -D directory : the directory that contain all the inverted Qinv result files. \n\
    -M maxAmp/maxPhase: max amplitude and max phase for the transfer function. \n\
    -O output file prename.\n\
    -H|h : display this message.\n\
"

def main(argv):
	if(len(argv) == 0):
		print(HMSG % sys.argv[0])
		exit(1)
	__OPTS = 'M:O:'
	dict_opt = {}
	try:
		opts, args = getopt.getopt(argv, __OPTS+'D:A:Hh')
	except getopt.GetoptError:
		print('err opts ...')
		sys.exit(-1)
	for opt, arg in opts:
		if opt in ('-D'):
			directory = arg
		elif opt in ('-A'):
			bestwin = arg
		elif opt in ('-h', '-H'):
			print(HMSG % sys.argv[0])
			sys.exit(1)
		else:
			dict_opt[opt] = arg

	dict_opt['-O'] = dict_opt['-O'] + '.stacked_FreqQinv'
	lst = [i.replace('.FreqQinv', '') for i in glob.glob( directory+'/*.FreqQinv' )]
	fp = open('tmp.stackQ.in', 'w')
	print( '\n'.join(lst), file = fp )
	fp.close()
	lag = [ float(line.split(',')[4].replace(' lag ','').replace(' ','')) for line in open(bestwin) ]
	optlag = ( sum(lag)/len(lag) )

	dict_opt['-A'] = 'tmp.stackQ.in'
	dict_opt['-T'] = '%f' % optlag
	#print(dict_opt)
	#sys.exit(0)
	for value in dict_opt.values():
		if not value:
			print(HMSG % sys.argv[0])
			sys.exit(-1)

	nlines = []
	for i, filename in enumerate( open(dict_opt['-A'], 'r')):
		#print(filename)
		#nlines [i] = .read().counter('\n')
		nlines += [sum(1 for line in open(filename.strip()+'.FreqQinv') )]
		#print( nlines[i], filename )

	#print( nlines )
	dict_opt['-N'] = str( max(nlines) )
	command = ['stackQ']
	command += [ key+value for key,value in dict_opt.items() ]

	#print( ' '.join(command) )
	p = subprocess.Popen(command)
	p.wait()
	os.remove('tmp.stackQ.in')

if __name__ == '__main__':
	main(sys.argv[1:])
