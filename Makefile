export LIBDIR=../lib/
export BIN_DIR=../bin/
#SUBS = from_sac src py_src
SUBS = src py_src
SUBS_make    = $(foreach sub, ${SUBS}, ${sub}.make)
SUBS_clean   = $(foreach sub, ${SUBS}, ${sub}.clean)
SUBS_cleanobj   = $(foreach sub, ${SUBS}, ${sub}.cleanobj)
SUBS_install = $(foreach sub, ${SUBS}, ${sub}.install)



all: ${SUBS_make}






###
#  Sub Project
###
${SUBS_make}:%.make:%
	make -C $<

clean: ${SUBS_clean}
	-rm bin/*

${SUBS_clean}:%.clean:%
	-make -C $< clean

cleanobj: ${SUBS_cleanobj}

${SUBS_cleanobj}:%.cleanobj:%
	-make -C $< cleanobj

install: ${SUBS_install}

${SUBS_install}:%.install:%
	-make -C $< install