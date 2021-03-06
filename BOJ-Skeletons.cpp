#include <iostream>
#include <vector>
// BOJ - 14193 Skeletons
// std id: 20171643		name: Park Junhyeok		풀이방법 제출
using namespace std;

typedef struct {
	bool in_edge = false;		// if vertex has incoming edge
	bool out_edge = false;		// if vertex has outgoing edge
	vector<int> E;				// adjacent list structure
}Vertex;

Vertex *V;
bool *visited;
vector<int> ST;					// stack used in DFS
int numofvisited;				// number of visited vertices
int *level;						// for topological direction checking
int check;						// for topological direction checking

int GCD(int a, int b) {			// euclidean algorithm for topological checking
	if (b == 0) return a;
	return GCD(b, a % b);
}

int DFS(int src, int n) {		// revised DFS
	int v, next;
	int cycle = 0;				// count of cycles

	ST.push_back(src);
	visited[src] = true;		// initial conditions
	numofvisited++;
	level[src] = 0;

	while (!ST.empty()) {
		v = ST.back();

		if (V[v].E.empty()) {		// if vertex has no remaining edges
			ST.pop_back();
			continue;
		}

		next = V[v].E.back(); V[v].E.pop_back();
		if (visited[next] == true) {		// if DFS encounters the visited node
			int flag = 0, j = 0, k = 0;

			for (int i = ST.size() - 1; i >= 0; i--) {	// check if its in the path
				if (next == ST[i]) {
					j = i; break;
				}
			}
			for (int i = ST.size() - 1; i >= j; i--) {	// check if there's branching
				if (!V[ST[i]].E.empty()) {				// possible node in the path
					k = i; flag = 1; break;
				}
			}
			if (next == ST.front() || (V[next].E.empty() && numofvisited == n))
				flag = 1;							// exception situation processing

			if (flag)			// if flag is set, increment the cycle count
				cycle++;		// if not, just pass since it will be determined later

			for (int i = ST.size() - 1; i > k; i--)
				ST.pop_back();			// backtracing until reaching branching node
										// below instruction counts topological roads
			check = GCD(check, abs(level[v] + 1 - level[next]));
		}
		else {								// if encountered node is un-visited,
			if (!V[next].E.empty()) {		// just go to that vertex
				visited[next] = true; 
				numofvisited++;
				ST.push_back(next);
				level[next] = level[v] + 1;
			}
		}
	}

	return cycle;		// total nums of cycles if the graph satisfies conditions
}

int main(void) {
	int n, m, f, r, res, no_flag;

	while (1) {
		no_flag = 0; check = 0; numofvisited = 0;

		cin >> n >> m;
		if (n == 0 && m == 0) break;

		V = new Vertex[n + 1];
		visited = new bool[n + 1];		// initialization
		for (int i = 1; i <= n; i++) visited[i] = false;
		level = new int[n + 1];

		for (int i = 0; i < m; i++) {
			int already = 0;
			cin >> f >> r;

			for (int j = 0; j < V[f].E.size(); j++)		// ignore already inserted edges
				if (V[f].E[j] == r) { already = 1; break; }		// for efficiency

			if (already == 0) {
				V[f].E.push_back(r);			// graph construction
				if (f != r) {
					V[f].out_edge = true;		// for making such conditions
					V[r].in_edge = true;
				}
			}
		}
			
		for (int i = 1; i <= n; i++) {		// check if there's isolated single vertex
			if (n == 1) break;				// only except for when n is 1
			if (V[i].in_edge == false || V[i].out_edge == false) {
				no_flag = 1;
				break;
			}
		}
		if (n >= m && n != 1 && n != 2) no_flag = 1;	// censor some 'do not need to check'

		if (no_flag) cout << "N\n";
		else {
			if (n == 1 && V[1].E.back() == 1) cout << "Y\n";	// process for some execeptional
			else if (n == 2 && V[1].in_edge && V[2].in_edge			// input situations that this
				&& V[1].out_edge && V[2].out_edge) cout << "Y\n";	// algorithm cannot handle
			else {
				res = DFS(1, n);
				
				if (res >= 2 && check == 1)		// if graph has more than two cycles, and only one
					cout << "Y\n";				// directional topological route, then it's okay!!
				else cout << "N\n";				// if not, it's NO!!
			}
		}

		delete[] V; delete[] visited; delete[] level;
	}

	return 0;
}