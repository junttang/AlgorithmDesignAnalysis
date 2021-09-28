#include "stackSLL.h"

void sllStack::push(SLL *st) {
	st->p = s;
	s = st;
	stack_size++;
}

SLL* sllStack::pop(void) {
	SLL *p;

	if (empty())
		errorExit("Stack is empty(Spop)");

	p = s;
	s = s->p;
	stack_size--;
	
	return p;
}

SLL* sllStack::top(void) {
	if (empty())
		errorExit("Stack is empty(Stop)");

	return s;
}

bool sllStack::empty(void) {
	if (stack_size < 0)
		errorExit("Stack is corrupted(Sempty)");
	
	if (stack_size == 0)
		return true;
	return false;
}