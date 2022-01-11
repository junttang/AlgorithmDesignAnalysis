#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// ���Ͽ��� �׷��� ������ �о� V�� E �迭�� �����Ѵ�.
	// V[ ].name�� E[ ].name�� ������ �ʿ䰡 ������ ���ǻ� �� �迭������
	//   index�� �����Ѵ�.
	// V[ ].S�� adjacent list header�� ���� ����ε� ���ȴ�.
	// ���� edge (i,j)�� ������ V[i].S�� edge index k�� push�ϰ�
	//  ���������� V[j].S�� k�� push�Ѵ�.
	// V[].flag�� E[].flag�� �ݵ�� false�� �����ؾ� �Ѵ�.
	SLL *new_edge;

	for (int v = 0; v < Vnum; v++) {
		V[v].name = v; V[v].flag = false;		// vertex array initialization
	}

	for (int e = 0; e < Enum; e++) {
		scanf_s("%d %d %d", &E[e].vf, &E[e].vr, &E[e].cost);
		E[e].name = e; E[e].flag = false;		// edge array initialization
		
		new_edge = pool.allocSLL(); 
		new_edge->i = e;				// store the edge index k
		V[E[e].vf].S.push(new_edge);	// push edges that are incident to vertex

		new_edge = pool.allocSLL(); 
		new_edge->i = e;
		V[E[e].vr].S.push(new_edge);
	}
}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V �迭�� adjacency list�� ���Ҹ� pop()�� ����Ͽ�
	// ���  pool�� �ǵ��� �ش�
	for (int k = 0; k < Vnum; k++) {	// clear all the adjacent lists
		while (!V[k].S.empty())
			pool.freeSLL(V[k].S.pop());
	}
}

int BFS_Tree_adj_list(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
) {
	// BFS�� ����Ͽ� BFS tree�� ���ϴ� ������ flag�� true�� �����Ѵ�.
	// BFS ���� vertex�� �Է� ���Ͽ��� �����ȴ�(src).
	// V[]�� adjacent list Ž���� .top() ��� �Լ��� ����Ͽ�
	//  adj list ����� ��� �̸� ���󰡸� ���� vertex�� Ž���Ѵ�.
	// BFS tree�� ���� ������ cost ���� return �Ѵ�
	//   (not unique but ��� �� edge cost�� 1�̸� unique)
	// -- �Լ��� ��� parameter�� ���� �ʿ�� ����.
	// -- BFS�� ���� queue�� �ʿ��ϸ� STL�� queue�� ����ص� ����
	std::queue<int> Q;
	int cur_vertex = src, next_vertex;
	int sum_of_cost = 0, edge_index;

	V[cur_vertex].flag = true;		// set 'visited'
	Q.push(cur_vertex);

	while (!Q.empty()) {			// until the queue is empty
		cur_vertex = Q.front(); Q.pop();

		for (SLL *p = (V[cur_vertex].S).top(); p != NULL; p = p->p) {
			edge_index = p->i;				// traverse the adj list of the vertex

			if (E[edge_index].vf == cur_vertex)		// determine the next vertex
				next_vertex = E[edge_index].vr;
			else next_vertex = E[edge_index].vf;

			if (V[next_vertex].flag == false) {		// check if 'visited'
				V[next_vertex].flag = true;			// set 'visited' and enqueue
				Q.push(next_vertex);

				E[edge_index].flag = true;			// add to the BFS spanning tree
				sum_of_cost += E[edge_index].cost;	// cumulate the cost
			}
		}
	}

	return sum_of_cost;
}

// SLL management functions
// �Ʒ� �Լ����� 02_04_CSE3081_CPrg&DataStructure_�߰��ڷ�.pdf��
//   �����Ͽ� �� �Լ����� ä�� ����.
// ���ø� �ʿ��ϹǷ� ť�� ��� �Լ��� �Ʒ� �Լ� ����Ʈ�� ����

// stack member fuctions
void sllStack2::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack2::pop(void) {
	// remove and return p at the top of ST
	SLL *p;

	if (empty())
		errorExit("The stack is empty(pop).");

	p = ST;
	ST = ST->p;	

	return p;
}

SLL *sllStack2::top(void) {
	// return p at the top of ST
	return ST;
}

bool sllStack2::empty(void) {
	// return true if empty
	if (ST == NULL)
		return true;
	else
		return false;
}

// SLList2 member functions
SLL *SLList2::allocSLL(void) {		// allocate an SLL element
	SLL *p;

	if (SLL_pool == NULL) {		// if the storage pool is empty,
		p = (SLL*)malloc(sizeof(SLL));
		if (p == NULL)
			errorExit("The error occurs in allocSLL.");

		p->i = NONE;
		UsedMemoryForSLLs += sizeof(SLL);
	}
	else {
		p = SLL_pool;
		SLL_pool = SLL_pool->p;		// pop from the storage pool stack
	}
	p->p = NULL;
	SLL_cnt++;

	return(p);
}

void SLList2::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}

	p->i = NONE;
	p->p = SLL_pool;		// push to the storage pool stack
	SLL_pool = p;

	SLL_cnt--;
}

void SLList2::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	while (p) {
		SLL_pool = SLL_pool->p;
		free(p);
		p = SLL_pool;
		UsedMemoryForSLLs -= sizeof(SLL);
	}

	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}