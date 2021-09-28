#ifndef __SLL_STACK__
#define __SLL_STACK__
#endif

#include "classSLL.h"

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
		while (!empty()) {
			SLL *p = pop();
			freeSLL(p);
		}
	}

	void push(SLL *st);
	SLL* pop(void);
	SLL* top(void);
	bool empty(void);
};