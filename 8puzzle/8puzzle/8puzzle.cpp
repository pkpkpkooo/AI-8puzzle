// 8puzzle.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

//#include "stdafx.h"
#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <time.h>
#define _CRT_CECURE_NO_WARNINGS
using namespace std;
time_t start_t;
time_t end_t;
//������ 0�� ��ġ ����
struct zero_position {
	int i;
	int j;
};
struct puzzle {
	int evalue;
	int panel[3][3];
	zero_position zp;
	puzzle *parent;
	list<puzzle> child;
};

//��������Լ�
void print_puzzle(puzzle p) {
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << p.panel[i][j] << " ";
		}
		cout << endl;
	}
}

//start�� goal ���� �ʱ�ȭ�Լ�
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

//Ǯ�̰��� ����Լ�
void print_answer(vector <puzzle> sol) {
	cout << "������ ã�ҽ��ϴ�" << endl;
	cout << end_t - start_t << " �� �ɷȽ��ϴ�." << endl;
	for (int i = 0; i < sol.size(); i++) {
		print_puzzle(sol[i]);
		cout << endl;
	}
}
//hamming �Լ�
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

//child ��� �����Լ�, 0��ġ���� ������������ �� �� �� �� ����
void make_nodes(puzzle* p) {
	puzzle tmp;
	puzzle *parent = p->parent;
	bool isNode = true;	//�̵��� ���°� ���� �������� üũ�ϴ� �÷���

	if (p->zp.j > 0) {	//�¹������� 0�� �̵��Ҽ� �ִ��� üũ
		memcpy(tmp.panel, p->panel, sizeof(int) * 9);	//tmp�� panel�� ���� ������ �� 0�� ��ġ�� ���������̵��Ѵ�
		tmp.panel[p->zp.i][p->zp.j] = p->panel[p->zp.i][p->zp.j - 1];
		tmp.panel[p->zp.i][p->zp.j - 1] = p->panel[p->zp.i][p->zp.j];

		while (parent != NULL) {		//�θ� �Ž����ö󰡸� hamming���� �ߺ��� �������� �˻�
			if (9 > hamming(*parent, tmp))
				parent = parent->parent;
			else {							//�θ��� �ߺ��� ���°� �����ϸ� isNode�� false�� �ٲٰ� break
				isNode = false;
				break;
			}
		}
		if (isNode) { //���ν� �����ϴ� �ǴܵǸ�
			tmp.zp.i = p->zp.i;
			tmp.zp.j = p->zp.j - 1;
			tmp.parent = p;
			p->child.push_back(tmp);
		}
	}
	parent = p->parent;
	isNode = true;
	if (p->zp.i > 0) {	//��������� 0�� �̵��� �� �ִ��� üũ
		memcpy(tmp.panel, p->panel, sizeof(int) * 9);	//tmp�� panel�� ���� ������ �� 0�� ��ġ�� ���������̵��Ѵ�
		tmp.panel[p->zp.i][p->zp.j] = p->panel[p->zp.i - 1][p->zp.j];
		tmp.panel[p->zp.i - 1][p->zp.j] = p->panel[p->zp.i][p->zp.j];

		while (parent != NULL) {		//�θ� �Ž����ö󰡸� hamming���� �ߺ��� �������� �˻�
			if (9 > hamming(*parent, tmp))
				parent = parent->parent;
			else {							//�θ��� �ߺ��� ���°� �����ϸ� isNode�� false�� �ٲٰ� break
				isNode = false;
				break;
			}
		}
		if (isNode) { //���ν� �����ϴ� �ǴܵǸ�
			tmp.zp.i = p->zp.i - 1;
			tmp.zp.j = p->zp.j;
			tmp.parent = p;
			p->child.push_back(tmp);
		}
	}
	parent = p->parent;
	isNode = true;
	if (p->zp.j < 2) {	//��������� 0�� �̵��� �� �ִ��� üũ
		memcpy(tmp.panel, p->panel, sizeof(int) * 9);	//tmp�� panel�� ���� ������ �� 0�� ��ġ�� ���������̵��Ѵ�
		tmp.panel[p->zp.i][p->zp.j] = p->panel[p->zp.i][p->zp.j + 1];
		tmp.panel[p->zp.i][p->zp.j + 1] = p->panel[p->zp.i][p->zp.j];

		while (parent != NULL) {		//�θ� �Ž����ö󰡸� hamming���� �ߺ��� �������� �˻�
			if (9 > hamming(*parent, tmp))
				parent = parent->parent;
			else {							//�θ��� �ߺ��� ���°� �����ϸ� isNode�� false�� �ٲٰ� break
				isNode = false;
				break;
			}
		}
		if (isNode) { //���ν� �����ϴ� �ǴܵǸ�
			tmp.zp.i = p->zp.i;
			tmp.zp.j = p->zp.j + 1;
			tmp.parent = p;
			p->child.push_back(tmp);
		}
	}
	parent = p->parent;
	isNode = true;
	if (p->zp.i < 2) {	//�Ϲ������� 0�� �̵��� �� �ִ��� üũ
		memcpy(tmp.panel, p->panel, sizeof(int) * 9);	//tmp�� panel�� ���� ������ �� 0�� ��ġ�� ���������̵��Ѵ�
		tmp.panel[p->zp.i][p->zp.j] = p->panel[p->zp.i + 1][p->zp.j];
		tmp.panel[p->zp.i + 1][p->zp.j] = p->panel[p->zp.i][p->zp.j];

		while (parent != NULL) {		//�θ� �Ž����ö󰡸� hamming���� �ߺ��� �������� �˻�
			if (9 > hamming(*parent, tmp))
				parent = parent->parent;
			else {							//�θ��� �ߺ��� ���°� �����ϸ� isNode�� false�� �ٲٰ� break
				isNode = false;
				break;
			}
		}
		if (isNode) { //���ν� �����ϴ� �ǴܵǸ�
			tmp.zp.i = p->zp.i + 1;
			tmp.zp.j = p->zp.j;
			tmp.parent = p;
			p->child.push_back(tmp);
		}
	}
}

