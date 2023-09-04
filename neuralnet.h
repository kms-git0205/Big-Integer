#pragma once
#include<vector>
#include<ctime>
void TestFFAlgorithm(string path);

class BigIntNerualNet {
public:
	
	string filename;

	BigIntNerualNet(string path) {
		filename = path;
	}

	vector< vector<BigInteger> > Layer;

	void InitNN();
	void FeedFoward();
	void ShowResult();
	void InitNNTest();


	void allClear();
	void setValue();
	void fixedWeightInit();
	void randomWeightInit();
};