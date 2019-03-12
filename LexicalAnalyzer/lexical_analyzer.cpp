#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>

using namespace std;

/*
	�ؼ��ֵı����
*/
string keywords[22] = { 
	"break","char","const","continue","cout","do","double","else","endl",
	"float","for","if","int","new","pair" "return","sizeof","string","then","void","while","vector" };
int keywordslength = 22;
/*
	�����ͱ�ʶ���ı��
*/
#define _NUM 30
#define _ID 40

/*
	������ı��
*/
#define MOV 50
#define EQUAL 51
#define JA 52
#define JAE 53
#define JB 54
#define JBE 55
#define ADD 56
#define SUB 57
#define MUL 58
#define DIV 59
#define REMAIN 60
#define DAND 61 // &&
#define DOR 62 // ||

/*
	���޷����ı��
*/
#define LLB 80 //(
#define LRB 81 //)
#define MLB 82 //[
#define MRB 83 //]
#define BLB 84 //{
#define BRB 85 //}
#define COMMA 86 //
#define COLON 87 //:ð��
#define SEMICOCLON 88 //;�ֺ�
#define DQMARKS 89 // "
#define NOTE1 90 // //
#define NOTE2_1 91 // /*
#define NOTE2_2 92 // */
#define OUTPUT 93 // <<
#define LEFTKUO 94 // <
#define RIGHTKUO 95 // >
#define NOTE_CONTEXT 200 //ע������

/*
	��Ϊ��������-1
*/
#define _ERROR -1


/*�ж��Ƿ�����ĸ����Ե����ַ����������*/
bool IsLetter(char c) {
	if (((c <= 'z') && (c >= 'a')) || ((c <= 'Z') && (c >= 'A')))
		return true;
	else
		return false;

}

/*�ж��Ƿ�Ϊ�ؼ���*/
int Iskeyword(string str) {
	/*��1��19�Ŷ��ǹ���keywords�ģ�����ѡ������֮���ֱ��ȷ�����ǵı��*/
	for (int i = 0; i < keywordslength; i++) {
		if (str == keywords[i])
			return i;
	}
	return -1;
}

/*�ж��Ƿ��ǳ���,���ַ���Ҳ���ַ�,ͨ���ų�����ȷ������*/
bool IsNum(string str) {
	
	int numPoint = 0; 

	/*������������֮һ�Ķ����ǳ���*/
	for (int i = 0; i < str.size(); i++)
	{
		/*�������λ�ò������ֵ�ʱ���п�����С���㣬�п�����E�����������ǣ��Ǿͷ��ش���*/
		if (str[i] < '0' || str[i] > '9') {
			
			if (str[i] == '.') {
				//������С�����ʱ��,С����ĸ�����������2
				numPoint++;
				if (numPoint > 1) {
					return false;
				}

			}
			else if ( (str[i-1] >= '0'&&str[i-1] <= '9')&& str[i] == 'E' ) {
				//������Eǰ�������ֵ�ʱ��,����E���������ʲô
				if ( i < str.size()-1) {
					//j����E���滹�ж���������,��������浽����һ�������棩
					return false;
				}
				else if(i == str.size() - 1) {
				
					return true;
				}
			}
			else {
				return false;
			}
		}
	}

	return true;

}




