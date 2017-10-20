#define _GNU_SOURCE 
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include "optqueue.h"
#include "line.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BOLDRED     	   "\033[1m\033[31m"

void initLines(Lines *l) {
	l->head = NULL;
	l->tail = NULL;
}
void addLine(Lines *l, char *s) {
	node *x;
	int len;
	if(!s)
		return;
	x = (node*)malloc(sizeof(node));
	if(!x)
		return;
	x->str = (char*)malloc(strlen(s) + 1);
	if(x->str == NULL) {
		free(x);
		return;
	}
	strcpy(x->str, s);
	len = length(*l);
	if(len == 0) {
		l->head = l->tail = x;
		x->prev = x->next = NULL;
		return;
	}
	x->next = NULL;
	x->occount = 0;
	x->prev = l->tail;
	l->tail->next = x;
	l->tail = x;
}
void destroyLines(Lines *l) {
	node *tmp = l->head->next;
	while(l->head != NULL) {
		if(tmp == NULL) {
			free(l->tail->str);
			free(l->tail);
			l->head = l->tail = NULL;
			break;
		}
		else {
			free(tmp->prev->str);
			free(tmp->prev);
			tmp = tmp->next;
		}
	}
}
int length(Lines l) {
	node *tmp;
	tmp = l.head;
	int count = 0;
	while(tmp != NULL) {
		count++;
		tmp = tmp->next;
	}
	return count;
}
/*Finds the position of occurrence of *word in each node of Lines l
 *and stores the position in occ[] array of the node. Stores positions
 *of multiple occurrences in each node if found in occ[]
 */
void grep(Lines *l, char *word, int fi) {
	node *tmp;
	char *text = NULL, *ptr = NULL, *found = NULL;
	int i = 0, j = 0, start = 0;
	tmp = l->head;
	if(fi) {
		while(tmp != NULL) {
			text = tmp->str;
			ptr = text;
			do {
				found = strcasestr(ptr, word);
				if(found != NULL) {
					tmp->occ[tmp->occount] = found - text;
					(tmp->occount)++;
					ptr = found + strlen(word);
				}
			}while(found != NULL);
			if(tmp->occount == 0)
				tmp->occ[0] = -1;
			tmp = tmp->next;
		}
	}
	else {
		while(tmp != NULL) {
			text = tmp->str;
			while(text[j]) {
				while(text[j] && word[i]) {
					if(text[j] == word[i]) {
						j++;
						i++;
					}
					else {
						j = j - (i - 1);
						i = 0;
					}
				}
				if(word[i] == '\0') {
						start = j - (i - 1) - 1;
						tmp->occ[tmp->occount] = start;		
						(tmp->occount)++;
						j = start + 1;
						i = 0;
				}
			}
			if(tmp->occount == 0)
				tmp->occ[0] = -1; 
			tmp = tmp->next;
			i = 0;
			j = 0;
		}
	}
}
/*Works the same as grep() function but checks if *word is not a 
 *character sequence by checking if it is both preceded and succeeded
 *by a non-word constituent
 */
void grepw(Lines *l, char *word, int fi) {
	int i = 0;
	node *tmp;
	char *text = NULL, *ptr = NULL, *found = NULL;
	tmp = l->head;
	if(fi) {
		while(tmp != NULL) {
			text = tmp->str;
			ptr = text;
			do {
				found = strcasestr(ptr, word);
				if(found != NULL) {
					if(found[strlen(word)] == '\n' || (!isdigit(found[strlen(word)]) && !isalpha(found[strlen(word)])) && found[strlen(word)] != '_') {
						if(found == text) {
							tmp->occ[tmp->occount] = found - text;
							(tmp->occount)++;
						}
						else {
							if(!isdigit(found[-1]) && !isalpha(found[-1]) && found[-1] != '_') {
								tmp->occ[tmp->occount] = found - text;
								(tmp->occount)++;
							}
						}
					}
					ptr = found + strlen(word);
				}
			}while(found != NULL);
			if(tmp->occount == 0)
				tmp->occ[0] = -1;
			tmp = tmp->next;
		}
	}
	else {
		while(tmp != NULL) {
			text = tmp->str;
			ptr = text;
			do {
				found = strstr(ptr, word);
				if(found != NULL) {
					if(found[strlen(word)] == '\n' || (!isdigit(found[strlen(word)]) && !isalpha(found[strlen(word)])) && found[strlen(word)] != '_') {
						if(found == text) {
							tmp->occ[tmp->occount] = found - text;
							(tmp->occount)++;
						}
						else {
							if(!isdigit(found[-1]) && !isalpha(found[-1])) {
								tmp->occ[tmp->occount] = found - text;
								(tmp->occount)++;
							}
						}
					}
					ptr = found + strlen(word);
				}
			}while(found != NULL);
			if(tmp->occount == 0)
				tmp->occ[0] = -1;
			tmp = tmp->next;
		}
	}
}
/*Finds the position of occurence of the pattern and the position 
 *of the end of the pattern in the node
 *and then stores these two positions in the occ[] array of node
 *The user need not call printLines()
 */
