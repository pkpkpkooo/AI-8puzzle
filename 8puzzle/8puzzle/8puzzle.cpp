// 8puzzle.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
//������ 0�� ��ġ ����
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

//��������Լ�
void print_puzzle(puzzle p) {
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << p.panel[i][j] << " ";
		}
		cout << endl;
	}
	cout << "���Լ� �� : " << p.evalue << endl;
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
	end_t = clock();
	cout << "������ ã�ҽ��ϴ�" << endl;
	//cout << (float)(end_t - start_t) / (CLOCKS_PER_SEC) << " �� �ɷȽ��ϴ�." << endl;
	printf("%f �� �ɷȽ��ϴ�.\n", (float)(end_t - start_t) / (CLOCKS_PER_SEC));
	for (unsigned int i = 0; i < sol.size(); i++) {
		print_puzzle(sol[i]);
		std::cout << endl;
	}
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
		tmp.dist = p->dist + 1;	//�ڽ��� �θ𺸴� �Ÿ�����ġ�� 1 ��������.
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
		tmp.dist = p->dist + 1;	//�ڽ��� �θ𺸴� �Ÿ�����ġ�� 1 ��������.
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
		tmp.dist = p->dist + 1;	//�ڽ��� �θ𺸴� �Ÿ�����ġ�� 1 ��������.
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
		tmp.dist = p->dist + 1;	//�ڽ��� �θ𺸴� �Ÿ�����ġ�� 1 ��������.
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
	start_t = clock();
	cout << "start DFS" << endl;
	stack<puzzle> OPEN;
	OPEN.push(start);
	do{
		//
		if (OPEN.top().child.size() == 0)
			make_nodes(&OPEN.top());
		OPEN.top().evalue = hamming(OPEN.top(), goal);
		if (OPEN.top().evalue < 8) {	//hamming�� ��� point�� 8���� ������(goal�� ��ġ���� ������) ������带 OPEN�� push
			if (OPEN.size() < 15) {	//���̸� 5�� �����Ѵ�.
				OPEN.push(OPEN.top().child.front());
			}else {
				do {
					OPEN.top().parent->child.pop_front(); //���ÿ� push�� ���� child����Ʈ���� �����Ѵ�.
					OPEN.pop();	//���̰� 5�� �����ϸ� ������ pop�Ѵ�
				} while (OPEN.top().child.size() == 0 && OPEN.size() > 1);
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
	}while (OPEN.size() > 0 && !((hamming(OPEN.top(), start) > 8) && (OPEN.top().child.size() == 0)));
	//&& !( (hamming(OPEN.top(),start) > 8) && (OPEN.top().child.size() == 0) ) 
	end_t = clock();
	cout << "���� ã�� ���Ͽ����ϴ�. �ɸ��ð� : " << (float)(end_t - start_t) / (CLOCKS_PER_SEC) << " ��"<< endl;
}

//�ʺ�켱 Ž��
void BFS(puzzle start, puzzle goal) {
	start_t = clock();
	cout << "start BFS" << endl;
	list<puzzle> OPEN;
	list<puzzle> CLOSE;
	OPEN.push_back(start);

	while(OPEN.size() > 0 && end_t - start_t < 5 ){	//OPEN�� ���̻� ��尡 ���ų�, ����� ������ õ���� �Ѿ�� ����
		//end_t = clock();
		OPEN.front().evalue = hamming(OPEN.front(), goal);
		if (OPEN.front().evalue < 8) {	//OPEN�� ���� �տ� �ִ� ��带 ��ǥ�� ���ؼ� ��ġ���� ������ 
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

bool pred(puzzle a, puzzle b) { return  a.evalue < b.evalue ? true : false; }

//��������� Ž��
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

		if(CLOSE.back().child.size() > 0){	//CLOSE�� �� ���ҿ� child�� �����ϸ�

			list<puzzle>::iterator it;
			for(it = CLOSE.back().child.begin() ; it != CLOSE.back().child.end(); it++){	//�� child�� ���Լ������� ���Ѵ�
				it->evalue = ef(*it, goal);		//���Լ��� ������ ������
				if(it->evalue >= 8) {			//���Լ��� ������ 8�̻��̸� ����					
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
			CLOSE.back().child.sort(pred);	//child���� ���Լ��� �������� ������������ ����
			for(it=CLOSE.back().child.begin(); it != CLOSE.back().child.end(); it++){		//child���� OPEN�� ���ʿ� ����
				OPEN.push_front(*it);
			}
		}
	}

}

//�����켱 Ž��
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
		if (CLOSE.back().child.size() > 0) {	//CLOSE�� �� ���ҿ� child�� �����ϸ�
			list<puzzle>::iterator it;
			for (it = CLOSE.back().child.begin(); it != CLOSE.back().child.end(); it++) {	//�� child�� ���Լ������� ���Ѵ�
				it->evalue = ef(*it, goal);		//���Լ��� ������ ������
				if (it->evalue >= 8) {			//���Լ��� ������ 8�̻��̸� ����					
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
			for (it = CLOSE.back().child.begin(); it != CLOSE.back().child.end(); it++) {		//child���� OPEN�� ����
				OPEN.push_back(*it);
			}
			OPEN.sort(pred);	//OPEN�� �����Ͽ� ���� ū���� ���������� �д�
		}
	}
}

bool Astar_pred(puzzle a, puzzle b) { return  (a.evalue - a.dist) > (b.evalue - b.dist) ? true : false; }	//���->��ǥ ������ �����κ�� - ��Ʈ->��� ������ ��κ�� ������ ���� 

void reset_dist(puzzle *a){		//�Է¹��� ����� �ڽĳ����� dist�� �Է¹��� ����� dist�� �������� �缳���Ѵ�
	if(a->child.size()){
		for (list<puzzle>::iterator it = a->child.begin(); it != a->child.begin(); it++){
			it->dist = a->dist + 1;
			reset_dist(&*it);
		}
	}
		return;
	
}

//A* Ž��
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
		if (CLOSE.back().child.size() > 0) {	//CLOSE�� �� ���ҿ� child�� �����ϸ�
			list<puzzle>::iterator it = CLOSE.back().child.begin();
			while (it != CLOSE.back().child.end()) {	//�� child�� ���Լ������� ���Ѵ�
				it->evalue = ef(*it, goal);		//���Լ��� ������ ������
				if (it->evalue >= 8) {			//���Լ��� ������ 8�̻��̸� ����					
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
				while ( oit != OPEN.end() && it != CLOSE.back().child.end()) {		//OPEN�� �ߺ��Ǵ� ��尡 �ִ��� �˻�
					
					if (hamming(*it, *oit) >= 8) {		//�ߺ��Ǵ� ��찡 �ִٸ�
						if (it->evalue - it->dist > oit->evalue - oit->dist) { 	//child�� g() + h()�� �� ū��쿡�� OPEN�� �ִ� �ߺ���带����
							oit = OPEN.erase(oit);							
						}
						else {												//�׿��� ��쿡�� child�� �ִ� �ߺ���带 ����
							it = CLOSE.back().child.erase(it);
						}
					}
					else
						oit++;
				}

				list<puzzle>::iterator cit = CLOSE.begin();
				while ( cit != CLOSE.end() && it != CLOSE.back().child.end()) {		//CLOSE�� �ߺ��Ǵ� ��尡 �ִ��� �˻�
					if (hamming(*it, *cit) >= 8) {		//�ߺ��Ǵ� ��찡 �ִٸ�
						if (it->evalue - it->dist > cit->evalue - cit->dist) {	//child�� g() + h()�� �� ū��쿡�� CLOSE�� �ִ� �ߺ���带����
							*cit->parent = *it->parent;
							cit->dist = it->dist;
							reset_dist(&*cit);
							it = CLOSE.back().child.erase(it);								
						}
						else {													//�׿��� ��쿡�� child�� �ִ� �ߺ���带 ����
							it = CLOSE.back().child.erase(it);
							
						}
					}
					cit++;
				}
				if (it != CLOSE.back().child.end()) { OPEN.push_back(*it); it++; }
				OPEN.sort(Astar_pred);	//OPEN�� �����Ͽ� g() + h()�� ���� ���� ū��带 ���������� �д�
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

	if (in = fopen("in.txt", "r"));	//�Է°�, ��ǥ�� �о����
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

