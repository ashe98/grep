typedef struct node {
	char *str;
	struct node *next, *prev;
	int occ[128];
	int occount;
}node;

typedef struct Lines {
	node *head, *tail;
}Lines;

void initLines(Lines *l);
void addLine(Lines *l, char *s);
void destroyLines(Lines *l);
int length(Lines l);
void grep(Lines *l, char *word, int fi);
void grepw(Lines *l, char *word, int fi);
void grepf(Lines *l, int fw, int fi, char *file2);
int search(Lines *l);
void printLines(Lines l, optqueue oq, char *word, char *filename, int ff);
void printMatched(node n, int len);
void printMatchedf(node n);
