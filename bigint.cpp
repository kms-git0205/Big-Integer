#include"biginteger.h"

using namespace std;

BigInteger::BigInteger() {	//BigInteger 생성
	int_length = 1;
	pint_data = (int*)malloc(sizeof(int));
	pint_data[0] = 0;
}

void BigInteger::deletee() {
	free(pint_data);
}

void BigInteger::resize(BigInteger* num, int int_size) { //num의 data길이 재설정, 더 커졌을 경우에는 커진 부분 0으로 초기화

	int tmp = num->int_length;
	num->int_length = int_size;

	int *tmpnum = num->pint_data;	//시작주소 저장

	num->pint_data = (int*)realloc(num->pint_data, int_size * sizeof(int));

	
	for (int i = tmp; i < int_size; i++) num->pint_data[i] = 0;
}

void BigInteger::setint(int int_num) { //bigint에 int값 대입
	int count = 0;
	while (int_num > MAXINT) {
		int_num -= (MAXINT + 1);
		count++;
	}

	if (count != 0) {
		resize(this, 2);
		pint_data[1] = count;
	}
	else resize(this, 1);
	pint_data[0] = int_num;
}

void BigInteger::set(BigInteger* pbigint_num2) {
	//num2를 현재 BigInteger에 대입
	resize(this, pbigint_num2->int_length);
	for (int i = 0; i < pbigint_num2->int_length; i++) {
		pint_data[i] = pbigint_num2->pint_data[i];
	}
}


inline int BigInteger::add_arr(int int_len, int* pint_num1, int* pint_num2) {
	//data[len-1]까지 num1에 num2를 더해줌(bigint의 두 data를 더할때 사용)
	//num[0]부터 num[len-1]까지 각각 서로 더해줌, 마지막에 carry 리턴

	int int_carry = 0;
	//num1[i], num2[i]를 더하고 나서 num2[i]가 더 크면 캐리발생한거
	for (int i = 0; i < int_len; i++) {
		pint_num1[i] += pint_num2[i];
		if (int_carry) {
			if (pint_num1[i] < MAXINT) int_carry = 0;
			else pint_num1[i] -= (MAXINT + 1);	//이 때 carry는 그대로 1
			++pint_num1[i];
		}
		else {
			if (pint_num1[i] > MAXINT) {
				pint_num1[i] -= (MAXINT + 1);
				int_carry = 1;
			}
		}
	}
	return int_carry;
}


void BigInteger::equal_length(BigInteger* pbigint_num2) {
	//num2가 길 때 해당 데이터의 길이를 num2길이까지 늘림
	if (this->int_length < pbigint_num2->int_length) 	//num2가 길 때
		resize(this, pbigint_num2->int_length);

}

void BigInteger::add(BigInteger* pbigint_num2) {
	//num2가 더해짐
	//add_arr(len, pnum1, pnum2), add_int(len, pnum1, num2),
	//alloc_dataspace(pnum1, size)

	equal_length(pbigint_num2);

	if (add_arr(pbigint_num2->int_length, pint_data, pbigint_num2->pint_data)) {
		//마지막에 캐리 발생
		if (this->int_length == pbigint_num2->int_length) {
			//마지막 자리에서 캐리 발생
			resize(this, this->int_length + 1);
			this->pint_data[this->int_length - 1] += 1;
		}
		else {//마지막 자리가 아닌 곳에서 캐리 발생
			BigInteger a;	//캐리가 발생한 곳의 윗자리에 1 더하는 과정
			resize(&a, pbigint_num2->int_length + 1);
			a.pint_data[a.int_length - 1] = 1;
			this->add(&a);
		}
	}
}

void BigInteger::print() {	//해당 bigint를 출력

	if (int_length == 1) {
		printf("%d\n", pint_data[0]);
		return;
	}

	int i = int_length - 1;

	printf("%d", pint_data[i]); i--;
	for (i; i >= 0; i--) {
		printf("%09d", pint_data[i]);
	}
	printf("\n");
}

//////////////////////////////////////////////////////

int BigInteger::compare(const BigInteger& other) const {
	//크기비교 / other보다 더 크면 1, 작으면 -1, 같으면 0 반환
	if (int_length > other.int_length) return 1;
	else if (int_length < other.int_length) return -1;

	//길이가 같은 같은 경우
	for (int i = int_length - 1; i >= 0; i--) {
		if (pint_data[i] > other.pint_data[i]) return 1;
		else if (pint_data[i] < other.pint_data[i]) return -1;
	}
	return 0;
}


