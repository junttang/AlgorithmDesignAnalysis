#include "stackSLL2.h"

void SLLstack2::push(SLL2 *st) {
	st->p = s;
	s = st;
	stack_size++;
}

SLL2* SLLstack2::pop(void) {
	SLL2 *p;

	if (empty())
		errorExit2("Stack is empty(Spop)");

	p = s;
	s = s->p;
	stack_size--;

	return p;
}

SLL2* SLLstack2::top(void) {
	if (empty())
		errorExit2("Stack is empty(Stop)");

	return s;
}

bool SLLstack2::empty(void) {
	if (stack_size < 0)
		errorExit2("Stack is corrupted(Sempty)");

	if (stack_size == 0)
		return true;
	return false;
}