// 8puzzle.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

//#include "stdafx.h"
#include <iostream>
#include <list>
#include <stack>
//#define _CRT_CECURE_NO_WARNINGS
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

//깊이우선탐색
void DFS(puzzle start, puzzle goal) {
	stack<puzzle> OPEN;
	OPEN.push(start);
	while (OPEN.size() > 0) {
		if (OPEN.top().child.size() == 0)
			make_nodes(&OPEN.top());
		if (hamming(OPEN.top(), goal) < 8) {	//hamming의 결과 point가 8보다 작으면(goal과 일치하지 않으면) 다음노드를 OPEN에 push
			if (OPEN.size() < 5) {	//깊이를 5로 제한한다.
				OPEN.push(OPEN.top().child.front());
				print_puzzle(OPEN.top());
				cout << endl;
			}
			else {
				do {
					OPEN.top().parent->child.pop_front(); //스택에 push한 노드는 child리스트에서 제거한다.
					OPEN.pop();	//깊이가 5에 도달하면 스택을 pop한다
				} while (OPEN.top().child.size() == 0);

				/*				if (OPEN.top().child.size() == 0) {	//pop한 후 현재의 top가 가진 child노드가 없으면 한번 더 pop한다.
				OPEN.top().parent->child.pop_front();
				OPEN.pop();
				}*/
			}
		}
		else {	//정답을 찾을경우
			cout << "정답을 찾았습니다" << endl;
			print_puzzle(OPEN.top());
		}
		char a;
		//scanf_s("%c",&a);
	}

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




	fclose(in);
	fclose(g);
	return 0;
}

