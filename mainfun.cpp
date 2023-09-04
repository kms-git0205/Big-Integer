#include"biginteger.h"
#include"neuralnet.h"

/*
랜덤으로 weight를 생성해야하는 경우(첫 입력이 0인 경우),
파일 입력에 레이어별 고정 weight값이 들어오지 않는다고 판단했습니다.

또한, weight를 고정으로 1-9까지 계속적으로 할당하는 함수를 만들었으나,
고정값으로 할당하도록 설정했을 때에는(첫 입력이 1인 경우)
파일에 주어진 레이어별 고정 weight값으로 FeedForward를 수행하게됩니다.
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
빅인티저 모듈테스트함수
BigIntegerTest1(993020341, 452378235);
BigIntegerTest2("BigIntegerTestFile.txt");
*/