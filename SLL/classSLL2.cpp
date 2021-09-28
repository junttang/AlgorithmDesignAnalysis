#include "classSLL2.h"
#define NONE	-1

SLL2* SLList2::allocSLL(void) {
	SLL2 *p;

	if (SLL_pool == NULL) {
		p = (SLL2*)malloc(sizeof(SLL2));
		if (p == NULL)
			errorExit2("Error in AllocSLL");

		p->key = NONE;
		UsedMemoryForSLLs += sizeof(SLL2);	// 할당메모리양
	}
	else {
		p = SLL_pool;
		SLL_pool = SLL_pool->p;
	}
	p->p = NULL;
	SLL_cnt++;

	return p;
}
void SLList2::freeSLL(SLL2* st) {
	if (st->key == NONE)
		errorExit2("This is already freed(freeSLL)");

	st->key = NONE;
	st->p = SLL_pool;
	SLL_pool = st;

	SLL_cnt--;
}

void SLList2::freeSLL_pool(void) {
	SLL2 *p = SLL_pool;

	while (p) {
		SLL_pool = SLL_pool->p;
		free(p);
		p = SLL_pool;
		UsedMemoryForSLLs -= sizeof(SLL2);
	}

	if (SLL_cnt != 0)
		errorExit2("Non-zero SLL_cnt after cleanup");
}

void SLList2::SLL_pool_status(void) {
	std::cout << "SLL_cnt : " << SLL_cnt << '\n';
	std::cout << "Memory : " << UsedMemoryForSLLs << '\n';
}

void errorExit2(const char *s) {
	std::cout << s;
	exit(-1);
}