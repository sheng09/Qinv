#include "winOpt.hh"

int main(int argc, char const *argv[])
{
	winOpt win;
	win.init(10.0, 0.01, 12, 15, -1, -0.5, 0.5, 1, 0.5);
	return 0;
}