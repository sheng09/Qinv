
PYLST = findBestWinSet.py qinvSet.py stackQ.py
subinstall = $(foreach py,${PYLST},$(subst .py,,${py}))
#BIN_DIR = ../bin

all: 
	# do nothing

${subinstall}:%:%.py
	cp $< ${BIN_DIR}

install: ${subinstall}

clean:
	#clean nothing