bool operator>(const BigInteger& ll, const BigInteger& rr) {
	return ll.compare(rr) == 1;
}


BigInteger BigInteger::operator << (int n)
{	//n에 어떤 수가 오든 1번만 시프트

	for (int i = int_length - 1; i >= 0; i--) {
		//위에서 부터 내려옴 -> 캐리가 계산결과에 영향을 안미치도록

		pint_data[i] <<= 1;
		if (pint_data[i] > MAXINT) {	//캐리 발생
			pint_data[i] -= (MAXINT + 1);

			if (i == int_length - 1) {	//가장 윗자리에서 캐리 발생
				resize(this, int_length + 1);
			}
			pint_data[i + 1] += 1;
		}
	}
	return *this;
}

BigInteger BigInteger::operator >> (int n) {	//n에 어떤 수가 오든 1번만 시프트
	for (int i = int_length - 1; i >= 0; i--) {
		//위에서부터 data를 돌아다님
		if (pint_data[i] == 0) continue;

		if (pint_data[i] & 1) {//홀수인 경우
			if (i != 0) pint_data[i - 1] += (MAXINT + 1);
		}
		pint_data[i] >>= 1;
	}
	//자릿수가 줄어든 경우, resize
	if (pint_data[int_length - 1] == 0 && int_length > 1)
		resize(this, int_length - 1);

	return *this;
}



BigInteger& BigInteger::operator >>=(int n) {
	return *this = (*this) >> n;
}
BigInteger& BigInteger::operator <<=(int n) {
	return *this = (*this) << n;
}


BigInteger& BigInteger::operator =(const BigInteger& other) {//len, data 똑같이 복사
	int_length = other.int_length;
	resize(this, other.int_length);
	for (int i = 0; i < int_length; i++)
		this->pint_data[i] = other.pint_data[i];

	return *this;
}


bool operator==(const BigInteger& lhs, const BigInteger& rhs)
{
	return lhs.compare(rhs) == 0;
}

void BigInteger::multiply(BigInteger * dest, const BigInteger * num1, const BigInteger * num2) {
	//a la russe 알고리즘
	BigInteger a, b, zero, sum;
	a = *num1;
	b = *num2;
	while (a > zero) {
		if (a.pint_data[0] & 1)
			sum.add(&b);

		a >>= 1;
		b <<= 1;
	}

	resize(dest, sum.int_length);
	*dest = sum;
}

void BigInteger::minus(BigInteger * pbigint_num2) {
	//num2가 빼짐, 항상 현재 수보다 작은 수를 빼도록 해야함
	if (*pbigint_num2 > *this) {
		BigInteger zero;
		printf("zero\n");
		*this = zero;
		return;
	}
	for (int i = pbigint_num2->int_length - 1; i >= 0; i--) {
		//윗자리부터

		pint_data[i] -= pbigint_num2->pint_data[i];
		
		if (pint_data[i] < 0) {	//캐리 발생
			pint_data[i] += (MAXINT + 1);	
			pint_data[i + 1] -= 1;
		}
	}
	while (int_length > 1 && pint_data[int_length - 1] == 0)	//자릿수 맞추기
		resize(this, int_length - 1);
}

void BigInteger::divide(BigInteger * dest, const BigInteger * num1, const BigInteger * num2, int flag = 0) {
	//num1 / num2 를 dest에 저장, flag는 신경안써도 됨
	BigInteger a, b, r0, r1, zero;

	if (*num2 > *num1) {	//num2가 더 클 때
		if (flag == 0) 
			*dest = zero;
		else *dest = *num1;
		return;		
	}

	a = *num1; b = *num2;
	BigInteger one; one.setint(1);
	BigInteger cnt;
	while (a > b || a == b) {
		b <<= 1;
		cnt.add(&one);
	}
	b >>= 1;
	r0 = zero;

	if (cnt == zero) r1 = a;
	else {
		while (cnt > one || cnt == one) {
			if (a > b || a == b) {
				//r[0]+= (1 << (cnt - 1));
				cnt.minus(&one);
				BigInteger tmp; tmp.setint(1);
				BigInteger ii;
				for (ii; cnt > ii; ii.add(&one))
					tmp <<= 1;
				r0.add(&tmp);
				cnt.add(&one);

				a.minus(&b);
			}
			b >>= 1;
			cnt.minus(&one);
		}
		r1 = a;
	}


	if (flag == 0) *dest = r0;
	else *dest = r1;
	return;

}