vector< pair<int, string> > analyse(vector<string> analyseWords) {

	int Notes_1 = 0; //����Ƿ���//��ע��;
	int Notes_2 = 0; //����Ƿ���/**/��ע��;
	int Rightflag = 0; //����>�������Ż��Ǵ��ں� 0 Ϊ���ں� 1Ϊ������
	vector< pair<int, string> > tempVector;
	
	for (int i = 0; i < analyseWords.size(); i++)
	{
		/*
			�����
		*/
		if ( analyseWords[i] == "=")
		{
			if (analyseWords[i+1] == "=" ) {
				pair<int, string> tempair(EQUAL, "==");
				tempVector.push_back(tempair);
				i++;
			}
			else {
				pair<int, string> tempair(MOV, "=");
				tempVector.push_back(tempair);
			}
			
		}
		else if (analyseWords[i] == ">")
		{
			/*
				ͬ�������>>�Ĵʷ�����
			*/
			if (analyseWords[i + 1] == "=") {
				pair<int, string> tempair(JAE, ">=");
				tempVector.push_back(tempair);
				i++;
			}
			else if (Rightflag == 1) {
				pair<int, string> tempair(RIGHTKUO, ">");
				tempVector.push_back(tempair);
			}
			else {
				pair<int, string> tempair(JA, ">");
				tempVector.push_back(tempair);
			}
			
		}
		else if (analyseWords[i] == "<")
		{
			if (analyseWords[i + 1] == "=") {
				pair<int, string> tempair(JBE, "<=");
				tempVector.push_back(tempair);
				i++;
			}
			else if (analyseWords[i + 1] == "<") {
				pair<int, string> tempair(OUTPUT, "<<");
				tempVector.push_back(tempair);
				i++;
			}
			else if( Iskeyword(analyseWords[i + 1]) ){
				Rightflag = 1;
				pair<int, string> tempair(LEFTKUO, "<");
				tempVector.push_back(tempair);
			}
				
		}
		else if (analyseWords[i] == "+")
		{
			/*ǰ���ǣ�����= �ŵĻ��������������һ�����֣������з�����*/
			if (( analyseWords[i -1] == "(" || analyseWords[i - 1] == "=") && IsNum(analyseWords[i+1]) ) {

				if (analyseWords[i + 1].find('E')) {
					/*������з��ŵ���Ч����*/

					if ((analyseWords[i + 2] == "+" || analyseWords[i + 2] == "-")) {
						//����E�������+�Ż���-�Ų����ٸ���һ�����ֵĻ���


						if (analyseWords[i + 3].find('E') == -1 && IsNum(analyseWords[i + 3]))
						{
							string temStr(analyseWords[i]);
							temStr.append(analyseWords[++i]);
							temStr.append(analyseWords[++i]);
							temStr.append(analyseWords[++i]);
							pair<int, string> tempair(_NUM, temStr);
							tempVector.push_back(tempair);
						}
						else
						{
							string temStr(analyseWords[i]);
							temStr.append(analyseWords[++i]);
							temStr.append(analyseWords[++i]);
							temStr.append(analyseWords[++i]);
							pair<int, string> tempair(_ERROR, temStr);
							tempVector.push_back(tempair);
						}

					}
				}
				else {
					string temStr(analyseWords[i]);
					temStr.append(analyseWords[++i]);
					pair<int, string> tempair(_NUM, temStr);
					tempVector.push_back(tempair);
				}
			}
			else {
				pair<int, string> tempair(ADD, "+");
				tempVector.push_back(tempair);
			}
			
		}
		else if (analyseWords[i] == "-")
		{
			/*ǰ���ǣ�����= �ŵĻ��������������һ�����֣������з�����*/
			if ((analyseWords[i - 1] == "(" || analyseWords[i - 1] == "=") && IsNum(analyseWords[i + 1])) {
				
				if (analyseWords[i + 1].find('E')) {
					/*������з��ŵ���Ч����*/

					if ((analyseWords[i + 2] == "+" || analyseWords[i + 2] == "-")) {
						//����E�������+�Ż���-�Ų����ٸ���һ�����ֵĻ���


						if (analyseWords[i + 3].find('E') == -1 && IsNum(analyseWords[i + 3]))
						{
							string temStr(analyseWords[i]);
							temStr.append(analyseWords[++i]);
							temStr.append(analyseWords[++i]);
							temStr.append(analyseWords[++i]);
							pair<int, string> tempair(_NUM, temStr);
							tempVector.push_back(tempair);
						}
						else
						{
							string temStr(analyseWords[i]);
							temStr.append(analyseWords[++i]);
							temStr.append(analyseWords[++i]);
							temStr.append(analyseWords[++i]);
							pair<int, string> tempair(_ERROR, temStr);
							tempVector.push_back(tempair);
						}

					}
				}
				else {
					string temStr(analyseWords[i]);
					temStr.append(analyseWords[++i]);
					pair<int, string> tempair(_NUM, temStr);
					tempVector.push_back(tempair);
				}
			
			}
			else {
				pair<int, string> tempair(SUB, "+");
				tempVector.push_back(tempair);
			}
			
		}
		else if (analyseWords[i] == "*")
		{
			pair<int, string> tempair(MUL, "*");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == "/")
		{
			/*������ע�͵�//���ţ���ֱ�ӽ��д���֪�����������з���\nΪֹ*/
			if (analyseWords[i + 1] == "/") {
				pair<int, string> tempair(NOTE1, "//"); 
				tempVector.push_back(tempair);
				i = i + 2;
				string temStr_2;
				while (analyseWords[i] != "\n")
				{
					temStr_2.append(analyseWords[i++]);
				}
				pair<int, string> tempair2(NOTE_CONTEXT, temStr_2);
				tempVector.push_back(tempair2);
				continue;
			}
			else if (analyseWords[i + 1] == "*") {
				pair<int, string> tempair(NOTE2_1, "/*");
				tempVector.push_back(tempair);
				i = i + 2;
				string temStr_2;
				while (true)
				{
					if (analyseWords[i] == "*" && analyseWords[i+1] == "/") {
						pair<int, string> tempair2(NOTE_CONTEXT, temStr_2);
						tempVector.push_back(tempair2);
						pair<int, string> tempair3(NOTE2_2, "*/");
						tempVector.push_back(tempair3);
						i = i + 1;
						break;
					}
					else {
						temStr_2.append(analyseWords[i++]);
					}
					
				}
				continue;
			}
			else
			{
				pair<int, string> tempair(DIV, "/");
				tempVector.push_back(tempair);
			}
			
		}
		else if (analyseWords[i] == "%")
		{
			pair<int, string> tempair(REMAIN, "%");
			tempVector.push_back(tempair);
		}

		/*
			���޷�
		*/
		else if (analyseWords[i] == "(")
		{
			pair<int, string> tempair(LLB, "(");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == ")")
		{
			pair<int, string> tempair(LRB, ")");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == "[")
		{
			pair<int, string> tempair(MLB, "[");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == "]")
		{
			pair<int, string> tempair(MRB, "]");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == "{")
		{
			pair<int, string> tempair(BLB, "{");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == "}")
		{
			pair<int, string> tempair(BRB, "}");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == ",")
		{
			pair<int, string> tempair(COMMA, ",");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == ":")
		{
			pair<int, string> tempair(COLON, ":");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == ";")
		{
			pair<int, string> tempair(SEMICOCLON, ";");
			tempVector.push_back(tempair);
		}
		else if (analyseWords[i] == "&")
		{
			if (analyseWords[i + 1] == "&") {
				pair<int, string> tempair(DAND, "&&");
				tempVector.push_back(tempair);
				i++;
			}
			else {
				//�˴���������&
			}
		}
		else if (analyseWords[i] == "|")
		{
			if (analyseWords[i + 1] == "|") {
				pair<int, string> tempair(DOR, "||");
				tempVector.push_back(tempair);
				i++;
			}
			else {
				//�˴���������|
			}
		}

		/*
			����
		*/
		else if ( ( analyseWords[i][0] >= '0' && analyseWords[i][0] <= '9' ) || analyseWords[i][0] == '.' ) {
			//�������ǵ�һ�������֣���������� 
			if (IsNum(analyseWords[i])) {
				//��������һ������,���� ����E ����.���� �Ļ�
				if (analyseWords[i].find('E') == -1) {
					string temStr(analyseWords[i]);
					pair<int, string> tempair(_NUM, temStr);
					tempVector.push_back(tempair);
				}
				else {
					/*��E��ʱ�򣬱��뱣֤��������2λ*/

					if (i < (analyseWords.size() - 2)) {

						if ((analyseWords[i + 1] == "+" || analyseWords[i + 1] == "-")) {
							//����E�������+�Ż���-�Ų����ٸ���һ�����ֵĻ���

							if (analyseWords[i + 2].find('E') == -1 && IsNum(analyseWords[i + 2]))
							{
								string temStr(analyseWords[i]);
								temStr.append(analyseWords[++i]);
								temStr.append(analyseWords[++i]);
								pair<int, string> tempair(_NUM, temStr);
								tempVector.push_back(tempair);
							}
							else
							{
								string temStr(analyseWords[i]);
								temStr.append(analyseWords[++i]);
								temStr.append(analyseWords[++i]);
								pair<int, string> tempair(_ERROR, temStr);
								tempVector.push_back(tempair);
							}

						}
					}

					else if (i == (analyseWords.size() - 2)) {
						string temStr(analyseWords[i]);
						temStr.append(analyseWords[++i]);
						pair<int, string> tempair(_ERROR, temStr);
						tempVector.push_back(tempair);
					}
					else if (i == (analyseWords.size() - 1)) {
						string temStr(analyseWords[i]);
						pair<int, string> tempair(_ERROR, temStr);
						tempVector.push_back(tempair);
					}
				}
			}
			else {
				pair<int, string> tempair(_ERROR, analyseWords[i]);
				tempVector.push_back(tempair);
			}
			
			
				
		}

		/*
			��ʶ���͹ؼ���
		*/
		else if (analyseWords[i][0] == '_' || IsLetter(analyseWords[i][0]) ) {
			/*�����_������ĸ��ͷ�����п����Ǳ�ʶ�������߹ؼ���*/
			int Index = Iskeyword(analyseWords[i]);
			if ( Index == -1 ) {
				/*������ڹؼ��ֱ��У���Ϊ��ʶ��*/
				pair<int, string> tempair(_ID, analyseWords[i]);
				tempVector.push_back(tempair);
			}
			else {
				pair<int, string> tempair( Index + 1, analyseWords[i]);
				tempVector.push_back(tempair);
			}
		}
		else if (analyseWords[i] == "\n") { 
			//����������ȡ���˻��з���,ֱ������
			continue;
		}

		else {
			pair<int, string> tempair(_ERROR, analyseWords[i]);
			tempVector.push_back(tempair);
		}
	}


	return tempVector;


}


