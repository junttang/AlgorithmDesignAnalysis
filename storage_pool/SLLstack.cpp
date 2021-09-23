#include "SLLstack.h"

void SLLstack::Spush(SLL *st) {
	st->p = s;
	s = st;
	stack_size++;
}

SLL* SLLstack::Spop(void) {
	SLL *p;

	if (Sempty())
		errorExit("Stack is empty(Spop)");

	p = s;
	s = s->p;
	stack_size--;
	
	return p;
}

SLL* SLLstack::Stop(void) {
	if (Sempty())
		errorExit("Stack is empty(Stop)");

	return s;
}

bool SLLstack::Sempty(void) {
	if (stack_size < 0)
		errorExit("Stack is corrupted(Sempty)");
	
	if (stack_size == 0)
		return true;
	return false;
}