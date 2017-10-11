#define N 20
typedef struct optqueue {
	char data[N];
	int f, r, count;
}optqueue;

void oqinit(optqueue *oq);
void oenq(optqueue *oq, char c); 
char odeq(optqueue *oq);
int oqempty(optqueue *oq);
int oqfull(optqueue *oq);
