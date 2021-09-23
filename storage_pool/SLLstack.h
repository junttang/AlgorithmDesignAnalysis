#ifndef __SLL_STACK__
#define __SLL_STACK__
#endif

#include "SLList.h"

class SLLstack:public SLList {
private:
	SLL *s;
	unsigned long stack_size;

public:
	SLLstack() {
		s = NULL;
		stack_size = 0;
	}
	~SLLstack() {
		while (!Sempty()) {
			SLL *p = Spop();
			freeSLL(p);
		}
	}

	void Spush(SLL *st);
	SLL* Spop(void);
	SLL* Stop(void);
	bool Sempty(void);
};