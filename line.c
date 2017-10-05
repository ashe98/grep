#include "line.h"
#define _GNU_SOURCE 
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
void initLines(Lines *l) {
	l->head = NULL;
	l->tail = NULL;
}
void addLine(Lines *l, char *s) {
	node *x;
	int len;
	x = (node*)malloc(sizeof(node));
	x->str = (char*)malloc(strlen(s) + 1);
	strcpy(x->str, s);
	len = length(*l);
	if(len == 0) {
		l->head = l->tail = x;
		x->prev = x->next = NULL;
		return;
	}
	x->next = NULL;
	x->prev = l->tail;
	l->tail->next = x;
	l->tail = x;
}
void destroyLines(Lines *l) {
	node *tmp = l->head->next;
	while(1) {
		if(tmp == NULL) {
			free(l->tail);
			break;
		}
		else {
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
void grep
void printLines(Lines l, optqueue oq, char *filename) {
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
		if(H == 1 && h == 0)
			printf("%s:", filename);
		*tmp = l.head;
		while(tmp != NULL) {
			if(tmp->pos != -1)
				count++;
			tmp = tmp->next;
		}
		if(v) {
			count = length(l) - count;
			if(m)
				printf("%d\n", (mNUM < count) ? mNUM : count);
			else
				printf("%d\n", count);
			return;
		}
		else {
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
				if(H == 1 && h == 0)
					printf("%s:", filename);
				if(tmp->pos == -1) {
					printf("%d:", bytes);
					printf("%s\n", tmp->str);
				}
				bytes = bytes + strlen(tmp->str);
				tmp = tmp->next;
				mNUM--;
			}
		}
		else {
			while(tmp != NULL && mNUM != 0) {
				if(H == 1 && h == 0)
					printf("%s:", filename);
				if(tmp->pos != -1) {
					printf("%d:", bytes + tmp->pos);
					printf("%s\n", tmp->str);
				}
				bytes = bytes + strlen(tmp->str);
				tmp = tmp->next;
				mNUM--;
			}
		}
		return;
	}
	tmp = l.head;
	while(tmp != NULL && mNUM != 0) {
		if(H == 1 && h == 0)
			printf("%s:", filename);
		if(v) {
			if(tmp->pos == -1)
				printf("%s\n", tmp->str);
		}
		else {
			if(tmp->pos != -1)
				printf("%s\n", tmp->str);
		}
		tmp = tmp->next;
	}
}
