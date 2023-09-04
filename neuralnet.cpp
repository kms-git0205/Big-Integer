#include"biginteger.h"
#include"neuralnet.h"

vector<BigInteger> output;
//output[nodeNum[LayerNum-1]]����

int flag;				//0, 1�Ǵ�(weight ��������)
int LayerNum;
vector<int> nodeNum;	//���̾ ����/ nodeNum[layerNum]
vector<int> input;		//input[nodeNum[0]]
	
vector<int> weight;		// Layer i ~ i+1�� �մ� ���̾ weight��
vector< vector< vector<int> > > weightset;	//[���̾�][i][j] �� weight
//weight�� 1~9�� (�����̵� �������̵� �� ���̹���)


void BigIntNerualNet::allClear() {	//��� ���ͺ��� Ŭ����
	nodeNum.clear();
	weight.clear();
	input.clear();
	output.clear();
	weightset.clear();
}

void BigIntNerualNet::setValue() {	
	//nn�� �ִ� filename�� �Է����� �޾Ƽ� �� �Ű������ ���� ����
	
	fstream infile(filename);
	if (infile.fail()) {
		cout << "������ ã�� �� �����ϴ�." << endl;
		return;
	}
	string n;	//�Է� �޾ư��鼭 ������ ������ �־���
	infile >> n;
	flag = stoi(n);	//0, 1�Ǵ�(weight��������)

	infile >> n;
	LayerNum = stoi(n);	//���̾� ��

	for (int i = 0; i < LayerNum; i++) {		//���̾ ��� �� ����
		infile >> n; nodeNum.push_back(stoi(n));
	}

	if (flag == 1) {//������ ó���� 0�̸� �������� weight�Ҵ� -> weight���� �ȵ��´ٰ� �Ǵ�
		for (int i = 0; i < LayerNum - 1; i++) {	//���̾� ������ weight����
			infile >> n; weight.push_back(stoi(n));
		}
	}

	for (int i = 0; i < nodeNum[0]; i++) {		//1���� input ����
		infile >> n; input.push_back(stoi(n));
	}

	for (int i = 0; i < nodeNum[LayerNum - 1]; i++) {	//����output ��� ����
		BigInteger a; output.push_back(a);
	}
}

void BigIntNerualNet::InitNN() {
	//Layer ������ layer ��, layer���� ��� ���� �°� ����
	allClear();
	setValue();
	
	if (flag == 0) randomWeightInit();
	else if (flag == 1) fixedWeightInit();

	for (int i = 0; i < LayerNum; i++) {	//���� �Է¹޾Ƽ� ������ ��� �Ű�� �������� ����
		vector<BigInteger> aa;
		Layer.push_back(aa);

		for (int j = 0; j < nodeNum[i]; j++) {
			BigInteger a;
			Layer[i].push_back(a);
		}
	}

}

void BigIntNerualNet::fixedWeightInit() {	
	//[���̾�][i][j] �� weight�� 1-9�� ���ʴ�� ����
	//�Ƹ� ���� ���� ���� ������

	for (int i = 0; i < LayerNum - 1; i++) {	//Layer�ݺ�
		vector< vector <int> > a;
		weightset.push_back(a);

		for (int j = 0; j < nodeNum[i]; j++) {
			//�ش� ���� ��� �� ��ŭ �ݺ�
			vector<int> a;
			weightset[i].push_back(a);

			for (int k = 0; k < nodeNum[i + 1]; k++) {
				weightset[i][j].push_back((k + 1) % 9 + 1);
				//weightset[i][j][k] = (k+1) % 9 + 1 �� ��
			}

		}
	}
}


void BigIntNerualNet::randomWeightInit() {
	//[���̾�][i][j] �� weight ������ ����
	srand(time(NULL));
	for (int i = 0; i < LayerNum - 1; i++) {	//Layer�ݺ�
		vector< vector <int> > a;
		weightset.push_back(a);

		for (int j = 0; j < nodeNum[i]; j++) {
			//�ش� ���� ��� �� ��ŭ �ݺ�
			vector<int> a;
			weightset[i].push_back(a);

			for (int k = 0; k < nodeNum[i + 1]; k++) {
				weightset[i][j].push_back(rand() % 9 + 1);
				//weightset[i][j][k] = rand() % 9 + 1 �� ��
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
	//Layer[0] ���� �Ϸ�

	for (int i = 1; i < LayerNum; i++) {	//layer�� �ݺ�
		for (int j = 0; j < nodeNum[i]; j++) {
			//�ش� layer�� ������ �ݺ�
			for (int k = 0; k < nodeNum[i - 1]; k++) {
				//x[i]w[i] ������ ���� �ݺ�(�ش� layer �Ʒ��� ���)
				BigInteger Bweight;
				Bweight.setint(weightset[i - 1][k][j]);	//�Ʒ����� ���� �����ؾ���
				if (flag == 1) Bweight.setint(weight[i - 1]);
				BigInteger res;
				a.multiply(&res, &Layer[i - 1][k], &Bweight);		
				Layer[i][j].add(&res);
				//�Ʒ����� k��° ��� * Bweight ����
			}
		}
	}
	
	for (int i = 0; i < Layer[Layer.size() - 1].size(); i++) {
		//������ ���� ��� ������ŭ �ݺ�
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
			//���� ���̾� w, Hǥ��
			cout << " => w : " << weight[i] << endl;
			cout << "H" << i << " : " << nodeNum[i] << endl;
		}
		cout << " => w : " << weight[0] << endl;
		cout << "Input : " << nodeNum[0] << endl;
	}
	else if (flag == 0) {
		cout << "Output : " << nodeNum[LayerNum - 1] << endl;

		for (int i = LayerNum - 2; i >= 1; i--) {
			//���� ���̾� w, Hǥ��
			cout << " => w : ������" << endl;
			cout << "H" << i << " : " << nodeNum[i] << endl;
		}
		cout << " => w : ������" << endl;
		cout << "Input : " << nodeNum[0] << endl;
	}
}