void BigInteger::modular(BigInteger *dest, BigInteger *num1, BigInteger *num2) {
	BigInteger a;
	a.divide(dest, num1, num2, 1);	//뒤에 1 추가 시 dest를 나머지로 설정하도록 되어있음
}

/*
BigInteger 구현할때 main에서 실험했던거

	//덧셈 피보나치로 검증, 뺄샘 length 5인 수 끼리 0될 때까지 계속 빼면서 검증
	//시프트 <<, >> 100번 반복 하면서 검증
	//곱셈 피보나치로 곱해가면서 검증(길이 매우 길어도 안깨지고 정확했음)
	//나눗셈 : 시프트 100번한 값에서 2로 나누어가면서 검증
	//모듈러 : 길이 3정도에서 실험한 결과 잘 됨
	BigInteger a, b, c, d, sum2, sum, zero;
	

*/

	/* 모듈러연산 테스트코드

	BigInteger a, b, c, d, sum2, sum, zero;

	a.setint(1000000000);
	for (int i = 0; i < 100; i++) a <<= 1;
	b.setint(1000000000);
	for (int i = 0; i < 50; i++) b <<= 1;

	a.print();
	d.setint(1);
	b.add(&d);
	b.print();

	a.modular(&c, &a, &b);
	c.print();
	a.modular(&c, &b, &a);
	c.print();
	a.divide(&c, &a, &b, 0);
	c.print();
	a.divide(&c, &b, &a, 0);
	c.print();
	
	*/


/* 나눗셈 테스트코드

BigInteger a, b, c, d, sum2, sum, zero;
a.setint(1000000000);
for (int i = 0; i < 100; i++) a <<= 1;

a.print();
printf("length : %d\n", a.int_length);
b.setint(2);

while (a > zero) {
a.divide(&sum, &a, &b, 0);
a = sum;
sum.print();
}

*/


/* 뺄셈 테스트코드
BigInteger a, b, c, d, sum2, sum, zero;
a.setint(1000000000);
for (int i = 0; i < 42; i++) a <<= 1;
a.print();
printf("length : %d\n", a.int_length);
b.setint(1000000000);
c.setint(1000000000);
a.multiply(&sum, &b, &c);

while (a > zero) {
	a.minus(&sum);
	a.print();
	printf("length : %d\n", a.int_length);
}
*/


/*곱셈 테스트코드
BigInteger a, b, c, d, sum2, sum, zero;
		a.setint(999999999);
	b.setint(999999999);
	a.print();
	b.print();

	a.multiply(&sum, &a, &b);
	sum.print();
	printf("length : %d\n", sum.int_length);

	a.multiply(&sum2, &a, &b);
	sum2.print();
	printf("length : %d\n", sum2.int_length);

	for (int i = 0; i < 10; i++) {
		a.multiply(&sum, &sum2, &a);
		sum.print();
		printf("length : %d\n", sum.int_length);

		a.multiply(&sum2, &sum, &a);
		sum2.print();
		printf("length : %d\n", sum2.int_length);
	}

*/

	/*시프트연산 테스트코드
	BigInteger a, b, c, d, sum2, sum, zero;
	a.setint(100000000);
	b.setint(100000000);
	a.print();
	b.print();

	a.multiply(&sum, &a, &b);
	sum.print();
	a.multiply(&sum2, &sum, &a);
	sum2.print();



	for (int i = 0; i < 100; i++) {
		sum2.print();
		printf("length : %d\n", sum2.int_length);
		sum2 <<= 1;
	}

	for (int i = 0; i < 100; i++) {
		sum2.print();
		printf("length : %d\n", sum2.int_length);
		sum2 >>= 1;
	}

	*/


	/*
	덧셈 테스트코드
	BigInteger a, b, c, d, sum2, sum, zero;

	a.setint(999999999);
	b.setint(999999999);

	for (int i = 0; i < 100; i++) {
		a.add(&b);
		a.print();
		b.add(&a);
		b.print();
	}
	*/