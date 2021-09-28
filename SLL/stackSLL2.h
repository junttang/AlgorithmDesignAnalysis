#ifndef __SLL_STACK2__
#define __SLL_STACK2__
#endif

#include "classSLL2.h"

class SLLstack2 {
private:
	SLL2 *s;
	unsigned long stack_size;

public:
	SLLstack2() {
		s = NULL;
		stack_size = 0;
	}
	~SLLstack2() {
	}

	void push(SLL2 *st);
	SLL2* pop(void);
	SLL2* top(void);
	bool empty(void);
};