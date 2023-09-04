#include"biginteger.h"
#include"neuralnet.h"

/*
�������� weight�� �����ؾ��ϴ� ���(ù �Է��� 0�� ���),
���� �Է¿� ���̾ ���� weight���� ������ �ʴ´ٰ� �Ǵ��߽��ϴ�.

����, weight�� �������� 1-9���� ��������� �Ҵ��ϴ� �Լ��� ���������,
���������� �Ҵ��ϵ��� �������� ������(ù �Է��� 1�� ���)
���Ͽ� �־��� ���̾ ���� weight������ FeedForward�� �����ϰԵ˴ϴ�.
*/

void main() {
	TestFFAlgorithm("FFtestFile.txt");
}

void TestFFAlgorithm(string path) {
	BigIntNerualNet nn(path);
	nn.InitNN();
	//nn.InitNNTest();
	nn.FeedFoward();
	nn.ShowResult();
	
}

/*
����Ƽ�� ����׽�Ʈ�Լ�
BigIntegerTest1(993020341, 452378235);
BigIntegerTest2("BigIntegerTestFile.txt");
*/