#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "optqueue.h"
#include "line.h"

int main(int argc, char *argv[]) {
	int opt, fi, fr, ff, fw, fe, fv, fH, fh, fb, fc, fm, fq;
	fi = fr = ff = fw = fe = fv = fH = fh = fb = fc = fm = fq = 0;
	int mNUM, filepos = 1;
	char *file1, *file2, *word = "this";
	optqueue o;
	Lines l;
	FILE *fp;
	oqinit(&o);
	initLines(&l);
	while((opt = getopt(argc, argv, "bcef:hHim:qrvw")) != -1) {
		switch(opt) {
			case 'i':
				fi = 1;
				filepos++;
				break;
			case 'r':
				fr = 1;
				filepos++;
				break;
			case 'f':
				ff = 1;
				file2 = optarg;
				break;
			case 'w':
				fw = 1;
				filepos++;
				break;
			case 'v':
				fv = 1;
				filepos++;
				break;
			case 'H':
				fH = 1;
				filepos++;
				break;
			case 'h':
				fh = 1;
				filepos++;
				break;
			case 'b':
				fb = 1;
				filepos++;
				break;
			case 'c':
				fc = 1;
				filepos++;
				break;
			case 'm':
				fm = 1;
				mNUM = atoi(optarg);
				filepos += 2;
				break;
			case 'q':
				fq = 1;
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
	fp = fopen("t2.txt", "r");
	size_t len = 0;
	char *line = NULL;
	while(getline(&line, &len, fp) != -1) {
		addLine(&l, line);
	}
	free(line);
	grepf(&l, o, 0, 1, "t1.txt", "t2.txt");
	destroyLines(&l);
	return 0;
}