//Ѱ�ҷǿո񲿷�,��Ϊ��ĸ�Ŀ�ͷλ��
int getFirstString(string str, int i) {
	while (true) {
		if (str[i] != ' ' && str[i] != '\t') {
			return i;
		}
		i++;
	}
}

string getWord(string str, int i, int& j) {

	string s(" \n+-*/%=()[]{},:;<>&|"); //�����ַ����ļ���
	j = str.find_first_of(s, i);  //find_first_ofΪ�ҵ���һ��s�е�Ԫ�ص�λ��
	
	if (j == -1) {  //�����Ҳ���ʱ�����
		return " ";
	}
	else if (i != j) {  //�����ҵ��˾ͻ�ȡ��Ч�ַ������һλ���ַ�
		j--;
	}

	return str.substr(i, j - i + 1); //��iλ�ÿ�ʼ��j-i+1������

	

}


int main() {

	string filename = "test.txt";
	ifstream infile(filename.c_str());
	if (!infile)
	{
		cerr << "�޷����ļ�! " << filename.c_str() << endl;
		exit(-1);
	}
	cout << endl;

	char containText[10000];
	infile.getline(containText, 10000, EOF); //���ļ�������ȫ����ֵ��containText
	infile.close();

	//����ļ��е�����
	cout << containText << endl;

	//��Ч�ַ�����ָ�ո���߻��з���֮����ַ���
	int a = 0;//��Ч�ַ�����ͷ��
	int b = 0;//��Ч�ַ�����β����Ŀո���߻��з��ŵ�λ��

	vector<string> restore; //���������Ѱ������Ч�ַ�����

	while (true) {
		a = getFirstString(containText, a);
		string s = getWord(containText, a, b);

		if (b == -1) {
			//�������һ��λ��Ϊ-1��ʱ��,֤���Ѿ�û����Ч�ַ������Զ�ȡ��,���˳�ѭ��
			break;
		}

		if (s.compare(" ") && s.compare("\t")) { //���Ϊ0 ����Ϊ1  \t����table��
			//��������Ч�ַ���,����ȥ��
			restore.push_back(s);
		}

		a = b + 1;  //�ӿո���߻��з��ŵ���һ����ʼȡ��һ����Ч�ַ�����ͷ,(����һ����һ��λ�þ�����Ч�ַ���ͷ)
	}

	vector< pair<int, string> >  v = analyse(restore);

	cout << "������:�����ҷֱ�Ϊ  ���  ��Ч�ַ�   ����" << endl;
	cout << endl;
	for (int i = 0; i < v.size(); i++)
	{
		cout << left << setw(10) << v[i].first << " " << left << setw(15) << v[i].second;
		if (v[i].first > 0 && v[i].first < 30) {
			cout<< left << setw(20) <<"�ؼ���"<< endl;
		}
		else if (v[i].first == 30) {
			cout<< left << setw(20) << "����" << endl;
		}
		else if (v[i].first == 40) {
			cout<<left << setw(20) << "��ʶ��" << endl;
		}
		else if (v[i].first >=50 && v[i].first <=70 ) {
			cout << left << setw(20) << "�����" << endl;
		}
		else if (v[i].first >= 80 && v[i].first <= 100) {
			cout << left << setw(20) << "���޷�" << endl;
		}
		else if (v[i].first > 100) {
			cout << left << setw(20) << "ע�����ݣ�����" << endl;
		}
		else {
			cout << left << setw(20) << "������ţ�����" << endl;
		}
		
	}

	system("pause");
	return 0;
}