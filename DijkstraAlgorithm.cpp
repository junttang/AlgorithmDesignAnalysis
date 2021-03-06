#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

#define NONE -1

typedef struct elm_edge {
	int  vf, vr;  // e = (vf, vr) where vf, vr are vertex array indices
	int  cost;    // edge cost
	bool flag;    // true if the edge is in the SP spanning tree, false otherwise 
	int  fp, rp;  // adj list ptr of forward and reverse (-1 if none)
} edge;

typedef struct elm_vertex {
	int  f_hd, r_hd;	// adj list header(-1 if none)
	int  distance;		// distance from src to this vertex
	int  heap_idx;		// minHeap index
	bool inS;			// true if SP length found(true when move from V-S to S) 
} vertex;

void HeapInsert(			// 최소 힙에 새로운 원소를 삽입하는 함수
	int Vnum, vertex *V, 
	int *heapSize, int *minHeap, int vertexIdx			// vertexIdx는 삽입될 정점의 인덱스
) {
	int i = *heapSize;

	if (i >= Vnum - 1) {
		fprintf(stderr, "The Heap is full(HeapInsert).\n");			// 비정상 입력 처리문
		exit(1);
	}
	i++; *heapSize = i;						// 기존 힙의 가장 마지막 인덱스 다음의 인덱스를 취한다 

	while ((i != 1) && (V[vertexIdx].distance < V[minHeap[i / 2]].distance)) {
		minHeap[i] = minHeap[i / 2];
		V[minHeap[i / 2]].heap_idx = i;					// 부모 노드와 비교를 하며 제 자리를
		i /= 2;											// 찾을때까지 힙을 거슬러 올라간다
	}

	minHeap[i] = vertexIdx;
	V[vertexIdx].heap_idx = i;							// 본 과제의 힙 관리에서 heap_idx 업데이트가
}														// 가장 중요한 부분 중 하나이다

void HeapUpdate(			// distance 업데이트 시 힙 자료구조도 업데이트하는 함수
	int Vnum, vertex *V,								// vertexIdx는 삽입될 정점의 인덱스
	int heapIdx, int *minHeap, int vertexIdx			// heapIdx는 해당 정점의 힙 내에서의 인덱스
) {
	int i = heapIdx;				// distance 업데이트 시 반드시 기존 distance보다 작을 수 밖에 없음
									// 따라서 Move-Up Operation을 진행(HeapInsert 함수와 유사)
	while ((i != 1) && (V[vertexIdx].distance < V[minHeap[i / 2]].distance)) {
		minHeap[i] = minHeap[i / 2];
		V[minHeap[i / 2]].heap_idx = i;					// 기존의 부모 노드에 있던 요소가 자식으로 내려옴
		i /= 2;
	}

	minHeap[i] = vertexIdx;
	V[vertexIdx].heap_idx = i;
}

int HeapDelete(				// 최소 힙에서 최상단 루트에 위치한 원소를 삭제하는 함수
	int Vnum, vertex *V,
	int *heapSize, int *minHeap
) {
	int parent = 1, child = 2;
	int delVelIdx, temp, size = *heapSize;

	if (size == 0) {
		fprintf(stderr, "The Heap is empty(HeapDelete).\n");		// 비정상 명령 처리문
		exit(1);
	}

	delVelIdx = minHeap[1];								// 힙의 최상단 루트에 위치한 원소를 선택
	V[delVelIdx].heap_idx = NONE;						// 해당 정점의 heap_idx를 NONE 처리
	temp = minHeap[size--]; *heapSize = size;			// 기존 힙의 최하단 마지막 인덱스의 원소를
														// 루트에서부터 내리는 연산을 진행
	while (child <= size) {								// 1 줄어든 사이즈만큼까지만 이동 가능
		if ((child < size) && (V[minHeap[child]].distance > V[minHeap[child + 1]].distance))
			child++;									// 두 자식 중 더 작은 자식과 비교를 진행
		if (V[temp].distance <= V[minHeap[child]].distance)			// 제자리를 찾을 때까지
			break;

		minHeap[parent] = minHeap[child];				// 실질적 swap이 아닌 '끌어내리기' 작업
		V[minHeap[child]].heap_idx = parent;

		parent = child;
		child *= 2;
	}
	
	minHeap[parent] = temp;
	V[temp].heap_idx = parent;

	return delVelIdx;				// 제거된 원소를 반환
}

