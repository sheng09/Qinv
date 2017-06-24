#include "tracef1d.hh"
#include "sac.h"
#include "test.hh"

int main(int argc, char *argv[])
{
	DPRINT("tracef1d(sacfile, tmarker, t1, t2)");
	tracef1d tr("../exam/data/VEL.SHT.SAC", -9999, 13.506775, 14.716775);
	tr.taper(5);
	tr.outTimeTrace("tmp.test_tracef1d.subtr1");

	DPRINT("tracef1d(size, dt, start)");
	tracef1d tr_empty(tr.size(), tr.getDelta(), tr.getStart() );
	tr_empty.outTimeTrace("tmp.test_tracef1d.empty");

	return 0;
}