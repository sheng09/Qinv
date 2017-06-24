#include "atten.hh"
#include "test.hh"
int main(int argc, char const *argv[])
{
	attenTrace att("../exam/data/VEL.SHT.SAC", 5, -5, 5, 4096);

	int it1, it2;
	float t1 = att.d_sac.floorCrossZero( att.d_sac.getTinc(), &it1 );
	DPRINT("%f", t1);
	float t2 = att.d_sac.ceilCrossZero(  att.d_sac.getTinc(), &it2 );
	DPRINT("%f", t2);

	t1 -= 0.5;
	t2 += 0.5;
	float lag = att.d_sac.getTlagRef();
	float t3 = t1 + lag, t4 = t2 + lag;
	int i1 = att.d_sac.roundIdx(t1);
	int i2 = att.d_sac.roundIdx(t2);
	int i3 = att.d_sac.roundIdx(t3);
	//int i4 = att.d_sac.roundIdx(t4);
	DPRINT("inc:[%f %f] ref:[%f %f]", t1, t2, t3, t4);
	att.make(i1, i2, i3-i1, 0.1);
	float c1 = att.cor();
	//part 1:
	DPRINT("Part [runSpec()]");
	att.runSpec();
	DPRINT("Part [runPow()]");
	att.runPow();

	//part
	att.smoothPow(1.5);
	float c2 = att.coef(1.0, 10.0);
	DPRINT("cor:%f coef:%f", c1, c2);

	//
	att.runPow();
	DPRINT("trace and spectrum are output to files: tmp.test_atten...");
	att.d_src.outTrace("tmp.test_atten.trx");
	att.d_src.outFreqPow("tmp.test_atten.FreqPowx");
	att.d_rec.outTrace("tmp.test_atten.try");
	att.d_rec.outFreqPow("tmp.test_atten.FreqPowy");
	att.d_sxy.outFreqCrossSpec("tmp.test_atten.FreqSxy");
	att.d_sxy.outPow2("tmp.test_atten.sxypow2");

	// part 2:
	att.green();
	att.qinv();
	att.outFreqGreen("tmp.test_atten.FreqGreen");
	att.outFreqGreenPhase("tmp.test_atten.FreqGreenPhase");
	att.outFreqGreenAmp("tmp.test_atten.FreqGreenAmp");
	att.outFreqQinv("tmp.test_atten.FreqQinv");

	// part 3:
	att.runAmpRatio();
	att.outFreqAmpratio("tmp.test_atten.FreqAmpratio");

	att.smoothPow(1.5);
	att.runAmpRatio();
	att.outFreqAmpratio("tmp.test_atten.FreqAmpratio.smooth(1.5Hz)");

	fftwf_cleanup();
	return 0;
}