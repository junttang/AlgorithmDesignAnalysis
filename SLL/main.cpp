#include "queueSLL.h"
#include "stackSLL2.h"
// SLL practice

/*
SSList�� ���� pool�� ����ϱ� ���� �ٽ� ���׵�

- SSList�� private ������(����� cnt, memorycnt ���)�� ���
static���� �����Ѵ�.(���� ����� ���ؼ� �� �۾��� �ʼ�)
- �׸��� �� static �����鿡 ���ؼ��� SSList ������ �ʱ�ȭ���� �ʴ´�.
- �̵��� SSList Ŭ������ �Լ��� ���� SLList.cpp���� �� 1���ʸ� ����
���� �ʱ�ȭ�ϵ��� �ʱ�ȭ���ش�.
- �׸��� ���ĺ��ʹ�, ���� ���� �� SLList�� ���� include�� �̷������
�ȵȴ�. �ֳ�? �� �������� �� �� �̻� �ʱ�ȭ�Ǵ� ���� �߻��ϹǷ�.
��Ȯ�ϰ� ���ϸ�, SLList�� ����ϴ� SSLstack ����� SSLqueue����� ����
���� ������ ������� SSList�� include�ϸ�, �̵��� ���� ��Ÿ�����δ� main
���� ����ص� ������, ���ÿ� SSLstack�� SSLqueue ����� include�ϸ�
�����ǰ� �̷���� ������ �߻��Ѵ�.

����, ����, queue�� stack�� ��� ����ϰ� ������, SLLqueue�� SSList��
�ƴ϶� SSLstack�� include�ϴ� ������ �����ؾ��� ���̴�.

�ٽ���, �����Ǹ� ���ϴ� ���̴�.
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
		p = q->allocSLL(); // ���� storage pool�̴�.
		p->key = i;
		q->enque(p);
	}
	while (!s.empty()) {
		p = s.pop();
		cout << p->key << ' ';
		cout << '\n';
		q->SLL_pool_status();	// �������� ���� �ִ��� üũ
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
		q->SLL_pool_status(); // s�� ��ü ����.
		q->freeSLL(p);
	}
	q->freeSLL_pool();	// ���� �Ҵ� �޸� free
	
	cout << '\n';
	s.SLL_pool_status();	// �� ��������� üũ, q�� �ص� �ȴ�.
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