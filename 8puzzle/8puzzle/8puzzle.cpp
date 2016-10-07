// 8puzzle.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

//#include "stdafx.h"
#include <iostream>
#include <list>
#include <stack>
#include <vector>
#define _CRT_CECURE_NO_WARNINGS
using namespace std;

//퍼즐의 0의 위치 변수
struct zero_position {
	int i;
	int j;
};
struct puzzle {
	int panel[3][3];
	zero_position zp;
	puzzle *parent;
	list<puzzle> child;
};

//퍼즐출력함수
void print_puzzle(puzzle p) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << p.panel[i][j] << " ";
		}
		cout << endl;
	}
}

//start와 goal 퍼즐 초기화함수
void init_puzzle(puzzle *p, FILE *f) {
	for (int i = 0; i < 3; i++) {
		fscanf_s(f, "%d %d %d", &p->panel[i][0], &p->panel[i][1], &p->panel[i][2]);
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << p->panel[i][j] << " ";
			if (p->panel[i][j] == 0) {
				p->zp.i = i;
				p->zp.j = j;
			}
		}
		cout << endl;
	}
}

//풀이과정 출력함수
void print_answer(vector <puzzle> sol) {
	cout << "정답을 찾았습니다" << endl;
	for (int i = 0; i < sol.size(); i++) {
		print_puzzle(sol[i]);
		cout << endl;
	}
}
//hamming 함수
int hamming(puzzle a, puzzle b) {
	int point = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (a.panel[i][j] == b.panel[i][j])
				point++;
		}
	}
	return point;
}

//child 노드 생성함수, 0위치기준 노드생성순서는 좌 상 우 하 순서
void make_nodes(puzzle* p) {
	puzzle tmp;
	puzzle *parent = p->parent;
	bool isNode = true;	//이동한 상태가 노드로 적합한지 체크하는 플래그

	if (p->zp.j > 0) {	//좌방향으로 0을 이동할수 있는지 체크
		memcpy(tmp.panel, p->panel, sizeof(int) * 9);	//tmp의 panel로 값을 복사한 후 0의 위치를 왼쪽으로이동한다
		tmp.panel[p->zp.i][p->zp.j] = p->panel[p->zp.i][p->zp.j - 1];
		tmp.panel[p->zp.i][p->zp.j - 1] = p->panel[p->zp.i][p->zp.j];

		while (parent != NULL) {		//부모를 거슬러올라가며 hamming으로 중복된 상태인지 검사
			if (9 > hamming(*parent, tmp))
				parent = parent->parent;
			else {							//부모중 중복된 상태가 존재하면 isNode를 false로 바꾸고 break
				isNode = false;
				break;
			}
		}
		if (isNode) { //노드로써 적합하다 판단되면
			tmp.zp.i = p->zp.i;
			tmp.zp.j = p->zp.j - 1;
			tmp.parent = p;
			p->child.push_back(tmp);
		}
	}
	parent = p->parent;
	isNode = true;
	if (p->zp.i > 0) {	//상방향으로 0을 이동할 수 있는지 체크
		memcpy(tmp.panel, p->panel, sizeof(int) * 9);	//tmp의 panel로 값을 복사한 후 0의 위치를 왼쪽으로이동한다
		tmp.panel[p->zp.i][p->zp.j] = p->panel[p->zp.i - 1][p->zp.j];
		tmp.panel[p->zp.i - 1][p->zp.j] = p->panel[p->zp.i][p->zp.j];

		while (parent != NULL) {		//부모를 거슬러올라가며 hamming으로 중복된 상태인지 검사
			if (9 > hamming(*parent, tmp))
				parent = parent->parent;
			else {							//부모중 중복된 상태가 존재하면 isNode를 false로 바꾸고 break
				isNode = false;
				break;
			}
		}
		if (isNode) { //노드로써 적합하다 판단되면
			tmp.zp.i = p->zp.i - 1;
			tmp.zp.j = p->zp.j;
			tmp.parent = p;
			p->child.push_back(tmp);
		}
	}
	parent = p->parent;
	isNode = true;
	if (p->zp.j < 2) {	//우방향으로 0을 이동할 수 있는지 체크
		memcpy(tmp.panel, p->panel, sizeof(int) * 9);	//tmp의 panel로 값을 복사한 후 0의 위치를 왼쪽으로이동한다
		tmp.panel[p->zp.i][p->zp.j] = p->panel[p->zp.i][p->zp.j + 1];
		tmp.panel[p->zp.i][p->zp.j + 1] = p->panel[p->zp.i][p->zp.j];

		while (parent != NULL) {		//부모를 거슬러올라가며 hamming으로 중복된 상태인지 검사
			if (9 > hamming(*parent, tmp))
				parent = parent->parent;
			else {							//부모중 중복된 상태가 존재하면 isNode를 false로 바꾸고 break
				isNode = false;
				break;
			}
		}
		if (isNode) { //노드로써 적합하다 판단되면
			tmp.zp.i = p->zp.i;
			tmp.zp.j = p->zp.j + 1;
			tmp.parent = p;
			p->child.push_back(tmp);
		}
	}
	parent = p->parent;
	isNode = true;
	if (p->zp.i < 2) {	//하방향으로 0을 이동할 수 있는지 체크
		memcpy(tmp.panel, p->panel, sizeof(int) * 9);	//tmp의 panel로 값을 복사한 후 0의 위치를 왼쪽으로이동한다
		tmp.panel[p->zp.i][p->zp.j] = p->panel[p->zp.i + 1][p->zp.j];
		tmp.panel[p->zp.i + 1][p->zp.j] = p->panel[p->zp.i][p->zp.j];

		while (parent != NULL) {		//부모를 거슬러올라가며 hamming으로 중복된 상태인지 검사
			if (9 > hamming(*parent, tmp))
				parent = parent->parent;
			else {							//부모중 중복된 상태가 존재하면 isNode를 false로 바꾸고 break
				isNode = false;
				break;
			}
		}
		if (isNode) { //노드로써 적합하다 판단되면
			tmp.zp.i = p->zp.i + 1;
			tmp.zp.j = p->zp.j;
			tmp.parent = p;
			p->child.push_back(tmp);
		}
	}
}

