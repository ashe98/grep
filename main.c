#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "line.h"
#include "optqueue.h"
int main(int argc, char *argv[]) {
	int opt, fi, fr, ff, fw, fe, fv, fH, fh, fb, fc, fm, fq;
	fi = fr = ff = fw = fe = fv = fH = fh = fb = fc = fm = fq = 0;
	int mNUM;
	char *file1, *file2, *pattern;
	optqueue o;
	oqinit(&o);
	while((opt = getopt(argc, argv, "bcef:hHim:qrvw")) != -1) {
		switch(opt) {
			case 'i':
				fi = 1;
				break;
			case 'r':
				fr = 1;
				break;
			case 'f':
				ff = 1;
				file2 = optarg;
				break;
			case 'w':
				fw = 1;
				break;
			case 'v':
				fv = 1;
				break;
			case 'H':
				fH = 1;
				break;
			case 'h':
				fh = 1;
				break;
			case 'b':
				fb = 1;
				break;
			case 'c':
				fc = 1;
				break;
			case 'm':
				fm = 1;
				mNUM = atoi(optarg);
				break;
			default:
				fprintf(stderr, "error\n");
				return 0;
		}
	}
	if(fH)
		oenq(&o, 'H');
	if(fh)
		oenq(&o, 'h');
	if(fb)
		oenq(&o, 'b');
	if(fc)
		oenq(&o, 'c');
	if(fv)
		oenq(&o, 'v');
	if(fm) {
		oenq(&o, 'm');
		oenq(&o, mNUM + '0');
	}
	
	return 0;
}
