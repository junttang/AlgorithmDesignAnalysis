#ifndef __Single_Linked_List2__
#define __Single_Linked_List2__
#endif

#include <iostream>
#include <stdlib.h>

extern void errorExit2(const char *s);

typedef struct _SLL2 {
	int key;
	struct _SLL2 *p;
}SLL2;

class SLList2 {
private:
	SLL2 *SLL_pool;

public:
	unsigned long SLL_cnt;
	unsigned long UsedMemoryForSLLs;
	SLL2* allocSLL(void);
	void freeSLL(SLL2* st);
	void freeSLL_pool(void);
	void SLL_pool_status(void);
};