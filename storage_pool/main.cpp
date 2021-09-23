#include "SLLqueue.h"
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

int main(void) {
	SLLstack s; 
	SLLqueue *q = new SLLqueue();
	SLL *p;

	for (int i = 9; i > 0; i--) {
		p = s.allocSLL(); 
		p->key = i;
		s.Spush(p);
		p = q->allocSLL(); // ���� storage pool�̴�.
		p->key = i;
		q->Enqueue(p);
	}
	while (!s.Sempty()) {
		p = s.Spop();
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
	while (!q->Qempty()) {
		p = q->Dequeue();
		cout << p->key << ' ';
		cout << '\n';
		q->SLL_pool_status();
		q->freeSLL(p);
	}
	q->freeSLL_pool();	// ���� �Ҵ� �޸� free
	
	cout << '\n';
	q->SLL_pool_status();	// �� ��������� üũ
	delete q;

	return 0;
}