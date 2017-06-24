#include "stackQinvTrace.hh"
#include "qkeyqinvTrace.hh"
int main(int argc, char *argv[])
{
	//std::string in();
	stackQinv stack(argc, argv);
	stack.run();
	return 0;
}