#!/usr/bin/env python3
import sys
import subprocess, getopt
def main(argv):
	__OPTS = 'C:T:S:'
	dict_opt = {}
	try:
		opts, args = getopt.getopt(argv, __OPTS+'A:P:'+'Hh')
			#['inlst=','cut=','lag=','mingap=','winopt=', 'taper=', 'freq=','smooth=','out=','help','HELP'])
	except getopt.GetoptError:
		print('err opts ...')
		sys.exit(-1)

	for opt, arg in opts:
		if opt in ('-A'):
			file = arg
		elif opt in ('-P'):
			prename = arg
		elif opt in ('-H', '-h'):
			sys.exit(1)
		else:
			dict_opt[opt[1]] = arg

	for line in open(file).readlines():
		info = line.split(',')
		sac = info[0]
		pre = info[ 9].split(':')[1].replace(' ','')
		suf = info[10].split(':')[1].replace(' ','')
		lag = info[11].split(':')[1].replace(' ','')
		#
		dict_opt['O'] = prename + sac.split('/')[-1]
		#
		command = ['qinv', '-I%s' % sac, '-L%s' % lag, '-W%s/%s' % (pre, suf) ]
		command += [ '-'+key+value  for key,value in dict_opt.items() ]
		#
		print('Inverting Q for ' + sac + ' ...')
		p = subprocess.Popen(command)
		p.wait() # Comment this for parallel computation




if __name__ == '__main__':
	main(sys.argv[1:])
