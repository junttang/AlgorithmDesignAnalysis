#ifndef __Single_Linked_List__
#define __Single_Linked_List__
#endif
#include <iostream>
#include <stdlib.h>

extern void errorExit(const char *s);

typedef struct _SLL {
	int key;
	struct _SLL *p;
}SLL;

class SLList {
private:
	static SLL *SLL_pool;
	static unsigned long SLL_cnt;
	static unsigned long UsedMemoryForSLLs;

public:
	SLL* allocSLL(void);
	void freeSLL(SLL* st);
	void freeSLL_pool(void);
	void SLL_pool_status(void);
};