#include"biginteger.h"
#include"neuralnet.h"

vector<BigInteger> output;
//output[nodeNum[LayerNum-1]]형태

int flag;				//0, 1판단(weight 고정여부)
int LayerNum;
vector<int> nodeNum;	//레이어별 노드수/ nodeNum[layerNum]
vector<int> input;		//input[nodeNum[0]]
	
vector<int> weight;		// Layer i ~ i+1을 잇는 레이어별 weight값
vector< vector< vector<int> > > weightset;	//[레이어][i][j] 의 weight
//weight은 1~9로 (랜덤이든 고정값이든 이 사이범위)


void BigIntNerualNet::allClear() {	//모든 벡터변수 클리어
	nodeNum.clear();
	weight.clear();
	input.clear();
	output.clear();
	weightset.clear();
}

void BigIntNerualNet::setValue() {	
	//nn에 있는 filename을 입력으로 받아서 각 신경망설정 변수 세팅
	
	fstream infile(filename);
	if (infile.fail()) {
		cout << "파일을 찾을 수 없습니다." << endl;
		return;
	}
	string n;	//입력 받아가면서 적절한 변수에 넣어줌
	infile >> n;
	flag = stoi(n);	//0, 1판단(weight고정여부)

	infile >> n;
	LayerNum = stoi(n);	//레이어 수

	for (int i = 0; i < LayerNum; i++) {		//레이어별 노드 수 설정
		infile >> n; nodeNum.push_back(stoi(n));
	}

	if (flag == 1) {//파일의 처음이 0이면 랜덤으로 weight할당 -> weight값이 안들어온다고 판단
		for (int i = 0; i < LayerNum - 1; i++) {	//레이어 사이의 weight설정
			infile >> n; weight.push_back(stoi(n));
		}
	}

	for (int i = 0; i < nodeNum[0]; i++) {		//1층의 input 설정
		infile >> n; input.push_back(stoi(n));
	}

	for (int i = 0; i < nodeNum[LayerNum - 1]; i++) {	//전역output 노드 생성
		BigInteger a; output.push_back(a);
	}
}

void BigIntNerualNet::InitNN() {
	//Layer 노드들을 layer 수, layer마다 노드 수에 맞게 생성
	allClear();
	setValue();
	
	if (flag == 0) randomWeightInit();
	else if (flag == 1) fixedWeightInit();

	for (int i = 0; i < LayerNum; i++) {	//파일 입력받아서 설정한 대로 신경망 토폴로지 생성
		vector<BigInteger> aa;
		Layer.push_back(aa);

		for (int j = 0; j < nodeNum[i]; j++) {
			BigInteger a;
			Layer[i].push_back(a);
		}
	}

}

void BigIntNerualNet::fixedWeightInit() {	
	//[레이어][i][j] 의 weight를 1-9로 차례대로 고정
	//아마 쓰일 일은 거의 없을듯

	for (int i = 0; i < LayerNum - 1; i++) {	//Layer반복
		vector< vector <int> > a;
		weightset.push_back(a);

		for (int j = 0; j < nodeNum[i]; j++) {
			//해당 층의 노드 수 만큼 반복
			vector<int> a;
			weightset[i].push_back(a);

			for (int k = 0; k < nodeNum[i + 1]; k++) {
				weightset[i][j].push_back((k + 1) % 9 + 1);
				//weightset[i][j][k] = (k+1) % 9 + 1 이 됨
			}

		}
	}
}


void BigIntNerualNet::randomWeightInit() {
	//[레이어][i][j] 의 weight 랜덤값 설정
	srand(time(NULL));
	for (int i = 0; i < LayerNum - 1; i++) {	//Layer반복
		vector< vector <int> > a;
		weightset.push_back(a);

		for (int j = 0; j < nodeNum[i]; j++) {
			//해당 층의 노드 수 만큼 반복
			vector<int> a;
			weightset[i].push_back(a);

			for (int k = 0; k < nodeNum[i + 1]; k++) {
				weightset[i][j].push_back(rand() % 9 + 1);
				//weightset[i][j][k] = rand() % 9 + 1 이 됨
			}

		}
	}
}

void BigIntNerualNet::FeedFoward() {
	
	BigInteger a;
	for (int i = 0; i < nodeNum[0]; i++) {
		a.setint(input[i]);
		Layer[0][i] = a;
	}
	//Layer[0] 세팅 완료

	for (int i = 1; i < LayerNum; i++) {	//layer를 반복
		for (int j = 0; j < nodeNum[i]; j++) {
			//해당 layer의 노드들을 반복
			for (int k = 0; k < nodeNum[i - 1]; k++) {
				//x[i]w[i] 연산을 위한 반복(해당 layer 아래층 노드)
				BigInteger Bweight;
				Bweight.setint(weightset[i - 1][k][j]);	//아래층의 노드랑 연산해야함
				if (flag == 1) Bweight.setint(weight[i - 1]);
				BigInteger res;
				a.multiply(&res, &Layer[i - 1][k], &Bweight);		
				Layer[i][j].add(&res);
				//아래층의 k번째 노드 * Bweight 저장
			}
		}
	}
	
	for (int i = 0; i < Layer[Layer.size() - 1].size(); i++) {
		//마지막 층의 노드 개수만큼 반복
		output[i] = Layer[Layer.size() - 1][i];
	}

}

void BigIntNerualNet::ShowResult() {
	for (int i = 0; i < nodeNum[LayerNum - 1]; i++) {
		cout << "OutNode" << i + 1 << " : ";
		output[i].print();
	}
}

void BigIntNerualNet::InitNNTest() {

	if (flag == 1) {
		cout << "Output : " << nodeNum[LayerNum - 1] << endl;

		for (int i = LayerNum - 2; i >= 1; i--) {
			//히든 레이어 w, H표시
			cout << " => w : " << weight[i] << endl;
			cout << "H" << i << " : " << nodeNum[i] << endl;
		}
		cout << " => w : " << weight[0] << endl;
		cout << "Input : " << nodeNum[0] << endl;
	}
	else if (flag == 0) {
		cout << "Output : " << nodeNum[LayerNum - 1] << endl;

		for (int i = LayerNum - 2; i >= 1; i--) {
			//히든 레이어 w, H표시
			cout << " => w : 랜덤값" << endl;
			cout << "H" << i << " : " << nodeNum[i] << endl;
		}
		cout << " => w : 랜덤값" << endl;
		cout << "Input : " << nodeNum[0] << endl;
	}
}
