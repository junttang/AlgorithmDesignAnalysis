#include "queueSLL.h"
#include "stackSLL2.h"
// SLL practice

/*
SSList를 공동 pool로 사용하기 위한 핵심 사항들

- SSList의 private 변수들(헤더와 cnt, memorycnt 등등)은 모두
static으로 선언한다.(공동 사용을 위해선 이 작업이 필수)
- 그리고 이 static 변수들에 대해서는 SSList 내에서 초기화하지 않는다.
- 이들은 SSList 클래스의 함수를 적는 SLList.cpp에서 단 1차례만 전역
변수 초기화하듯이 초기화해준다.
- 그리고 이후부터는, 결코 절대 이 SLList의 복수 include가 이루어지면
안된다. 왜냐? 이 변수들이 두 번 이상 초기화되는 일이 발생하므로.
정확하게 말하면, SLList를 상속하는 SSLstack 헤더와 SSLqueue헤더를 따로
따로 각각의 헤더에서 SSList를 include하면, 이들을 서로 배타적으로는 main
에서 사용해도 되지만, 동시에 SSLstack과 SSLqueue 헤더를 include하면
재정의가 이루어져 오류가 발생한다.

따라서, 만일, queue와 stack을 모두 사용하고 싶으면, SLLqueue가 SSList가
아니라 SSLstack을 include하는 식으로 구성해야할 것이다.

핵심은, 재정의를 피하는 것이다.
*/

using namespace std;

SLList2 pool;

int main(void) {
	sllStack s; 
	sllStack2 s2;
	sllQueue *q = new sllQueue();
	SLL *p; SLL2 *pp;

	for (int i = 9; i > 0; i--) {
		p = s.allocSLL(); 
		p->key = i;
		s.push(p);
		p = q->allocSLL(); // 같은 storage pool이다.
		p->key = i;
		q->enque(p);
	}
	while (!s.empty()) {
		p = s.pop();
		cout << p->key << ' ';
		cout << '\n';
		q->SLL_pool_status();	// 공동으로 쓰고 있는지 체크
		s.freeSLL(p);
	}
	cout << '\n';
	/*for (int i = 9; i > 0; i--) {
		p = q->allocSLL();
		p->key = i;
		q->Enqueue(p);
	}*/
	while (!q->empty()) {
		p = q->deque();
		cout << p->key << ' ';
		cout << '\n';
		q->SLL_pool_status(); // s로 대체 가능.
		q->freeSLL(p);
	}
	q->freeSLL_pool();	// 실제 할당 메모리 free
	
	cout << '\n';
	s.SLL_pool_status();	// 다 사라졌음을 체크, q로 해도 된다.
	delete q;

	cout << '\n'; cout << '\n';

	for (int i = 1; i < 10; i++) {
		pp = pool.allocSLL();
		pp->key = i;
		s2.push(pp);
	}
	for (int i = 1; i < 10; i++) {
		pp = s2.pop();
		cout << pp->key << ' ';
		pool.freeSLL(pp);
	}
	while (!s2.empty())
		pool.freeSLL(s2.pop());

	return 0;
}