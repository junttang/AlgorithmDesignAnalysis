#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <time.h>
#include <stack>
#include "DBL.h"

//#define NO_PATH_OUT   // comment out this line for path output
void Error_Exit(const char *s);

typedef struct _Vertex {
	dblStack S;		// adj list contains edge index
	int degree;
} Vertex;

typedef struct _Edge {
	int v1, v2;
} Edge;

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN);
void adjListGenerate(Vertex *V, Edge *E, int VN, int EN);
void deallocGraph(Vertex *V, Edge *E, int VN);
int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN);

DBList pool;	// DBL storage pool

int main() {
	int T, VN, EN;
	Vertex *V;
	Edge   *E;
	int *path;	// Euler cycle or path
	int pathN;  // path length
	int  flag;	// 0: cycle, 1: path, 2: none
	clock_t start_time, finish_time;

	scanf("%d", &T);	// read # of tests
	for (int t = 1; t <= T; t++) {	// for each test
		graphGeneration(&V, &E, &VN, &EN);
		
		start_time = clock(); // set the start time

		path = Find_Euler(V, E, VN, EN, &flag, &pathN); // find an Euler path or cycle

		finish_time = clock(); // set finish time
		
		double cmpt = (((double)(finish_time - start_time)) / CLK_TCK)*1000; // compute the time passed
		printf("Test= %d flag= %d VnumInCycle/Path)= %d ", t, flag, pathN);

		if (flag == 0)
			printf("Euler_cycle(exec_time= %.2f msec)\n",cmpt);
		else if (flag == 1)
			printf("Euler_path(exec_time= %.2f msec)\n", cmpt);
		else
			printf("not_Eulerian(exec_time= %.2f msec)\n", cmpt);
		
#ifndef NO_PATH_OUT
		if (flag != 2) {
			for (int i = 0; i < EN + 1; i++) {
				printf("%d\n", path[i]);
			}
		}
#endif
		if (flag != 2)
			delete[] path;
		deallocGraph(V, E, VN);
	}
	pool.freeDBL_pool();	// clear all the DBL elements

	return 0;
}

int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN) {
	// input V, VN, E, EN
	// output through paramters
	//   *flag = 0 (Euler cycle), 1 (Euler path), 2 (not Eulerian)
	//   *pathN = size of path[] array
	// output by return
	//   *path = list of vertex ids found(Euler cycle or path)

	stack<int> ST;		// use STL stack as explained in the class
	int *path = NULL;
	int even_cnt = 0, odd_cnt = 0;
	int src_vertex = 0, cur_vertex, next_vertex;  // source default is zero
	DBL *cur_edge, *twin_edge;
	int path_index = 0;

	for (int v = 0; v < VN; v++) { 	 // checking all the degrees of all vertices
		if (V[v].degree % 2 == 0)
			even_cnt++;
		else {
			if (odd_cnt == 0) 
				src_vertex = v;   // could be src vertex if the graph has EulerPath
			odd_cnt++;
		}
	}

	if (even_cnt == VN) *flag = 0;				// Euler cycle
	else if (odd_cnt == 2) *flag = 1;			// Euler path
	else { *flag = 2; *pathN = 0; return path;}	// Not Eulerian

	*pathN = EN + 1;
	path = new int[*pathN];			// path array allocation with the size |E|+1

	ST.push(src_vertex);						// path finding process
	while (!ST.empty()) {
		cur_vertex = ST.top();

		if (V[cur_vertex].degree == 0) {		// if vertex has no remaining edges
			path[path_index++] = cur_vertex;	// push to the result 'path' array
			ST.pop();
		}
		else {
			cur_edge = V[cur_vertex].S.pop();		// if vertex has remaining edges
			next_vertex = cur_edge->d;				// determine the next vertex and
			twin_edge = cur_edge->twin;				// find the twin of current edge

			V[next_vertex].S.remove(twin_edge);		// unlink the twin node
			pool.freeDBL(twin_edge);				// return to the storage pool
			pool.freeDBL(cur_edge);

			V[cur_vertex].degree--;					// update the degree info
			V[next_vertex].degree--;

			ST.push(next_vertex);
		}
	}

	return path;
}

