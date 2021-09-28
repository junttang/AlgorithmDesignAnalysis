#include "queueSLL.h"

void sllQueue::enque(SLL *st) {
	if (Q == NULL && Q_end == NULL)
		Q = st;
	else
		Q_end->p = st;
	Q_end = st;
	queue_size++;
}

SLL* sllQueue::deque(void) {
	SLL *p = Q;
	if (empty())
		errorExit("Queue is empty(Dequeue)");

	if (queue_size == 1) 
		Q = Q_end = NULL;
	else
		Q = Q->p;
	queue_size--;

	return p;
}

SLL* sllQueue::front(void) {
	if (empty())
		errorExit("Queue is empty(Qfront)");

	return Q;
}

bool sllQueue::empty(void) {
	if (queue_size < 0)
		errorExit("Queue is corrupted(Qempty)");

	if (queue_size == 0)
		return true;
	return false;
}