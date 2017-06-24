#include "tracef1d.hh"
#include "sac.h"
#include "test.hh"
#include "tracef1d_Spec.hh"
#include "cross_Spec.hh"

int main(int argc, char *argv[])
{
	tracef1d tr("../exam/data/VEL.SHT.SAC", -9999, 13.506775, 14.716775);
	tr.taper(5);
	tr.outTimeTrace("tmp.test_crossSpec.subtr1");

	tracef1d_Spec speZero(4096);
	speZero.assign(tr.getData(), tr.size() );
	speZero.dft();
	speZero.power();

	crossSpec cross(speZero.sizeT());
	cross.cross(speZero, speZero);
	cross.power2();
	cross.outPow2("tmp.test_crossSpec.Pow2");
	cross.outCrossSpec("tmp.test_crossSpec.cross");
	fftwf_cleanup();
	return 0;
}