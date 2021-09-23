#ifndef __SLL_QUEUE__
#define __SLL_QUEUE__
#endif

#include "SLLstack.h"

class SLLqueue:public SLList {
private:
	SLL *qfront, *qrear;
	unsigned long queue_size;

public:
	SLLqueue() {
		qfront = qrear = NULL;
		queue_size = 0;
	}
	~SLLqueue() {
		while (!Qempty()) {
			SLL *p = Dequeue();
			freeSLL(p);
		}
	}

	void Enqueue(SLL *st);
	SLL* Dequeue(void);
	SLL* Qfront(void);
	bool Qempty(void);
};