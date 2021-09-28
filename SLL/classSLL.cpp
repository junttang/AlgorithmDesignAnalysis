#include "classSLL.h"
#define NONE	-1

SLL* SLList::SLL_pool;
unsigned long SLList::SLL_cnt; 
unsigned long SLList::UsedMemoryForSLLs;

SLL* SLList::allocSLL(void) {
	SLL *p;
	
	if (SLL_pool == NULL) {
		p = (SLL*)malloc(sizeof(SLL));
		if (p == NULL)
			errorExit("Error in AllocSLL");

		p->key = NONE;
		UsedMemoryForSLLs += sizeof(SLL);	// 할당메모리양
	}
	else {
		p = SLL_pool;
		SLL_pool = SLL_pool->p;
	}
	p->p = NULL;
	SLL_cnt++;

	return p;
}
void SLList::freeSLL(SLL* st) {
	if (st->key == NONE)
		errorExit("This is already freed(freeSLL)");

	st->key = NONE;
	st->p = SLL_pool;
	SLL_pool = st;

	SLL_cnt--;
}

void SLList::freeSLL_pool(void) {
	SLL *p = SLL_pool;

	while (p) {
		SLL_pool = SLL_pool->p;
		free(p);
		p = SLL_pool;
		UsedMemoryForSLLs -= sizeof(SLL);
	}

	if (SLL_cnt != 0)
		errorExit("Non-zero SLL_cnt after cleanup");
}

void SLList::SLL_pool_status(void) {
	std::cout << "SLL_cnt : " << SLL_cnt << '\n';
	std::cout << "Memory : " << UsedMemoryForSLLs << '\n';
}

void errorExit(const char *s) {
	std::cout << s;
	exit(-1);
}