int SPT_Backtracing(		 // Shortest Spanning Tree를 구하기 위한 Backtracing 수행 함수
	int src,
	int Vnum, vertex *V,
	int Enum, edge *E
) {
	int w;
	int treeCost = 0, minCost_1, minCost_2;		// treeCost는 트리의 누적 가중치
	int candidate_1, candidate_2, selected;
	bool found;
	
	for (int v = 0; v < Vnum; v++) {			// 소스 정점을 제외한 모든 정점 v에 대하여
		if (v == src) continue;
		found = false;
		candidate_1 = candidate_2 = NONE;
		minCost_1 = minCost_2 = INT_MAX;

		for (int idx = V[v].f_hd; idx != NONE; idx = E[idx].fp) {	// v의 front adj list 순회
			w = E[idx].vr;											// 인접한 정점 w에 대해

			if (V[w].distance + E[idx].cost == V[v].distance) {		// w의 최소거리와 (v, w)의
				if (E[idx].cost < minCost_1) {						// 가중치 합이 v의 최소거리
					minCost_1 = E[idx].cost;						// 와 같을 때, 간선 후보로!
					candidate_1 = idx;
				}

				found = true;										// 후보 에지가 있음을 명시
			}
		}
		
		for (int idx = V[v].r_hd; idx != NONE; idx = E[idx].rp) {	// rear adj list도 순회
			w = E[idx].vf;

			if (V[w].distance + E[idx].cost == V[v].distance) {
				if (E[idx].cost < minCost_2) {						// 위에서 설명한 조건과 일치
					minCost_2 = E[idx].cost;						// 하는 후보 에지가 복수일 경우
					candidate_2 = idx;								// 그들 중 최소의 에지를 선택
				}

				found = true;
			}
		}

		if (found) {
			if (candidate_1 != NONE && candidate_2 != NONE)		// 신장트리로 들어갈 후보 에지 중
				selected = (E[candidate_1].cost < E[candidate_2].cost) ?
					candidate_1 : candidate_2;		// 더 작은 Cost의 에지를 최종 선택 에지로 결정
			else if (candidate_1 != NONE)						// 후보 에지가 하나이면 그대로 감
				selected = candidate_1;
			else 
				selected = candidate_2;

			E[selected].flag = true;
			treeCost += E[selected].cost;			// 선택된 에지를 ST에 포함시키고 Cost 합 누적
		}
	}

	return treeCost;
}

int SPT_Dijkstra(
	int src,	// source vertex index
	// graph structure array
	// 1. the adjacency list structure is the same as PHW02
	// 2. additional fields are added for Dijkstra's algorithm(see .h file)
	int Vnum, vertex *V,	// Vertex array size and the array
	int Enum, edge *E,		// Edge array size and the array

	int *minHeap	// array for min heap (array size = Vnum+1)
		// heap index range is 1 ~ (Vnum - 1) note: src must not in the initial heap
		// just arry is passed. must construct min-heap in this function

	// OUTPUT
	// 1. V[].distance : shortest path length from src to this vertex
	// 2. shortest path spanning tree : set E[].flag = true if the edge is in the tree
	// 3. return the sum of edge costs in the shortest path spanning tree.
	//    should be small as possible (think a way to make it small)
) {
	int treeCost = 0;
	// *** 이 함수를 작성하자 ***
	// 반드시 min-heap을 사용하여 O((n+m)logn) 알고리즘을 구현해야 한다(아니면 trivial한 프로그램임)
	// heap 연산 등 필요한 함수는 자유롭게 작성하여 추가한다.
	// 그러나 global 변수, dynamic array 등은 추가로 사용하지 않는다(실제로 필요 없다)

	int u, dis, w;
	int heapSize = 0;

	V[src].inS = true;					// 소스 정점은 처음부터 영역 S에 포함
	V[src].distance = 0;

	for (int idx = V[src].f_hd; idx != NONE; idx = E[idx].fp) {		// 소스와 직접 연결된 정점은
		w = E[idx].vr;												// distance를 간선 Cost로 초기화
		V[w].distance = E[idx].cost;
	}
	for (int idx = V[src].r_hd; idx != NONE; idx = E[idx].rp) {
		w = E[idx].vf;
		V[w].distance = E[idx].cost;
	}

	for (int vertexIdx = 0; vertexIdx < Vnum; vertexIdx++) {		// 소스 정점을 제외하고 힙에 삽입
		if (vertexIdx != src)
			HeapInsert(Vnum, V, &heapSize, minHeap, vertexIdx);
	}

	for (int i = 0; i < Vnum - 1; i++) {							// |V|-1개의 정점에 대하여
		u = HeapDelete(Vnum, V, &heapSize, minHeap);				// 최소 힙에서 최소 distance 정점
		V[u].inS = true;											// u를 택한 후, 영역 S에 포함시킴

		for (int idx = V[u].f_hd; idx != NONE; idx = E[idx].fp) {	// 정점 u의 인접 정점들 중
			w = E[idx].vr;							

			if (V[w].inS == false) {								// 영역 V-S에 포함되어 있는 정점에
				dis = V[u].distance + E[idx].cost;					// 대하여, Distance Update가 필요
																	// 한지 확인해 업데이트
				if (dis < V[w].distance) {
					V[w].distance = dis;
					HeapUpdate(Vnum, V, V[w].heap_idx, minHeap, w);	// 힙에도 Distance 업데이트를 반영
				}
			}
		}
		for (int idx = V[u].r_hd; idx != NONE; idx = E[idx].rp) {	// rear list에 대해서도 동일하게
			w = E[idx].vf;
			
			if (V[w].inS == false) {
				dis = V[u].distance + E[idx].cost;

				if (dis < V[w].distance) {
					V[w].distance = dis;
					HeapUpdate(Vnum, V, V[w].heap_idx, minHeap, w);
				}
			}
		}
	}

	treeCost = SPT_Backtracing(src, Vnum, V, Enum, E);		// 구축된 Distance 값을 기준으로 Backtracing

	return treeCost;
}

