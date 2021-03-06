#ifndef __SLL_STACK2__
#define __SLL_STACK2__
#endif

#include "classSLL2.h"

class sllStack2 {
private:
	SLL2 *s;
	unsigned long stack_size;

public:
	sllStack2() {
		s = NULL;
		stack_size = 0;
	}
	~sllStack2() {
	}

	void push(SLL2 *st);
	SLL2* pop(void);
	SLL2* top(void);
	bool empty(void);
};