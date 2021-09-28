#ifndef __SLL_QUEUE__
#define __SLL_QUEUE__
#endif

#include "stackSLL.h"

class sllQueue:public SLList {
private:
	SLL *Q, *Q_end;
	unsigned long queue_size;

public:
	sllQueue() {
		Q = Q_end = NULL;
		queue_size = 0;
	}
	~sllQueue() {
		while (!empty()) {
			SLL *p = deque();
			freeSLL(p);
		}
	}

	void enque(SLL *st);
	SLL* deque(void);
	SLL* front(void);
	bool empty(void);
};