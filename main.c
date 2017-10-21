#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "optqueue.h"
#include "line.h"
int fi, fr, ff, fw, fv, fH, fh, fb, fc, fm, fq;
char *word = NULL;
optqueue o;
void grepr(char *dirname) {
	DIR *dir;
	FILE *fp;
	struct stat st;
	Lines l;
	size_t len = 0;
	char *line = NULL;
	initLines(&l);
	struct dirent *d;
	char temp[256] = "", *tmp = NULL;
	if((dir = opendir(dirname)) == NULL)
		return;
	while((d = readdir(dir)) != NULL) {
		if(d->d_type == DT_DIR) {
			if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
				continue;
			strcat(temp, dirname);
			strcat(temp, "/");
			strcat(temp, d->d_name);
			grepr(temp);
			strcpy(temp, "");
		}
		else {
			if(d->d_type != DT_REG)
				continue;
			strcat(temp, dirname);
			strcat(temp, "/");
			strcat(temp, d->d_name);
			if(stat(temp, &st) == 0 && st.st_mode & S_IXUSR)
				continue;
			fp = fopen(temp, "r");
			if(!fp)
				continue;		
			while(getline(&line, &len, fp) != -1) {
				addLine(&l, line);
			}
			if(line) {
				free(line);
				line = NULL;
				len = 0;
			}
			fclose(fp); 	
			if(fw)
				grepw(&l, word, fi);
			else
				grep(&l, word, fi);
			if(temp[0] == '.') {
				tmp = temp + 2;
				printLines(l, o, word, tmp, 0);
			}
			else
				printLines(l, o, word, temp, 0);
			strcpy(temp, "");
			tmp = NULL;
			destroyLines(&l);
		}
	}
	closedir(dir);
}
int main(int argc, char *argv[]) {
	DIR *dir;
	int mNUM, filepos = 1, opt, i, s;
	char *file2, *line = NULL;;
	size_t len = 0;
	struct stat st;
	fi = fr = ff = fw = fv = fH = fh = fb = fc = fm = fq = 0;
	Lines l;
	FILE *fp;
	oqinit(&o);
	initLines(&l);
	while((opt = getopt(argc, argv, "bcf:hHim:qrvw")) != -1) {
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
				filepos += 2;
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
				filepos++;
				break;
			default:
				fprintf(stderr, "Usage: grep [OPTION]... PATTERN [FILE]...\n");
				return 0;
		}
	}
	if(argc == 1) {
			printf("Usage: grep [OPTION]... PATTERN [FILE]...\n");
			return 0;
	}			
	if((filepos + 1 != argc) && ff)
		fH = 1;
	if(filepos + 2 != argc && ff == 0)
		fH = 1;
	if(fr)
		fH = 1;
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
	if(fr && fq)
		return 0;
	word = argv[filepos];
	if(ff) {
		for(i = filepos; i < argc; i++) {
			fp = fopen(argv[i], "r");
			if(fp == NULL) {
				fprintf(stderr, "%s : No such file or directory\n", argv[i]);
				continue;
			}
			while(getline(&line, &len, fp) != -1) {
				addLine(&l, line);
			}
			if(line) {
				free(line);
				line = NULL;
				len = 0;
			}
			grepf(&l, fw, fi, file2);
			if(fq) {
				s = search(&l);
				return s;
			}
			printLines(l, o, NULL, argv[i], ff);
			destroyLines(&l);
			fclose(fp);
		}
		return 0;
	}
	
	if(fr) {
		if(argc == 2) {
			printf("Usage: grep [OPTION]... PATTERN [FILE]...\n");
			return 0;
		}
		if(filepos + 1 == argc)
			grepr(".");
		else {
			if((dir = opendir(argv[filepos + 1])) == NULL) {
				stat(argv[filepos + 1], &st);
				if(!S_ISREG(st.st_mode)) {
					fprintf(stderr, "%s : No such file or directory\n", argv[filepos + 1]);
					return 0;
				}
			}
			else {
				grepr(argv[filepos + 1]);
				return 0;
			}
		}
	}	
	for(i = filepos + 1; i < argc; i++) {
		if(fw) {
			fp = fopen(argv[i], "r");
			if(fp == NULL) {
				fprintf(stderr, "%s : No such file or directory\n", argv[i]);
				continue;
			}
			while(getline(&line, &len, fp) != -1) {
				addLine(&l, line);
			}
			if(line) {
				free(line);
				line = NULL;
				len = 0;
			}
			grepw(&l, word, fi);
			if(fq) {
				s = search(&l);
				return s;
			}
			printLines(l, o, word, argv[i], ff);
			destroyLines(&l);
			fclose(fp);
		}
		else {
			fp = fopen(argv[i], "r");
			if(fp == NULL) {
				fprintf(stderr, "%s : No such file or directory\n", argv[i]);
				continue;
			}
			while(getline(&line, &len, fp) != -1) {
				addLine(&l, line);
			}
			if(line) {
				free(line);
				line = NULL;
				len = 0;
			}
			grep(&l, word, fi);
			if(fq) {
				s = search(&l);
				return s;
			}
			printLines(l, o, word, argv[i], ff);
			destroyLines(&l);
			fclose(fp);
		}
	}
	return 0;
}
