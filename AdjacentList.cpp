#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// 파일에서 그래프 정보를 읽어 V와 E 배열을 구성한다.
	// V[ ].name과 E[ ].name은 설정할 필요가 없으나 편의상 각 배열에서의
	//   index를 저장한다.
	// V[ ].S는 adjacent list header로 스택 헤더로도 사용된다.
	// 따라서 edge (i,j)를 읽으면 V[i].S에 edge index k를 push하고
	//  마찬가지로 V[j].S에 k를 push한다.
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.
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
	// V 배열의 adjacency list의 원소를 pop()을 사용하여
	// 모두  pool로 되돌려 준다
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
	// BFS를 사용하여 BFS tree에 속하는 에지의 flag을 true로 설정한다.
	// BFS 시작 vertex는 입력 파일에서 지정된다(src).
	// V[]의 adjacent list 탐색은 .top() 멤버 함수를 사용하여
	//  adj list 헤더를 얻고 이를 따라가며 인접 vertex를 탐색한다.
	// BFS tree에 속한 에지의 cost 함을 return 한다
	//   (not unique but 모든 각 edge cost가 1이면 unique)
	// -- 함수의 모든 parameter가 사용될 필요는 없다.
	// -- BFS를 위한 queue가 필요하면 STL의 queue를 사용해도 좋다
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
// 아래 함수들은 02_04_CSE3081_CPrg&DataStructure_추가자료.pdf를
//   참조하여 각 함수들을 채워 넣자.
// 스택만 필요하므로 큐의 멤버 함수는 아래 함수 리스트에 없다

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