//깊이우선 탐색
void DFS(puzzle start, puzzle goal) {
	cout << "start DFS" << endl;
	stack<puzzle> OPEN;
	OPEN.push(start);
	while (OPEN.size() > 0) {
		if (OPEN.top().child.size() == 0)
			make_nodes(&OPEN.top());
		if (hamming(OPEN.top(), goal) < 8) {	//hamming의 결과 point가 8보다 작으면(goal과 일치하지 않으면) 다음노드를 OPEN에 push
			if (OPEN.size() < 6) {	//깊이를 5로 제한한다.
				OPEN.push(OPEN.top().child.front());
				//print_puzzle(OPEN.top());
				//cout << endl;
			}else {
				do {
					OPEN.top().parent->child.pop_front(); //스택에 push한 노드는 child리스트에서 제거한다.
					OPEN.pop();	//깊이가 5에 도달하면 스택을 pop한다
				} while (OPEN.top().child.size() == 0);
			}
		}
		else {	//정답을 찾을경우
			
			vector<puzzle> sol;
			while(OPEN.size() > 0) { //OPEN에 있는 퍼즐목록을 역순으로 list에 삽입
				sol.push_back(OPEN.top());
				OPEN.pop();
			}
			reverse(sol.begin(), sol.end());
			print_answer(sol);
			return;
		}
		char a;
		//scanf_s("%c",&a);
	}
	cout << "답을 찾지 못하였습니다." << endl;
}

//너비우선 탐색
void BFS(puzzle start, puzzle goal) {
	cout << "start BFS" << endl;
	list<puzzle> OPEN;
	list<puzzle> CLOSE;
	OPEN.push_back(start);

	while(OPEN.size() > 0){
		if (hamming(OPEN.front(), goal) < 8) {	//OPEN의 가장 앞에 있는 노드를 목표와 비교해서 일치하지 않으면 
			CLOSE.push_back(OPEN.front());		//OPEN의 첫번쨰 노드를 CLOSE로 복사하고 pop한다
			OPEN.pop_front();	
			make_nodes(&CLOSE.back());			//CLOSE의 마지막 노드에게 자식노드를 만들어준다
			list<puzzle>::iterator it;
			for (it = CLOSE.back().child.begin(); it != CLOSE.back().child.end(); it++) {
				OPEN.push_back(*it);
			}
			
		}
		else {
			vector<puzzle> sol;
			puzzle *tmp = &OPEN.front();
			do {
				sol.push_back(*tmp);
				tmp = tmp->parent;
			} while (tmp != NULL);
			reverse(sol.begin(), sol.end());
			print_answer(sol);
			return;
		}

	}
}

//언덕오르기 탐색
void HillClimbing(puzzle start, puzzle goal){
}

//최적우선 탐색
void BestFirstSearch(puzzle start, puzzle goal){
}

//A* 탐색
void AStar(puzzle start, puzzle goal){

}
int main(int argc, char* argv[])
{
	FILE *in;
	FILE *g;
	puzzle start;
	puzzle goal;

	if (in = fopen("in.txt", "r"));	//입력값, 목표값 읽어오기
	else {
		printf("file open error in in.txt\n");
		return -1;
	}
	if (g = fopen("g.txt", "r"));
	else {
		printf("file open error in g.txt\n");
		return -1;
	}

	cout << "start state :\n" << endl;
	init_puzzle(&start, in);
	cout << "zero position : i = " << start.zp.i << ", j = " << start.zp.j << endl;
	cout << "\ngoal state :" << endl;
	init_puzzle(&goal, g);
	cout << "zero position : i = " << goal.zp.i << ", j = " << goal.zp.j << endl;
	printf("\n");
	start.parent = NULL;

	DFS(start, goal);
	BFS(start, goal);



	fclose(in);
	fclose(g);
	return 0;
}

