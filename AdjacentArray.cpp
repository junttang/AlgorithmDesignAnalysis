
#include "Graph_adj_array.h"

void Read_Graph_adj_array (int Vnum, vertex V[], int Enum, edge E[]) {
	// read graph information
	// V와 E의 name field는 각각 자신의 인덱스를 저장한다
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.
	int prev_index;

	for (int v = 0; v < Vnum; v++) {		// vertex array initialization
		V[v].name = v; V[v].flag = false;
		V[v].f_hd = V[v].r_hd = -1;
	}
	for (int e = 0; e < Enum; e++) {		// edge array initialization
		E[e].name = e; E[e].flag = false;
		E[e].fp = E[e].rp = -1;
	}

	for (int e = 0; e < Enum; e++) {
		scanf_s("%d %d %d", &E[e].vf, &E[e].vr, &E[e].cost);

		if (V[E[e].vf].f_hd == -1)			// first insertion (front adj list)
			V[E[e].vf].f_hd = e;
		else {
			prev_index = V[E[e].vf].f_hd;	// remember the existing edge_index
			V[E[e].vf].f_hd = e;			// replace with new_edge_index
			E[e].fp = prev_index;			// update the front adjacent list
		}

		if (V[E[e].vr].r_hd == -1)			// first insertion (rear adj list)
			V[E[e].vr].r_hd = e;
		else {
			prev_index = V[E[e].vr].r_hd;	// same as the procedure above
			V[E[e].vr].r_hd = e;			// except for the direction type of 
			E[e].rp = prev_index;			// the adjacent list (here's the rear)
		}
	}
}

int DFS_Tree_adj_array(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0) 
) {  
	// DFS를 사용하여 DFS tree에 속하는 에지의 flag을 true로 설정한다.
	// DFS 시작 vertex는 입력 파일에서 지정된다(src).
	// DFS tree에 속한 에지의 cost 함을 return 한다(모든 각 edge cost가 1이면 unique)
	// recursive 함수로 작성해도 무방하다.
	int w, sum_of_cost = 0;

	V[src].flag = true;		// set 'visited'

	for (int idx = V[src].f_hd; idx != -1; idx = E[idx].fp) {	// iterate the f_list
		if (E[idx].vf == src)
			w = E[idx].vr;					// determine the next vertex
		else w = E[idx].vf;

		if (V[w].flag == false) {			// check if 'visited'
			E[idx].flag = true;				// add to the DFS spanning tree
			sum_of_cost += E[idx].cost;									// cumulate
			sum_of_cost += DFS_Tree_adj_array(w, Vnum, V, Enum, E);		// recursion
		}	
	}
	for (int idx = V[src].r_hd; idx != -1; idx = E[idx].rp) {	// iterate the r_list
		if (E[idx].vr == src)
			w = E[idx].vf;			
		else w = E[idx].vr;			

		if (V[w].flag == false) {	// same as the procedure above only except for
			E[idx].flag = true;		// the direction type of the adj list
			sum_of_cost += E[idx].cost;
			sum_of_cost += DFS_Tree_adj_array(w, Vnum, V, Enum, E);
		}
	}

	return sum_of_cost;
}
