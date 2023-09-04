#pragma once
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>

using namespace std;

#define MAXINT 999999999

class BigInteger {	//BigInteger 클래스 정의
public:
	int int_length;		//pint_data의 배열 요소 개수 (인덱스 최대값 + 1)
	int * pint_data;	//실제 숫자정보 배열[int], [int], ...

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


	//비트 연산
	BigInteger operator << (int n);
	BigInteger operator >> (int n);

	//대입 연산
	BigInteger& BigInteger::operator >>=(int n);
	BigInteger& BigInteger::operator <<=(int n);
	BigInteger& BigInteger::operator =(const BigInteger& other);
	
private:
	
};

//비교연산
bool operator>(const BigInteger& ll, const BigInteger& rr);
bool operator==(const BigInteger& lhs, const BigInteger& rhs);

//모듈테스트 함수
void BigIntegerTest1(int op1, int op2);
void BigIntegerTest2(string testfile);