#include "optqueue.h"

void oqinit(optqueue *oq){
	oq->r = -1;
	oq->f = 0;
	oq->count = 0;
}
void oenq(optqueue *oq, char c){
	oq->r = (oq->r + 1) % N;
	oq->data[oq->r] = c;	
	(oq->count)++;
} 
char odeq(optqueue *oq){
	char t = oq->data[oq->f];
	oq->f = (oq->f + 1) % N;
	(oq->count)--;
	return t;	
}
int oqempty(optqueue *oq){
	return oq->count == 0;
}
int oqfull(optqueue *oq){
	return oq->count == 20;
}
