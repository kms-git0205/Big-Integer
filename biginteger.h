#pragma once
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>

using namespace std;

#define MAXINT 999999999

class BigInteger {	//BigInteger Ŭ���� ����
public:
	int int_length;		//pint_data�� �迭 ��� ���� (�ε��� �ִ밪 + 1)
	int * pint_data;	//���� �������� �迭[int], [int], ...

	BigInteger();

	void deletee();
	void resize(BigInteger* num, int size);
	void setint(int int_num);
	void set(BigInteger* pbigint_num2);
	void print();

	inline int add_arr(int int_len, int* pint_num1, int* pint_num2);
	void equal_length(BigInteger* pbigint_num2);
	void add(BigInteger* pbigint_num2);

	void multiply(BigInteger * dest, const BigInteger * num1, const BigInteger * num2);
	void minus(BigInteger *pbigint_num2);
	void divide(BigInteger * dest, const BigInteger * num1, const BigInteger * num2, int flag);
	void modular(BigInteger *dest, BigInteger *num1, BigInteger *num2);
	int compare(const BigInteger& other) const;


	//��Ʈ ����
	BigInteger operator << (int n);
	BigInteger operator >> (int n);

	//���� ����
	BigInteger& BigInteger::operator >>=(int n);
	BigInteger& BigInteger::operator <<=(int n);
	BigInteger& BigInteger::operator =(const BigInteger& other);
	
private:
	
};

//�񱳿���
bool operator>(const BigInteger& ll, const BigInteger& rr);
bool operator==(const BigInteger& lhs, const BigInteger& rhs);

//����׽�Ʈ �Լ�
void BigIntegerTest1(int op1, int op2);
void BigIntegerTest2(string testfile);