#include "optqueue.h"
typedef struct node {
	char *str;
	struct node *next, *prev;
	int pos;
}node;

typedef struct Lines {
	node *head, *tail;
}Lines;

void initLines(Lines *l);
void addLine(Lines *l, char *s);
void destroyLines(Lines *l);
int length(Lines l);
void printLines(Lines l, optqueue oq, char *filename);



