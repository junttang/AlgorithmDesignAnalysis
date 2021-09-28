#ifndef __SLL_STACK__
#define __SLL_STACK__
#endif

#include "classSLL.h"

class sllStack:public SLList {
private:
	SLL *s;
	unsigned long stack_size;

public:
	sllStack() {
		s = NULL;
		stack_size = 0;
	}
	~sllStack() {
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