#include <cstdlib>
#include <cstdio>

#ifndef __TEST__
#define __TEST__


#ifdef DEGBUG
#define DPRINT(fmt, args...) fprintf(stderr, "[DPRINT...][%s %d] "fmt"\n", __FILE__, __LINE__, ##args); 

#else
#define DPRINT(fmt, ...)  
#endif

#endif