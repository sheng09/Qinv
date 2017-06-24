#include "parMot.hh"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

static char HSMGP[]="\
Usage:\n\
    %s -Nn.sac -Ee.sac -Zz.sac -Wb/e -Rr.sac -Tt.sac \n\
\n\
Invert Radial direction angle, and rotate to get R,T component.\n\
\n\
Options:\n\
    -N n.sac : NS component input sac filename.\n\
    -E e.sac : EW component input sac filename.\n\
    -Z z.sac : Z  component input sac filename.\n\
    -W b/e   : time window for P phase.\n\
\n\
    -R r.sac : radial component output sac filename.\n\
    -T t.sac : tangential component output sac filename.\n\
\n";

parMot::parMot(int ac, char *av[]) {
	int flag = 0;
	for(int iopt =1; iopt<ac; ++iopt) {
		if(av[iopt][0] == '-') {
			if(av[iopt][1] == 'N') {
				++flag;
				d_Nname = std::string( &(av[iopt][2]) );
			} else if(av[iopt][1] == 'E') {
				++flag;
				d_Ename = std::string( &(av[iopt][2]) );
			} else if(av[iopt][1] == 'Z') {
				++flag;
				d_Zname = std::string( &(av[iopt][2]) );
			} else if(av[iopt][1] == 'W') {
				++flag;
				sscanf(av[iopt], "-W%f/%f", &d_wb, &d_we);
			} else if(av[iopt][1] == 'R') {
				++flag;
				d_Rname = std::string( &(av[iopt][2]) );
			} else if(av[iopt][1] == 'T') {
				++flag;
				d_Tname = std::string( &(av[iopt][2]) );
			} else if(av[iopt][1] == 'H') {
				fprintf(stderr, HSMGP, av[0]);
				exit(1);
			} else {
				std::cerr << "Unrecognized argument: " << av[iopt] << "\n";
				exit(-1);
			}
		} else {
			std::cerr << "Unrecognized argument: " << av[iopt] << "\n";
			exit(-1);
		}
	}
	if(flag < 6) {
		fprintf(stderr, HSMGP, av[0]);
		exit(1);
	}
	init();
}
parMot::parMot(		const std::string& Nname,
					const std::string& Ename,
					const std::string& Zname,
					const std::string& Rname,
					const std::string& Tname,
					float wb, float we
				)
	:  d_wb(wb), d_we(we),
	d_Nname(Nname), d_Ename(Ename), d_Zname(Zname), d_Rname(Rname), d_Tname(Tname)
{
	init();
}
int parMot::init() {
	d_n.initSac(d_Nname);
	d_e.initSac(d_Ename);
	d_z.initSac(d_Zname);
	if (!d_n.checkTime(d_wb)) {
		fprintf(stderr, "Warning: time window B is out of the sac range." );
		fprintf(stderr, " Revise from %f to %f.\n", d_wb,  d_n.getSacB() );
		d_wb = d_n.getSacB();
	}
	if (!d_n.checkTime(d_we)) {
		fprintf(stderr, "Warning: time window E is out of the sac range.\n" );
		fprintf(stderr, " Revise from %f to %f.\n", d_we,  d_n.getSacE() );
		d_we = d_n.getSacE();
	}
	d_r.alloc(d_n.size());
	d_t.alloc(d_n.size());
	return 0;
}
int parMot::rotate2gcp() {
	std::vector<float> &n = d_n.getDataVec();
	std::vector<float> &e = d_e.getDataVec();
	std::vector<float> &z = d_z.getDataVec();
	std::vector<float> &r = d_r.getDataVec();
	std::vector<float> &t = d_t.getDataVec();
	for(int i=0;i<d_n.size(); ++i) {
		r[i] =  e[i] * d_Rsin + n[i] * d_Rcos;
		t[i] =  e[i] * d_Rcos - n[i] * d_Rsin;
	}
	return 0;
}

int parMot::p_diag1() {
	float nn =0.0, en =0.0, ee =0.0;
	float *n = d_n.getData();
	float *e = d_e.getData();
	int ib = d_n.floorIdx(d_wb);
	int ie = d_e.ceilIdx(d_we);
	for(int i=ib; i<=ie; ++i) {
		nn += n[i] * n[i];
		en += n[i] * e[i];
		ee += e[i] * e[i];
	}
	float sum, cs, sn;
	/*
      N ^
        |      R
        |     /
        |    /
        |   /
        |a /
        | / theta
    ----+--------------> E
        |
        |
        |
        | a is the azimuth angle for R component
        |
    baz = a + 180

    e = sin(a)
    n = cos(a)
    ee = sin^2
    nn = cos^2
    en = cos sin
                  cos                     cos^2
    cos =  ------------------- = -----------------------
            sqrt(cos^2+sin^2)     sqrt(cos^4+sin^2cos^2)

                  nn
        =  -------------------
            sqrt(nn^2+en^2)

    OR:
                  cos                     cos* sin
    cos =  ------------------- = -----------------------
            sqrt(cos^2+sin^2)     sqrt(sin^4+sin^2cos^2)

                  en
        =  -------------------
            sqrt(ee^2+en^2)
	*/
	if(ee>nn) {
		sum = sqrt(ee*ee + en*en);
		cs  = en/sum;
		sn  = ee/sum;
	} else {
		sum = sqrt(nn*nn + en*en);
		cs  = nn /sum;
		sn  = en /sum;
	}
	d_Rcos = cs;
	d_Rsin = sn;
	d_baz = atan2f(d_Rsin, d_Rcos) + PI;
	return 0;
}
int parMot::outRT() {
	SACHEAD hdr = d_z.getSacHdr();
	hdr.kcmpnm[0] = 'T';
	hdr.kcmpnm[1] = '\0';
	d_t.setHdr(hdr);
	d_t.outSac(d_Tname);

	hdr.kcmpnm[0] = 'R';
	hdr.kcmpnm[1] = '\0';
	d_r.setHdr(hdr);
	d_r.outSac(d_Rname);
	outInfo();
	return 0;
}
int parMot::outInfo() {
	std::cout << "BAZ: " << d_baz/PI*180.0 << " (DEG)\n";
	return 0;
}
int parMot::run() {
	p_diag1();
	rotate2gcp();
	outRT();
	return 0;
}