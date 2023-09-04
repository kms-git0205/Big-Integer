#include"biginteger.h"
#include<string>	//string, vector, sstream ����׽�Ʈ �Լ��� ���
#include<sstream>
#include<vector>


void BigIntegerTest1(int op1, int op2) {
	BigInteger res, a, b;
	a.setint(op1); b.setint(op2);
	res = a;
	res.add(&b);
	cout << op1 << " + " << op2 << " = ";
	res.print();

	a.multiply(&res, &a, &b);
	cout << op1 << " * " << op2 << " = ";
	res.print();

	a.divide(&res, &a, &b, 0);
	cout << op1 << " / " << op2 << " = ";
	res.print();

	a.modular(&res, &a, &b);
	cout << op1 << " % " << op2 << " = ";
	res.print();

}

void BigIntegerTest2(string testfile) {
	//ù�� w ��, �ι�°�� x��
	//ù��, �ι�°���� �� ������ ���ƾ���

	ifstream input(testfile);

	if (!input.is_open()) {
		cout << "������ ã�� �� �����ϴ�." << endl;
		return;
	}

	string w, x;
	vector<string> list;
	
	while (getline(input, w, ' '))
		list.push_back(w);
	
	//(size+1)/2 - 1 ��° �迭�� ����
	int target = (list.size() + 1) / 2 - 1;
	stringstream tmp(list[target]);
	getline(tmp, list[target]);
	getline(tmp, w); list.push_back(w);
	
	list.insert(list.begin() + target + 1, list[list.size() - 1]);
	list.pop_back();
	//list[0~target]���� w, ������ x ���� �Ϸ�

	BigInteger res;
	BigInteger a, b, c;
	for (int i = 0; i <= target; i++) {
		a.setint(stoi(list[i]));
		b.setint(stoi(list[i + target + 1]));
		a.multiply(&c, &a, &b);
		res.add(&c);
	}

	cout << "BigIntegerTest2 ���� ��� : ";
	res.print();
}