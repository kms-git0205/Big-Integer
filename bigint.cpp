#include"biginteger.h"

using namespace std;

BigInteger::BigInteger() {	//BigInteger ����
	int_length = 1;
	pint_data = (int*)malloc(sizeof(int));
	pint_data[0] = 0;
}

void BigInteger::deletee() {
	free(pint_data);
}

void BigInteger::resize(BigInteger* num, int int_size) { //num�� data���� �缳��, �� Ŀ���� ��쿡�� Ŀ�� �κ� 0���� �ʱ�ȭ

	int tmp = num->int_length;
	num->int_length = int_size;

	int *tmpnum = num->pint_data;	//�����ּ� ����

	num->pint_data = (int*)realloc(num->pint_data, int_size * sizeof(int));

	
	for (int i = tmp; i < int_size; i++) num->pint_data[i] = 0;
}

void BigInteger::setint(int int_num) { //bigint�� int�� ����
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
	//num2�� ���� BigInteger�� ����
	resize(this, pbigint_num2->int_length);
	for (int i = 0; i < pbigint_num2->int_length; i++) {
		pint_data[i] = pbigint_num2->pint_data[i];
	}
}


inline int BigInteger::add_arr(int int_len, int* pint_num1, int* pint_num2) {
	//data[len-1]���� num1�� num2�� ������(bigint�� �� data�� ���Ҷ� ���)
	//num[0]���� num[len-1]���� ���� ���� ������, �������� carry ����

	int int_carry = 0;
	//num1[i], num2[i]�� ���ϰ� ���� num2[i]�� �� ũ�� ĳ���߻��Ѱ�
	for (int i = 0; i < int_len; i++) {
		pint_num1[i] += pint_num2[i];
		if (int_carry) {
			if (pint_num1[i] < MAXINT) int_carry = 0;
			else pint_num1[i] -= (MAXINT + 1);	//�� �� carry�� �״�� 1
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
	//num2�� �� �� �ش� �������� ���̸� num2���̱��� �ø�
	if (this->int_length < pbigint_num2->int_length) 	//num2�� �� ��
		resize(this, pbigint_num2->int_length);

}

void BigInteger::add(BigInteger* pbigint_num2) {
	//num2�� ������
	//add_arr(len, pnum1, pnum2), add_int(len, pnum1, num2),
	//alloc_dataspace(pnum1, size)

	equal_length(pbigint_num2);

	if (add_arr(pbigint_num2->int_length, pint_data, pbigint_num2->pint_data)) {
		//�������� ĳ�� �߻�
		if (this->int_length == pbigint_num2->int_length) {
			//������ �ڸ����� ĳ�� �߻�
			resize(this, this->int_length + 1);
			this->pint_data[this->int_length - 1] += 1;
		}
		else {//������ �ڸ��� �ƴ� ������ ĳ�� �߻�
			BigInteger a;	//ĳ���� �߻��� ���� ���ڸ��� 1 ���ϴ� ����
			resize(&a, pbigint_num2->int_length + 1);
			a.pint_data[a.int_length - 1] = 1;
			this->add(&a);
		}
	}
}

void BigInteger::print() {	//�ش� bigint�� ���

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
	//ũ��� / other���� �� ũ�� 1, ������ -1, ������ 0 ��ȯ
	if (int_length > other.int_length) return 1;
	else if (int_length < other.int_length) return -1;

	//���̰� ���� ���� ���
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
{	//n�� � ���� ���� 1���� ����Ʈ

	for (int i = int_length - 1; i >= 0; i--) {
		//������ ���� ������ -> ĳ���� ������� ������ �ȹ�ġ����

		pint_data[i] <<= 1;
		if (pint_data[i] > MAXINT) {	//ĳ�� �߻�
			pint_data[i] -= (MAXINT + 1);

			if (i == int_length - 1) {	//���� ���ڸ����� ĳ�� �߻�
				resize(this, int_length + 1);
			}
			pint_data[i + 1] += 1;
		}
	}
	return *this;
}

BigInteger BigInteger::operator >> (int n) {	//n�� � ���� ���� 1���� ����Ʈ
	for (int i = int_length - 1; i >= 0; i--) {
		//���������� data�� ���ƴٴ�
		if (pint_data[i] == 0) continue;

		if (pint_data[i] & 1) {//Ȧ���� ���
			if (i != 0) pint_data[i - 1] += (MAXINT + 1);
		}
		pint_data[i] >>= 1;
	}
	//�ڸ����� �پ�� ���, resize
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


BigInteger& BigInteger::operator =(const BigInteger& other) {//len, data �Ȱ��� ����
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
	//a la russe �˰���
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
	//num2�� ����, �׻� ���� ������ ���� ���� ������ �ؾ���
	if (*pbigint_num2 > *this) {
		BigInteger zero;
		printf("zero\n");
		*this = zero;
		return;
	}
	for (int i = pbigint_num2->int_length - 1; i >= 0; i--) {
		//���ڸ�����

		pint_data[i] -= pbigint_num2->pint_data[i];
		
		if (pint_data[i] < 0) {	//ĳ�� �߻�
			pint_data[i] += (MAXINT + 1);	
			pint_data[i + 1] -= 1;
		}
	}
	while (int_length > 1 && pint_data[int_length - 1] == 0)	//�ڸ��� ���߱�
		resize(this, int_length - 1);
}

void BigInteger::divide(BigInteger * dest, const BigInteger * num1, const BigInteger * num2, int flag = 0) {
	//num1 / num2 �� dest�� ����, flag�� �Ű�Ƚᵵ ��
	BigInteger a, b, r0, r1, zero;

	if (*num2 > *num1) {	//num2�� �� Ŭ ��
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
	a.divide(dest, num1, num2, 1);	//�ڿ� 1 �߰� �� dest�� �������� �����ϵ��� �Ǿ�����
}

/*
BigInteger �����Ҷ� main���� �����ߴ���

	//���� �Ǻ���ġ�� ����, ���� length 5�� �� ���� 0�� ������ ��� ���鼭 ����
	//����Ʈ <<, >> 100�� �ݺ� �ϸ鼭 ����
	//���� �Ǻ���ġ�� ���ذ��鼭 ����(���� �ſ� �� �ȱ����� ��Ȯ����)
	//������ : ����Ʈ 100���� ������ 2�� ������鼭 ����
	//��ⷯ : ���� 3�������� ������ ��� �� ��
	BigInteger a, b, c, d, sum2, sum, zero;
	

*/

	/* ��ⷯ���� �׽�Ʈ�ڵ�

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


/* ������ �׽�Ʈ�ڵ�

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


/* ���� �׽�Ʈ�ڵ�
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


/*���� �׽�Ʈ�ڵ�
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

	/*����Ʈ���� �׽�Ʈ�ڵ�
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
	���� �׽�Ʈ�ڵ�
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