void grepf(Lines *l, int fw, int fi, char *file2) {
	FILE *fp1;
	size_t len = 0;
	node *tmp;
	int i, j, start;
	char *word = NULL, *text = NULL, *ptr = NULL, *found = NULL;
	fp1 = fopen(file2, "r");
	if(fp1 == NULL) {
		fprintf(stderr, "%s : No such file or directory\n", file2);
		exit(0);
	}
	if(fw) {
		while(getline(&word, &len, fp1) != -1) {
			text = NULL, ptr = NULL, found = NULL;
			word[strlen(word) - 1] = '\0';
			i = 0;
			tmp = l->head;
			if(fi) {
				while(tmp != NULL) {
					text = tmp->str;
					ptr = text;
					do {
						found = strcasestr(ptr, word);
						if(found != NULL) {
							if(found[strlen(word)] == '\n' || (!isdigit(found[strlen(word)]) && !isalpha(found[strlen(word)]))) {
								if(!isdigit(found[-1]) && !isalpha(found[-1])) {
									tmp->occ[tmp->occount] = found - text;
									(tmp->occount)++;
									tmp->occ[tmp->occount] = found + strlen(word) - text - 1;
									(tmp->occount)++;
								}
							}
							ptr = found + strlen(word);
						}
					}while(found != NULL);
					if(tmp->occount == 0)
						tmp->occ[0] = -1;
					tmp = tmp->next;
				}
			}
			else {
				while(tmp != NULL) {
					text = tmp->str;
					ptr = text;
					do {
						found = strstr(ptr, word);
						if(found != NULL) {
							if(found[strlen(word)] == '\n' || (!isdigit(found[strlen(word)]) && !isalpha(found[strlen(word)]))) {
								if(!isdigit(found[-1]) && !isalpha(found[-1])) {
									tmp->occ[tmp->occount] = found - text;
									(tmp->occount)++;
									tmp->occ[tmp->occount] = found + strlen(word) - text - 1;
									(tmp->occount)++;
								}
							}
							ptr = found + strlen(word);
						}
					}while(found != NULL);
					if(tmp->occount == 0)
						tmp->occ[0] = -1;
					tmp = tmp->next;
				}
			}
		}
		free(word);
	}
	else {
		while(getline(&word, &len, fp1) != -1) {
			i = 0, j = 0, start = 0;
			word[strlen(word) - 1] = '\0';
			tmp = l->head;
			if(fi) {
				while(tmp != NULL) {
					text = tmp->str;
					ptr = text;
					do {
						found = strcasestr(ptr, word);
						if(found != NULL) {
							tmp->occ[tmp->occount] = found - text;
							(tmp->occount)++;
							tmp->occ[tmp->occount] = found + strlen(word) - text - 1;
							(tmp->occount)++;
							ptr = found + strlen(word);
						}
					}while(found != NULL);
					if(tmp->occount == 0)
						tmp->occ[0] = -1;
					tmp = tmp->next;
				}
			}
			else {
				while(tmp != NULL) { 
					text = tmp->str;
					while(text[j]) {
						while(text[j] && word[i]) {
							if(text[j] == word[i]) {
								j++;
								i++;
							}
							else {
								j = j - (i - 1);
								i = 0;
							}
						}
						if(word[i] == '\0') {
								start = j - (i - 1) - 1;
								tmp->occ[tmp->occount] = start;
								(tmp->occount)++;
								tmp->occ[tmp->occount] = start + strlen(word) - 1;
								j = start + 1;
								i = 0;
						}
					}
					if(tmp->occount == 0)
						tmp->occ[0] = -1; 
					tmp = tmp->next;
					i = 0;
					j = 0;
				}
			}
		}
		free(word);
	}
}
int search(Lines *l) {
	node *tmp;
	tmp = l->head;
	while(tmp != NULL) {
		if(tmp->occ[0] != -1)
			return 0;
	}
	return 1;
}
void printLines(Lines l, optqueue oq, char *word, char *filename, int ff) {
	int H = 0, h = 0, b = 0, c = 0, v = 0, m = 0, mNUM = -1, count = 0, bytes = 0;
	node *tmp;
	char op = odeq(&oq);
	if(op == 'H') {
		H = 1;
		if(!oqempty(&oq))
			op = odeq(&oq);
	}
	if(op == 'h') {
		h = 1;
		if(!oqempty(&oq))
			op = odeq(&oq);
	}
	if(op == 'b') {
		b = 1;
		if(!oqempty(&oq))
			op = odeq(&oq);
	}
	if(op == 'c') {
		c = 1;
		if(!oqempty(&oq))
			op = odeq(&oq);
	}
	if(op == 'v') {
		v = 1;
		if(!oqempty(&oq))
			op = odeq(&oq);
	}
	if(op == 'm') {
		m = 1;
		if(!oqempty(&oq))
			mNUM = odeq(&oq) - '0';
	}
	
	if(c) {
		tmp = l.head;
		while(tmp != NULL) {
			if(tmp->occ[0] != -1)
				count++;
			tmp = tmp->next;
		}
		if(v) { 
			count = length(l) - count;
			if(H == 1 && h == 0)
				printf(ANSI_COLOR_MAGENTA  "%s:" ANSI_COLOR_RESET , filename);
			if(m)
				printf("%d\n", (mNUM < count) ? mNUM : count);
			else
				printf("%d\n", count);
			return;
		}
		else {
			if(H == 1 && h == 0)
				printf(ANSI_COLOR_MAGENTA  "%s:" ANSI_COLOR_RESET , filename);
			if(m)
				printf("%d\n", (mNUM < count) ? mNUM : count);
			else
				printf("%d\n", count);
			return;
		}
	}
	
	if(b) {
		tmp = l.head;
		if(v) {
			while(tmp != NULL && mNUM != 0) {
				if(tmp->occ[0] == -1) {
					if(H == 1 && h == 0)
						printf(ANSI_COLOR_MAGENTA  "%s:" ANSI_COLOR_RESET , filename);
					printf(ANSI_COLOR_GREEN  "%d:"  ANSI_COLOR_RESET, bytes);
					printf("%s", tmp->str);
					mNUM--;
				}
				bytes = bytes + strlen(tmp->str);
				tmp = tmp->next;
			}
		}
		else {
			while(tmp != NULL && mNUM != 0) {
				if(tmp->occ[0] != -1) {
					if(H == 1 && h == 0)
						printf(ANSI_COLOR_MAGENTA  "%s:" ANSI_COLOR_RESET , filename);
					printf(ANSI_COLOR_GREEN  "%d:"  ANSI_COLOR_RESET, bytes);
					if(ff)
						printMatchedf(*tmp);
					else
						printMatched(*tmp, strlen(word));
					mNUM--;
				}
				bytes = bytes + strlen(tmp->str);
				tmp = tmp->next;
			}
		}
		return;
	}
	tmp = l.head;
	while(tmp != NULL && mNUM != 0) {
		if(v) {	
			if(tmp->occ[0] == -1) {
				if(H == 1 && h == 0)
					printf(ANSI_COLOR_MAGENTA  "%s:" ANSI_COLOR_RESET , filename);
				printf("%s", tmp->str);
				mNUM--;
			}
		}
		else {
			if(tmp->occ[0] != -1) {	
				if(H == 1 && h == 0) 
					printf(ANSI_COLOR_MAGENTA  "%s:" ANSI_COLOR_RESET , filename);
				if(ff)
					printMatchedf(*tmp);
				else
					printMatched(*tmp, strlen(word));
				mNUM--;
			}
		}
		tmp = tmp->next;
	}
}
void printMatched(node n, int len) {
	char *text = n.str, *ptr = text, w[1024] = "";
	int i, count = 0;
	for(i = 0; i < strlen(text); i++) {
		if(i == n.occ[count]) {
			ptr = text + i;
			strncat(w, ptr, len);
			printf(BOLDRED  "%s"  ANSI_COLOR_RESET, w);
			strcpy(w, "");
			count++;
			i += len - 1;
		}
		else
			putchar(text[i]);
	}
}
void printMatchedf(node n) {
	char *text = n.str, *ptr = text, w[1024] = "";
	int i, j, count = 0;
	for(i = 0; i < strlen(text); i++) {
		for(j = 0; j < n.occount; j++) {
			if(i == n.occ[j])
				break;
		}
		if(j != n.occount) {
			ptr = text + i;
			strncat(w, ptr, n.occ[j + 1] - n.occ[j] + 1);
			printf(BOLDRED  "%s"  ANSI_COLOR_RESET, w);
			strcpy(w, "");
			i += n.occ[j + 1] - n.occ[j];
			count += 2;
		}
		else
			putchar(text[i]);
	}
}