void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) {
	// Graph 자료구조를 만드는 함수
	// *** 이 함수를 추가하자 ***
	// PHW02의 Read_Graph_adj_array()를 이 과제의 자료구조에 맞춰 살짝 수정하여 사용한다
	// 즉, Read_Graph_adj_array()에서 불필요한 내용을 제거한 후 사용.

	int prev_index;

	for (int v = 0; v < Vnum; v++) {						// vertex array initialization
		V[v].distance = INT_MAX; V[v].inS = false;			// INT_MAX는 간선이 없음을 의미
		V[v].f_hd = V[v].r_hd = NONE;
	}
	for (int e = 0; e < Enum; e++) {						// edge array initialization
		E[e].flag = false;
		E[e].fp = E[e].rp = NONE;
	}

	for (int e = 0; e < Enum; e++) {
		scanf_s("%d %d %d", &E[e].vf, &E[e].vr, &E[e].cost);

		if (V[E[e].vf].f_hd == NONE)						// first insertion (front adj list)
			V[E[e].vf].f_hd = e;
		else {
			prev_index = V[E[e].vf].f_hd;					// remember the existing edge_index
			V[E[e].vf].f_hd = e;							// replace with new_edge_index
			E[e].fp = prev_index;							// update the front adjacent list
		}

		if (V[E[e].vr].r_hd == NONE)						// first insertion (rear adj list)
			V[E[e].vr].r_hd = e;
		else {
			prev_index = V[E[e].vr].r_hd;					// same as the procedure above
			V[E[e].vr].r_hd = e;							// except for the direction type of 
			E[e].rp = prev_index;							// the adjacent list (here's the rear)
		}
	}
}

// the following functions are for testing if the submitted program is correct.
int  Tree_Check(int Vnum, vertex *V, int Enum, edge *E, int *visited);
bool SPT_test(int src, int Vnum, vertex *V, int Enum, edge *E, int *minHeap);
void Error_Exit(const char *s);

int main(void) {
	int		src;
	vertex *V;		int Vnum;
	edge   *E;		int Enum;
	int    *minHeap;
	int    Tree_cost;
	int    Tnum; 		// # of test cases
	clock_t start, finish;
	double cmpt;

	scanf_s("%d", &Tnum);		// read # of tests

	for (int t = 0; t < Tnum; t++) {
		scanf_s("%d %d %d", &Vnum, &Enum, &src);
		V = new vertex[Vnum];
		E = new edge[Enum];
		minHeap = new int[Vnum + 1];	// heap array allocation
		if (V == NULL || E == NULL || minHeap == NULL) {
			Error_Exit("Memory Allocation Error");
		}
		Read_Graph(Vnum, V, Enum, E);

		/**/start = clock();	// start timer

		Tree_cost = SPT_Dijkstra(src, Vnum, V, Enum, E, minHeap);	// code by students

		/**/finish = clock();	// stop timer
		cmpt = ((double)(finish - start)) / (double)CLK_TCK;

		// 아래 Tree_Check와 SPT_test 함수는 첨부한 SPT_test.obj 파일에 있다.
		// 이 테스트에서 오류로 인하여 프로그램이 정지하면 뭔가 잘못된 것이다(제출해도 0점)
		if (Tree_Check(Vnum, V, Enum, E, minHeap) == 0) {
			Error_Exit("   ERROR The result is not a spanning tree");
		}
		if (SPT_test(src, Vnum, V, Enum, E, minHeap) == false) {
			Error_Exit("** Something wrong in applying Dijkstra's");
		}
		if (t != 0)
			printf("\n");

		printf("**T%d (Dijkstra) (V = %d, E = %d, time = %.3f sec) Tree Cost = %d\n",
			t + 1, Vnum, Enum, cmpt, Tree_cost);

		delete[] minHeap; delete[] V; delete[] E;
	}
	return 0;
}

void Error_Exit(const char *s) {
	printf("%s\n", s);
	exit(-1);
}
