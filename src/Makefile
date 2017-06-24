SRC = tracef1d.cxx tracef1d_Spec.cxx cross_Spec.cxx test.cxx \
		atten.cxx winOpt.cxx q_job.cxx jobQinv.cxx jobwin.cxx \
		la_trace.cxx  qkeyqinvTrace.cxx \
		stackQinvTrace.cxx io_format.cxx ranStack.cxx \
		parMot.cxx
		#filter.cxx
OBJ = $(foreach src,${SRC},$(subst .cxx,.o,${src}))

SRC2 = sacio.c
OBJ2 = $(foreach src,${SRC2},$(subst .c,.o,${src}))

SRC3 = test_tracef1d.cxx test_tracef1d_Spec.cxx test_crossSpec.cxx \
	 test_atten.cxx test_qjob.cxx findBestWin.cxx test_winopt.cxx \
	 test_findBestWin.cxx test_qinv.cxx qinv.cxx test_stackQ.cxx \
	 stackQ.cxx test_ranStack.cxx polarizeP.cxx test_polarize.cxx
OBJ3 = $(foreach src,${SRC3},$(subst .cxx,.o,${src}))
EXEC = $(foreach src,${SRC3},$(subst .cxx,,${src}))

CXX = g++
INC = -I../from_sac/inc -L../lib -L../from_sac
DEBUG = -DDEGBUG -Wall -g
#DEBUG = -O3
DEBUG = -Ofast

all: ${OBJ} ${OBJ2} ${EXEC}

test_tracef1d: test_tracef1d.o
	${CXX} -o $@ ${DEBUG} test_tracef1d.o tracef1d.o sacio.o io_format.o

test_tracef1d_Spec: test_tracef1d_Spec.o
	${CXX} -o $@  ${DEBUG} test_tracef1d_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

test_crossSpec: test_crossSpec.o
	${CXX} -o $@  ${DEBUG} test_crossSpec.o cross_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

test_atten: test_atten.o
	${CXX} -o $@ ${DEBUG} test_atten.o atten.o cross_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

test_winopt: test_winopt.o
	${CXX} -o $@ ${DEBUG} test_winopt.o winOpt.o atten.o cross_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

test_qjob:test_qjob.o
	${CXX} -o $@ ${DEBUG} test_qjob.o q_job.o winOpt.o atten.o cross_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

test_findBestWin: test_findBestWin.o
	${CXX} -o $@ ${DEBUG} test_findBestWin.o jobwin.o q_job.o winOpt.o atten.o cross_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

findBestWin: findBestWin.o
	${CXX} -o $@ ${DEBUG} findBestWin.o jobwin.o q_job.o winOpt.o atten.o cross_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

test_qinv: test_qinv.o
	${CXX} -o $@ ${DEBUG} test_qinv.o jobQinv.o q_job.o winOpt.o atten.o cross_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

qinv: qinv.o
	${CXX} -o $@ ${DEBUG} qinv.o jobQinv.o q_job.o winOpt.o atten.o cross_Spec.o tracef1d_Spec.o tracef1d.o sacio.o io_format.o -lm -lfftw3f

test_stackQ: test_stackQ.o
	${CXX} -o $@ ${DEBUG} test_stackQ.o qkeyqinvTrace.o stackQinvTrace.o io_format.o -lm

stackQ: stackQ.o
	${CXX} -o $@ ${DEBUG} stackQ.o qkeyqinvTrace.o stackQinvTrace.o io_format.o -lm

test_ranStack: test_ranStack.o
	${CXX} -o $@ ${DEBUG} test_ranStack.o ranStack.o tracef1d.o sacio.o -lm

test_polarize: test_polarize.o
	${CXX} -o $@ ${DEBUG} test_polarize.o tracef1d.o sacio.o parMot.o -lm

polarizeP: polarizeP.o
	${CXX} -o $@ ${DEBUG} polarizeP.o tracef1d.o sacio.o parMot.o -lm

${OBJ}:%.o:%.cxx
	${CXX} -c -o $@ ${INC} ${DEBUG} $<

${OBJ2}:%.o:%.c
	${CXX} -c -o $@ ${INC} ${DEBUG} $<

${OBJ3}:%.o:%.cxx
	${CXX} -c -o $@ ${INC} ${DEBUG} $<


clean:
	rm ${OBJ2} ${OBJ} ${OBJ3} ${EXEC}

cleanobj:
	rm ${OBJ2} ${OBJ} ${OBJ3}

install:
	cp ${EXEC} ${BIN_DIR}
