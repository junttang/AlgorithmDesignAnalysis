#include "stackSLL.h"

void SLLstack::push(SLL *st) {
	st->p = s;
	s = st;
	stack_size++;
}

SLL* SLLstack::pop(void) {
	SLL *p;

	if (empty())
		errorExit("Stack is empty(Spop)");

	p = s;
	s = s->p;
	stack_size--;
	
	return p;
}

SLL* SLLstack::top(void) {
	if (empty())
		errorExit("Stack is empty(Stop)");

	return s;
}

bool SLLstack::empty(void) {
	if (stack_size < 0)
		errorExit("Stack is corrupted(Sempty)");
	
	if (stack_size == 0)
		return true;
	return false;
}