#include <stdlib.h>
#include <string.h>

int Minimum(int ins, int del, int sub) {				// 최소값 찾아 반환하는 함수
	int min = ins;

	if (del < min) min = del;
	if (sub < min) min = sub;

	return min;
}

void Calculation(			  // Table을 Bottom-Up Fashion으로 구축하는 DP 수행 함수 
	char *SS, char *TS,	  // 이 함수가 종료되면, Table[m][n]은 Edit Distance를 의미함
	int m, int n, int **Table,
	int ins_cost, int del_cost, int sub_cost
) { 
	int ins, del, sub;					// 각각의 연산에서의 누적 Cost 값을 담을 변수

	for (int i = 0; i <= m; i++)				   // 첫 행과 첫 열을 초기화하는 부분
		Table[i][0] = i * del_cost;					// 이때, 연산의 Cost를 신경써야함
	for (int j = 0; j <= n; j++)
		Table[0][j] = j * ins_cost;

	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			ins = Table[i][j - 1] + ins_cost;
			del = Table[i - 1][j] + del_cost;

			if (SS[i - 1] == TS[j - 1])				   // 만약 마지막 문자들이 같다면
				sub = Table[i - 1][j - 1];					// No Operation으로 간주
			else sub = Table[i - 1][j - 1] + sub_cost;		   // 그렇지 않다면, 교체
			
			Table[i][j] = Minimum(ins, del, sub);				   // 최소값을 취한다
		}
	}		
}

int Minimum_Opt(				// 최소값에서의 '사용된 연산'이 무엇인지 반환하는 함수
	int ins, int del, int sub, int ins_cost, int del_cost) {
	int min = ins, option = 1;			// Default Setting은 Insertion 연산으로 설정

	if (del == min) {							   // PDF의 조건을 만족하기 위한 부분
		if (ins_cost <= del_cost) option = 1;
		else option = 2;
	}
	if (del < min) { 
		min = del; 
		option = 2; 
	}
	if (sub < min) { 
		min = sub; 
		option = 3; 
	}

	return option;	   // 1은 Insertion, 2는 Deletion, 3은 Substitution 또는 No Opt
}

void Backtracing(		  // Top-Down Solution Searching을 위한 Operation Sequence의
	char *SS, char *TS,				// 길이를 계산해서 반환하는 함수 with Backtracing
	int m, int n, int **Table,
	int ins_cost, int del_cost, int sub_cost,
	int *opt_cnt
) {  
	int i = m, j = n;
	int ins, del, sub;
	
	while (!(i == 0 && j == 0)) {					  // (0, 0)에 도달할 때까지 반복
		ins = del = sub = INT_MAX;		// 인덱스가 좌측 또는 상단 경계선에 있는 경우

		if (j >= 1) ins = Table[i][j - 1] + ins_cost;	  // if문은 경계선 여부 체크
		if (i >= 1) del = Table[i - 1][j] + del_cost;

		if (i >= 1 && j >= 1) {
			if (SS[i - 1] == TS[j - 1])
				sub = Table[i - 1][j - 1];
			else sub = Table[i - 1][j - 1] + sub_cost;
		}

		   // 아래의 switch문은 최소값의 연산 종류에 따라 인덱스를 변화시키는 역할 수행 
		switch (Minimum_Opt(ins, del, sub, ins_cost, del_cost)) {	 // Backtracing
		case 1: i = i; j = j - 1; break;							// if insertion
		case 2: i = i - 1; j = j; break;							 // if deletion
		case 3: i = i - 1; j = j - 1; break;					 // if substitution
		}

		(*opt_cnt)++;			   // 각 순회마다 Operation Sequence의 길이를 늘린다
	}
}

void Find_Operation(	  // Top-down solution searching for the operation sequence 
	char *SS, char *TS,					  // 위의 함수와 매우 유사한 Backtracing 과정
	int m, int n, int **Table, 
	int ins_cost, int del_cost, int sub_cost,
	char **SR, char **OP, char **TR, int opt_cnt
) { 
	int i = m, j = n;
	int ins, del, sub;
	bool no_opt_flag;						     // 'no operation' 상황을 위한 플래그
	int index = opt_cnt - 1;			   // 문자열을 끝에서부터 거꾸로 구성해나갈 것
	
	while (!(i == 0 && j == 0)) {
		ins = del = sub = INT_MAX;
		no_opt_flag = false;

		if (j >= 1) ins = Table[i][j - 1] + ins_cost;
		if (i >= 1) del = Table[i - 1][j] + del_cost;

		if (i >= 1 && j >= 1) {
			if (SS[i - 1] == TS[j - 1]) {
				sub = Table[i - 1][j - 1];
				no_opt_flag = true;
			}
			else sub = Table[i - 1][j - 1] + sub_cost;
		}

		// 아래의 switch문은 Backtracing 뿐만 아니라 Operation Sequence의 구축도 수행
		switch (Minimum_Opt(ins, del, sub, ins_cost, del_cost)) {
		case 1:						// 1 means ins, 2 means del, 3 means sub/no opt
			(*SR)[index] = '*';				   // PDF의 조건을 충족시키기 위한 코드들
			(*OP)[index] = 'i';
			(*TR)[index] = TS[j - 1];		  // 이 부분에서는 인덱스 차이를 주의한다
			i = i; j = j - 1;
			index--;  break;
		case 2: 
			(*SR)[index] = SS[i - 1];
			(*OP)[index] = 'd';
			(*TR)[index] = '*';
			i = i - 1; j = j;
			index--;  break;
		case 3: 
			(*SR)[index] = SS[i - 1];
			if (no_opt_flag) (*OP)[index] = '.';
			else (*OP)[index] = 's';
			(*TR)[index] = TS[j - 1];
			i = i - 1; j = j - 1;
			index--;  break;
		}
	}
}	  // I used switch sytax instead of 'if~else' since switch has more readability

void Edit_Distance(											// main procedure of DP
	char *SS,												 // source string array
	char *TS,												 // edited string array
	int ins_cost, int del_cost, int sub_cost,			  // cost of each operation
	int **Table,													  // cost table
	char **SR,						 // source string with insertion to T marks '*'
	char **OP,					 // operation sequence('i', 'd', 's', or '.' (nop))
	char **TR,						 // edited string with deletion form S mark '*'
	// NOTE: ST, OP, TR must be allocated inside the function as needed (mini size)
	int  *Mem_Allocated						// cumulated size of allocated memories
) {
	int m = strlen(SS);
	int n = strlen(TS);
	int opt_cnt = 0;

	Calculation(SS, TS, m, n, Table, ins_cost, del_cost, sub_cost);
	Backtracing(SS, TS, m, n, Table, ins_cost, del_cost, sub_cost, &opt_cnt);

	*SR = (char*)malloc((opt_cnt + 1) * sizeof(char));
	*OP = (char*)malloc((opt_cnt + 1) * sizeof(char));
	*TR = (char*)malloc((opt_cnt + 1) * sizeof(char));
	(*Mem_Allocated) = 3 * (opt_cnt + 1) * sizeof(char);
	(*SR)[opt_cnt] = (*OP)[opt_cnt] = (*TR)[opt_cnt] = '\0';	// 문자열이므로 끝의 널 처리
	
	Find_Operation(SS, TS, m, n, Table, ins_cost, del_cost, sub_cost, SR, OP, TR, opt_cnt);
}