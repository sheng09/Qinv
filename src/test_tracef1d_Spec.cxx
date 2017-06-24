#include "tracef1d.hh"
#include "sac.h"
#include "test.hh"
#include "tracef1d_Spec.hh"

int main(int argc, char *argv[])
{
	tracef1d tr("../exam/data/VEL.SHT.SAC", -9999, 13.506775, 14.716775);
	tr.taper(5);
	tr.outTimeTrace("tmp.test_tracef1d.subtr1");

	tracef1d_Spec speZero(2048);
	speZero.setDelta(tr.getDelta() );
	speZero.assign(tr.getData(), tr.size() );
	speZero.dft();
	speZero.power();
	//speZero.mul(2.0);
	speZero.outFreqPow("tmp.test_tracef1d_Spec.2pow");
	speZero.outFreqSpec("tmp.test_tracef1d_Spec.spec");
	speZero.outTrace("tmp.test_tracef1d_Spec.trace");
	speZero.smoothPow3();
	speZero.outFreqPow("tmp.test_tracef1d_Spec.2pow.smooth3");
	speZero.power(); // refresh the power
	DPRINT("Do smoothPowN");
	speZero.smoothPowN(5);
	DPRINT("Output Freq Pow");
	speZero.outFreqPow("tmp.test_tracef1d_Spec.2pow.smooth5");
	DPRINT("fftwf_cleanup");
	fftwf_cleanup();

	const float *parzen = parzen_win(5);
	DPRINT("Parzen_win(5): %f %f %f %f %f", 
		parzen[0], parzen[1], parzen[2], parzen[3], parzen[4]);
	DPRINT("static parzen_win: %p", parzen);
	return 0;
}