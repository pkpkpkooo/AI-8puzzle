// 8puzzle.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
//#include "stdafx.h"
#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <time.h>
#include <algorithm>
#define _CRT_CECURE_NO_WARNINGS
using namespace std;
time_t start_t;
time_t end_t;
//퍼즐의 0의 위치 변수
struct zero_position {
	int i;
	int j;
};
struct puzzle {
	int evalue;
	int dist;
	int panel[3][3];
	zero_position zp;
	puzzle *parent;
	list<puzzle> child;
};

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

//퍼즐출력함수
void print_puzzle(puzzle p) {
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << p.panel[i][j] << " ";
		}
		cout << endl;
	}
	cout << "평가함수 값 : " << p.evalue << endl;
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
	end_t = clock();
	cout << "정답을 찾았습니다" << endl;
	//cout << (float)(end_t - start_t) / (CLOCKS_PER_SEC) << " 초 걸렸습니다." << endl;
	printf("%f 초 걸렸습니다.\n", (float)(end_t - start_t) / (CLOCKS_PER_SEC));
	for (unsigned int i = 0; i < sol.size(); i++) {
		print_puzzle(sol[i]);
		std::cout << endl;
	}
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
		tmp.dist = p->dist + 1;	//자식은 부모보다 거리가중치가 1 많아진다.
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
		tmp.dist = p->dist + 1;	//자식은 부모보다 거리가중치가 1 많아진다.
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
		tmp.dist = p->dist + 1;	//자식은 부모보다 거리가중치가 1 많아진다.
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
		tmp.dist = p->dist + 1;	//자식은 부모보다 거리가중치가 1 많아진다.
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
	start_t = clock();
	cout << "start DFS" << endl;
	stack<puzzle> OPEN;
	OPEN.push(start);
	do{
		//
		if (OPEN.top().child.size() == 0)
			make_nodes(&OPEN.top());
		OPEN.top().evalue = hamming(OPEN.top(), goal);
		if (OPEN.top().evalue < 8) {	//hamming의 결과 point가 8보다 작으면(goal과 일치하지 않으면) 다음노드를 OPEN에 push
			if (OPEN.size() < 15) {	//깊이를 5로 제한한다.
				OPEN.push(OPEN.top().child.front());
			}else {
				do {
					OPEN.top().parent->child.pop_front(); //스택에 push한 노드는 child리스트에서 제거한다.
					OPEN.pop();	//깊이가 5에 도달하면 스택을 pop한다
				} while (OPEN.top().child.size() == 0 && OPEN.size() > 1);
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
	}while (OPEN.size() > 0 && !((hamming(OPEN.top(), start) > 8) && (OPEN.top().child.size() == 0)));
	//&& !( (hamming(OPEN.top(),start) > 8) && (OPEN.top().child.size() == 0) ) 
	end_t = clock();
	cout << "답을 찾지 못하였습니다. 걸린시간 : " << (float)(end_t - start_t) / (CLOCKS_PER_SEC) << " 초"<< endl;
}

//너비우선 탐색
void BFS(puzzle start, puzzle goal) {
	start_t = clock();
	cout << "start BFS" << endl;
	list<puzzle> OPEN;
	list<puzzle> CLOSE;
	OPEN.push_back(start);

	while(OPEN.size() > 0 && end_t - start_t < 5 ){	//OPEN에 더이상 노드가 없거나, 노드의 갯수가 천개가 넘어가면 종료
		//end_t = clock();
		OPEN.front().evalue = hamming(OPEN.front(), goal);
		if (OPEN.front().evalue < 8) {	//OPEN의 가장 앞에 있는 노드를 목표와 비교해서 일치하지 않으면 
			CLOSE.push_back(OPEN.front());		//OPEN의 첫번쨰 노드를 CLOSE로 복사하고 pop한다
			OPEN.pop_front();	 
			make_nodes(&CLOSE.back());			//CLOSE의 마지막 노드에게 자식노드를 만들어준다
			list<puzzle>::iterator it;
			for (it = CLOSE.back().child.begin(); it != CLOSE.back().child.end(); it++) {
				OPEN.push_back(*it);
			}
		}else {
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
	cout << "답을 찾지 못하였습니다. 걸린시간 : " << end_t - start_t << " 초" << endl;
}

bool pred(puzzle a, puzzle b) { return  a.evalue < b.evalue ? true : false; }

//언덕오르기 탐색
void HillClimbing(puzzle start, puzzle goal, int ef(puzzle a, puzzle b)){
	start_t = clock();
	cout << "start Hill climbing" << endl;
	list<puzzle> OPEN;
	list<puzzle> CLOSE;
	OPEN.push_back(start);

	while(OPEN.size() > 0 ){
		end_t = clock();
		CLOSE.push_back(OPEN.front());
		make_nodes(&CLOSE.back());
		OPEN.pop_front();

		if(CLOSE.back().child.size() > 0){	//CLOSE에 들어간 원소에 child가 존재하면

			list<puzzle>::iterator it;
			for(it = CLOSE.back().child.begin() ; it != CLOSE.back().child.end(); it++){	//각 child의 평가함수점수를 구한다
				it->evalue = ef(*it, goal);		//평가함수의 점수를 저장함
				if(it->evalue >= 8) {			//평가함수의 점수가 8이상이면 정답					
					vector<puzzle> sol;
					puzzle *tmp = &*it;
					do {
						sol.push_back(*tmp);
						tmp = tmp->parent;
					} while (tmp != NULL);
					reverse(sol.begin(), sol.end());
					print_answer(sol);
					return;
				}
			}
			CLOSE.back().child.sort(pred);	//child들을 평가함수의 점수기준 오름차순으로 정렬
			for(it=CLOSE.back().child.begin(); it != CLOSE.back().child.end(); it++){		//child들을 OPEN의 앞쪽에 삽입
				OPEN.push_front(*it);
			}
		}
	}

}

//최적우선 탐색
void BestFirstSearch(puzzle start, puzzle goal, int ef(puzzle a, puzzle b)) {
	start_t = clock();
	cout << "start BestFirstSearch" << endl;
	list<puzzle> OPEN;
	list<puzzle> CLOSE;
	OPEN.push_back(start);

	while (OPEN.size() > 0) {
		end_t = clock();
		CLOSE.push_back(OPEN.back());
		make_nodes(&CLOSE.back());
		OPEN.pop_back();
		if (CLOSE.back().child.size() > 0) {	//CLOSE에 들어간 원소에 child가 존재하면
			list<puzzle>::iterator it;
			for (it = CLOSE.back().child.begin(); it != CLOSE.back().child.end(); it++) {	//각 child의 평가함수점수를 구한다
				it->evalue = ef(*it, goal);		//평가함수의 점수를 저장함
				if (it->evalue >= 8) {			//평가함수의 점수가 8이상이면 정답					
					vector<puzzle> sol;
					puzzle *tmp = &*it;
					do {
						sol.push_back(*tmp);
						tmp = tmp->parent;
					} while (tmp != NULL);
					reverse(sol.begin(), sol.end());
					print_answer(sol);
					return;
				}
			}
			for (it = CLOSE.back().child.begin(); it != CLOSE.back().child.end(); it++) {		//child들을 OPEN에 삽입
				OPEN.push_back(*it);
			}
			OPEN.sort(pred);	//OPEN을 정렬하여 가장 큰값을 마지막으로 둔다
		}
	}
}

bool Astar_pred(puzzle a, puzzle b) { return  (a.evalue - a.dist) > (b.evalue - b.dist) ? true : false; }	//노드->목표 까지의 예상경로비용 - 루트->노드 까지의 경로비용 순으로 정렬 

void reset_dist(puzzle *a){		//입력받은 노드의 자식노드들의 dist를 입력받은 노드의 dist를 기준으로 재설정한다
	if(a->child.size()){
		for (list<puzzle>::iterator it = a->child.begin(); it != a->child.begin(); it++){
			it->dist = a->dist + 1;
			reset_dist(&*it);
		}
	}
		return;
	
}

//A* 탐색
void AStar(puzzle start, puzzle goal, int ef(puzzle a, puzzle b)) {
	start_t = clock();
	cout << "start A* algorithm" << endl;
	list<puzzle> OPEN;
	list<puzzle> CLOSE;
	OPEN.push_back(start);

	while (OPEN.size() > 0) {
		end_t = clock();
		CLOSE.push_back(OPEN.front());
		make_nodes(&CLOSE.back());
		OPEN.pop_front();
		if (CLOSE.back().child.size() > 0) {	//CLOSE에 들어간 원소에 child가 존재하면
			list<puzzle>::iterator it = CLOSE.back().child.begin();
			while (it != CLOSE.back().child.end()) {	//각 child의 평가함수점수를 구한다
				it->evalue = ef(*it, goal);		//평가함수의 점수를 저장함
				if (it->evalue >= 8) {			//평가함수의 점수가 8이상이면 정답					
					vector<puzzle> sol;
					puzzle *tmp = &*it;
					do {
						sol.push_back(*tmp);
						tmp = tmp->parent;
					} while (tmp != NULL);
					reverse(sol.begin(), sol.end());
					print_answer(sol);
					return;
				}
				list<puzzle>::iterator oit = OPEN.begin();
				while ( oit != OPEN.end() && it != CLOSE.back().child.end()) {		//OPEN에 중복되는 노드가 있는지 검사
					
					if (hamming(*it, *oit) >= 8) {		//중복되는 경우가 있다면
						if (it->evalue - it->dist > oit->evalue - oit->dist) { 	//child의 g() + h()가 더 큰경우에는 OPEN에 있는 중복노드를삭제
							oit = OPEN.erase(oit);							
						}
						else {												//그외의 경우에는 child에 있는 중복노드를 삭제
							it = CLOSE.back().child.erase(it);
						}
					}
					else
						oit++;
				}

				list<puzzle>::iterator cit = CLOSE.begin();
				while ( cit != CLOSE.end() && it != CLOSE.back().child.end()) {		//CLOSE에 중복되는 노드가 있는지 검사
					if (hamming(*it, *cit) >= 8) {		//중복되는 경우가 있다면
						if (it->evalue - it->dist > cit->evalue - cit->dist) {	//child의 g() + h()가 더 큰경우에는 CLOSE에 있는 중복노드를삭제
							*cit->parent = *it->parent;
							cit->dist = it->dist;
							reset_dist(&*cit);
							it = CLOSE.back().child.erase(it);								
						}
						else {													//그외의 경우에는 child에 있는 중복노드를 삭제
							it = CLOSE.back().child.erase(it);
							
						}
					}
					cit++;
				}
				if (it != CLOSE.back().child.end()) { OPEN.push_back(*it); it++; }
				OPEN.sort(Astar_pred);	//OPEN을 정렬하여 g() + h()의 값이 가장 큰노드를 마지막으로 둔다
			}
		}
		
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
		cout <<"file open error in in.txt" << endl;
		return -1;
	}
	if (g = fopen("g.txt", "r"));
	else {
		cout << "file open error in g.txt" << endl;
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
	start.evalue = hamming(start, goal);
	start.dist = 0;

	DFS(start, goal);
	BFS(start, goal);
	HillClimbing(start, goal, hamming);
	BestFirstSearch(start, goal, hamming);
	AStar(start, goal, hamming);

	fclose(in);
	fclose(g);
	return 0;
}

