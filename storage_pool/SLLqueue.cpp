#include "SLLqueue.h"

void SLLqueue::Enqueue(SLL *st) {
	if (qfront == NULL && qrear == NULL)
		qfront = st;
	else
		qrear->p = st;
	qrear = st;
	queue_size++;
}

SLL* SLLqueue::Dequeue(void) {
	SLL *p = qfront;
	if (Qempty())
		errorExit("Queue is empty(Dequeue)");

	if (queue_size == 1) 
		qfront = qrear = NULL;
	else
		qfront = qfront->p;
	queue_size--;

	return p;
}

SLL* SLLqueue::Qfront(void) {
	if (Qempty())
		errorExit("Queue is empty(Qfront)");

	return qfront;
}

bool SLLqueue::Qempty(void) {
	if (queue_size < 0)
		errorExit("Queue is corrupted(Qempty)");

	if (queue_size == 0)
		return true;
	return false;
}