void deallocGraph(Vertex *V, Edge *E, int VN) {
	DBL *p;

	for (int i = 0; i < VN; i++) {
		while (!V[i].S.empty()) {		// clear all the remaining nodes in each
			p = V[i].S.pop();			// stack, especially when 'Not Eulerian'
			pool.freeDBL(p);
		}
	}

	delete[] V;
	delete[] E;
}

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN) {
	scanf("%d %d", VN, EN);	// read # of vertices and edges
	//allocVandEarray(V, E, *VN, *EN);	// vertex and edge array allocation

	*V = new Vertex[*VN];
	*E = new Edge[*EN];
	if (*V == NULL || *E == NULL) {
		Error_Exit("Memory Allocation Error!");
	}

	for (int v = 0; v < *VN; v++) {
		(*V)[v].degree = 0;
	}
	for (int e = 0; e < *EN; e++) {
		scanf("%d %d", &((*E)[e].v1), &((*E)[e].v2));	// read edge information
		++((*V)[(*E)[e].v1].degree);
		++((*V)[(*E)[e].v2].degree);
	}
	adjListGenerate(*V, *E, *VN, *EN);	// create adj lust in G=(V,E)
}

void adjListGenerate(Vertex *V, Edge *E, int VN, int EN) {
	// Construct adjacent list in V array
	int v1, v2;
	DBL *adjE1, *adjE2;

	for (int e = 0; e < EN; e++) {
		v1 = E[e].v1; v2 = E[e].v2;

		adjE1 = pool.allocDBL();
		adjE2 = pool.allocDBL();

		adjE1->d = v2; adjE2->d = v1;
		adjE1->twin = adjE2;			// connect the twin edge nodes
		adjE2->twin = adjE1;

		V[v1].S.push(adjE1);			// construct the adjacent list(stack)
		V[v2].S.push(adjE2);
	}
}

void Error_Exit(const char *s) {
	printf("%s", s);
	exit(-1);
}

DBL *DBList::allocDBL(void) {		// allocate a DBL element
	DBL *p;

	if (DBL_pool == NULL) {
		p = new DBL;
		if (p == NULL)
			Error_Exit("Memory allocation error(Alloc_DBL).");

		p->d = NONE;
		UsedMemoryForDBLs += sizeof(DBL);
	}
	else {
		p = DBL_pool;
		DBL_pool = DBL_pool->rp;
	}
	p->twin = p->lp = p->rp = NULL;		// must care 'left link' since it's DBL
	++DBL_cnt;

	return(p);
}

void DBList::freeDBL(DBL *p) {
	// move p to pool

	if (p->d == NONE) {
		Error_Exit("This element is already freed(Free_DBL).");
	}

	p->d = NONE;
	p->rp = DBL_pool;
	DBL_pool = p;

	--DBL_cnt;		// reduce # of active DBL elements by one 
}

void DBList::freeDBL_pool(void) {
	DBL *p = DBL_pool;

	while (p != NULL) {
		DBL_pool = p->rp;	// get next pointer
		delete p;
		p = DBL_pool;
		UsedMemoryForDBLs -= sizeof(DBL);
	}
	if (DBL_cnt != 0) {
		Error_Exit("Non-zero DBL_cnt after cleanup.");
	}
}

void dblStack::push(DBL *p) {
	if (ST != NULL)
		ST->lp = p;		// if stack is nonempty, we should care 'left link'
	
	p->rp = ST;
	p->lp = NULL;
	ST = p;
}

DBL *dblStack::pop(void) {	// remove and return p in front of Q
	DBL *p;

	if (empty())
		Error_Exit("The stack is already empty(pop).");		// error checking

	p = ST;
	if (ST->rp == NULL)
		ST = NULL;
	else {
		ST = ST->rp;
		ST->lp = NULL;
	}

	return p;
}

void dblStack::remove(DBL *p) {	// delete p from ST
	if (ST == p) {
		ST = p->rp;
		if (p->rp != NULL)
			(p->rp)->lp = NULL;
	}
	else {
		(p->lp)->rp = p->rp;
		if (p->rp != NULL)
			(p->rp)->lp = p->lp;
	}
}

DBL *dblStack::top(void) {
	return ST;
}

bool dblStack::empty(void) {
	if (ST == NULL)
		return true;
	else
		return false;
}