//���̿켱 Ž��
void DFS(puzzle start, puzzle goal) {
	start_t = time(NULL);
	end_t = time(NULL);
	cout << "start DFS" << endl;
	stack<puzzle> OPEN;
	OPEN.push(start);
	while (OPEN.size() > 0 && end_t - start_t < 5) {
		end_t = time(NULL);
		if (OPEN.top().child.size() == 0)
			make_nodes(&OPEN.top());
		if (hamming(OPEN.top(), goal) < 8) {	//hamming�� ��� point�� 8���� ������(goal�� ��ġ���� ������) ������带 OPEN�� push
			if (OPEN.size() < 6) {	//���̸� 5�� �����Ѵ�.
				OPEN.push(OPEN.top().child.front());
			}else {
				do {
					OPEN.top().parent->child.pop_front(); //���ÿ� push�� ���� child����Ʈ���� �����Ѵ�.
					OPEN.pop();	//���̰� 5�� �����ϸ� ������ pop�Ѵ�
				} while (OPEN.top().child.size() == 0);
			}
		}
		else {	//������ ã�����
			vector<puzzle> sol;
			while(OPEN.size() > 0) { //OPEN�� �ִ� �������� �������� list�� ����
				sol.push_back(OPEN.top());
				OPEN.pop();
			}
			reverse(sol.begin(), sol.end());
			print_answer(sol);
			return;
		}
	}
	cout << "���� ã�� ���Ͽ����ϴ�. �ɸ��ð� : " << end_t - start_t  << " ��"<< endl;
}

//�ʺ�켱 Ž��
void BFS(puzzle start, puzzle goal) {
	start_t = time(NULL);
	end_t = time(NULL);
	cout << "start BFS" << endl;
	list<puzzle> OPEN;
	list<puzzle> CLOSE;
	OPEN.push_back(start);

	while(OPEN.size() > 0 && end_t - start_t < 5 ){	//OPEN�� ���̻� ��尡 ���ų�, ����� ������ õ���� �Ѿ�� ����
		end_t = time(NULL);
		if (hamming(OPEN.front(), goal) < 8) {	//OPEN�� ���� �տ� �ִ� ��带 ��ǥ�� ���ؼ� ��ġ���� ������ 
			CLOSE.push_back(OPEN.front());		//OPEN�� ù���� ��带 CLOSE�� �����ϰ� pop�Ѵ�
			OPEN.pop_front();	
			make_nodes(&CLOSE.back());			//CLOSE�� ������ ��忡�� �ڽĳ�带 ������ش�
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
	cout << "���� ã�� ���Ͽ����ϴ�. �ɸ��ð� : " << end_t - start_t << " ��" << endl;
}

//��������� Ž��
void HillClimbing(puzzle start, puzzle goal, int ef(puzzle a, puzzle b)){
	start_t = time(NULL);
	end_t = time(NULL);
	cout << "start Hill climbing" << endl;
	list<puzzle> OPEN;
	list<puzzle> CLOSE;
	OPEN.push_back(start);

	while(OPEN.size() > 0  && end_t - start_t < 5){
		CLOSE.push_back(OPEN.front());
		make_nodes(&CLOSE.back());
		if(CLOSE.back().child.size() > 0){	//CLOSE�� �� ���ҿ� child�� �����ϸ�
			list<puzzle>::iterator it;
			for(it = CLOSE.back().child.begin() ; it != CLOSE.back().child.end(); it++){
				if(ef(*it, goal) < 8){
					it->evalue = ef(*it, goal);
				}else{
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
			CLOSE.back().child.sort(puzzle().evalue);
		}
	
	}



}

//�����켱 Ž��
void BestFirstSearch(puzzle start, puzzle goal){
}

//A* Ž��
void AStar(puzzle start, puzzle goal){

}
int main(int argc, char* argv[])
{
	FILE *in;
	FILE *g;
	puzzle start;
	puzzle goal;

	if (in = fopen("in.txt", "r"));	//�Է°�, ��ǥ�� �о����
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
	//BFS(start, goal);



	fclose(in);
	fclose(g);
	return 0;
}

