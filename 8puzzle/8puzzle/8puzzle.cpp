// 8puzzle.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

//#include "stdafx.h"
#include <iostream>
#include <list>
#include <stack>
//#define _CRT_CECURE_NO_WARNINGS
using namespace std;

//������ 0�� ��ġ ����
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

//���̿켱Ž��
void DFS(puzzle start, puzzle goal) {
	stack<puzzle> OPEN;
	OPEN.push(start);
	while (OPEN.size() > 0) {
		if (OPEN.top().child.size() == 0)
			make_nodes(&OPEN.top());
		if (hamming(OPEN.top(), goal) < 8) {	//hamming�� ��� point�� 8���� ������(goal�� ��ġ���� ������) ������带 OPEN�� push
			if (OPEN.size() < 5) {	//���̸� 5�� �����Ѵ�.
				OPEN.push(OPEN.top().child.front());
				print_puzzle(OPEN.top());
				cout << endl;
			}
			else {
				do {
					OPEN.top().parent->child.pop_front(); //���ÿ� push�� ���� child����Ʈ���� �����Ѵ�.
					OPEN.pop();	//���̰� 5�� �����ϸ� ������ pop�Ѵ�
				} while (OPEN.top().child.size() == 0);

				/*				if (OPEN.top().child.size() == 0) {	//pop�� �� ������ top�� ���� child��尡 ������ �ѹ� �� pop�Ѵ�.
				OPEN.top().parent->child.pop_front();
				OPEN.pop();
				}*/
			}
		}
		else {	//������ ã�����
			cout << "������ ã�ҽ��ϴ�" << endl;
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




	fclose(in);
	fclose(g);
	return 0;
}
