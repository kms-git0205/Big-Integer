#include"biginteger.h"
#include<string>	//string, vector, sstream 모듈테스트 함수에 사용
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
	//첫줄 w 값, 두번째줄 x값
	//첫줄, 두번째줄의 값 개수가 같아야함

	ifstream input(testfile);

	if (!input.is_open()) {
		cout << "파일을 찾을 수 없습니다." << endl;
		return;
	}

	string w, x;
	vector<string> list;
	
	while (getline(input, w, ' '))
		list.push_back(w);
	
	//(size+1)/2 - 1 번째 배열이 문제
	int target = (list.size() + 1) / 2 - 1;
	stringstream tmp(list[target]);
	getline(tmp, list[target]);
	getline(tmp, w); list.push_back(w);
	
	list.insert(list.begin() + target + 1, list[list.size() - 1]);
	list.pop_back();
	//list[0~target]까지 w, 나머지 x 세팅 완료

	BigInteger res;
	BigInteger a, b, c;
	for (int i = 0; i <= target; i++) {
		a.setint(stoi(list[i]));
		b.setint(stoi(list[i + target + 1]));
		a.multiply(&c, &a, &b);
		res.add(&c);
	}

	cout << "BigIntegerTest2 연산 결과 : ";
	res.print();
}