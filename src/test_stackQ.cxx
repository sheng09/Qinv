#include "stackQinvTrace.hh"
#include "qkeyqinvTrace.hh"
int main(int argc, char const *argv[])
{
	//std::string in();
	stackQinv stack("../exam/data/sacfile.lst", 200, 1.3);
	stack.fill();
	stack.makeValid(50.0, 0.99);
	stack.aveSub();
	stack.output1st("tmp.test_stackQ");
	return 0;
}