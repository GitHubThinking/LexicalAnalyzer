#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>

using namespace std;

/*
	关键字的标号在
*/
string keywords[22] = { 
	"break","char","const","continue","cout","do","double","else","endl",
	"float","for","if","int","new","pair" "return","sizeof","string","then","void","while","vector" };
int keywordslength = 22;
/*
	常量和标识符的标号
*/
#define _NUM 30
#define _ID 40

/*
	运算符的标号
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
	界限符符的标号
*/
#define LLB 80 //(
#define LRB 81 //)
#define MLB 82 //[
#define MRB 83 //]
#define BLB 84 //{
#define BRB 85 //}
#define COMMA 86 //
#define COLON 87 //:冒号
#define SEMICOCLON 88 //;分号
#define DQMARKS 89 // "
#define NOTE1 90 // //
#define NOTE2_1 91 // /*
#define NOTE2_2 92 // */
#define OUTPUT 93 // <<
#define LEFTKUO 94 // <
#define RIGHTKUO 95 // >
#define NOTE_CONTEXT 200 //注释内容

/*
	若为错误则置-1
*/
#define _ERROR -1


/*判断是否是字母，针对单个字符串的情况下*/
bool IsLetter(char c) {
	if (((c <= 'z') && (c >= 'a')) || ((c <= 'Z') && (c >= 'A')))
		return true;
	else
		return false;

}

/*判断是否为关键字*/
int Iskeyword(string str) {
	/*从1到19号都是关于keywords的，所以选出来了之后就直接确定它们的标号*/
	for (int i = 0; i < keywordslength; i++) {
		if (str == keywords[i])
			return i;
	}
	return -1;
}

/*判断是否是常量,有字符串也有字符,通过排除法来确定常量*/
bool IsNum(string str) {
	
	int numPoint = 0; 

	/*满足下面条件之一的都不是常量*/
	for (int i = 0; i < str.size(); i++)
	{
		/*倘若这个位置不是数字的时候，有可能是小数点，有可能是E，倘若都不是，那就返回错误*/
		if (str[i] < '0' || str[i] > '9') {
			
			if (str[i] == '.') {
				//假如是小数点的时候,小数点的个数不允许超过2
				numPoint++;
				if (numPoint > 1) {
					return false;
				}

			}
			else if ( (str[i-1] >= '0'&&str[i-1] <= '9')&& str[i] == 'E' ) {
				//假如是E前面是数字的时候,看看E后面跟的是什么
				if ( i < str.size()-1) {
					//j假如E后面还有东西，出错,（运算符存到了另一个串里面）
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

	int Notes_1 = 0; //标记是否有//的注释;
	int Notes_2 = 0; //标记是否有/**/的注释;
	int Rightflag = 0; //决定>是右括号还是大于号 0 为大于号 1为有括号
	vector< pair<int, string> > tempVector;
	
	for (int i = 0; i < analyseWords.size(); i++)
	{
		/*
			运算符
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
				同理可以做>>的词法分析
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
			/*前面是（或者= 号的话，倘若后面跟着一个数字，则是有符号数*/
			if (( analyseWords[i -1] == "(" || analyseWords[i - 1] == "=") && IsNum(analyseWords[i+1]) ) {

				if (analyseWords[i + 1].find('E')) {
					/*如果是有符号的有效数字*/

					if ((analyseWords[i + 2] == "+" || analyseWords[i + 2] == "-")) {
						//数字E后面跟着+号或者-号并且再跟着一个数字的话，


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
			/*前面是（或者= 号的话，倘若后面跟着一个数字，则是有符号数*/
			if ((analyseWords[i - 1] == "(" || analyseWords[i - 1] == "=") && IsNum(analyseWords[i + 1])) {
				
				if (analyseWords[i + 1].find('E')) {
					/*如果是有符号的有效数字*/

					if ((analyseWords[i + 2] == "+" || analyseWords[i + 2] == "-")) {
						//数字E后面跟着+号或者-号并且再跟着一个数字的话，


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
			/*倘若有注释的//符号，就直接进行处理，知道检索到换行符号\n为止*/
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
			界限符
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
				//此处可以设置&
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
				//此处可以设置|
			}
		}

		/*
			常量
		*/
		else if ( ( analyseWords[i][0] >= '0' && analyseWords[i][0] <= '9' ) || analyseWords[i][0] == '.' ) {
			//假如它是第一个是数字，才允许进入 
			if (IsNum(analyseWords[i])) {
				//假如它是一个数字,或者 数字E 或者.数字 的话
				if (analyseWords[i].find('E') == -1) {
					string temStr(analyseWords[i]);
					pair<int, string> tempair(_NUM, temStr);
					tempVector.push_back(tempair);
				}
				else {
					/*有E的时候，必须保证后面留有2位*/

					if (i < (analyseWords.size() - 2)) {

						if ((analyseWords[i + 1] == "+" || analyseWords[i + 1] == "-")) {
							//数字E后面跟着+号或者-号并且再跟着一个数字的话，

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
			标识符和关键字
		*/
		else if (analyseWords[i][0] == '_' || IsLetter(analyseWords[i][0]) ) {
			/*如果是_或者字母开头，则有可能是标识符，或者关键字*/
			int Index = Iskeyword(analyseWords[i]);
			if ( Index == -1 ) {
				/*如果不在关键字表中，则为标识符*/
				pair<int, string> tempair(_ID, analyseWords[i]);
				tempVector.push_back(tempair);
			}
			else {
				pair<int, string> tempair( Index + 1, analyseWords[i]);
				tempVector.push_back(tempair);
			}
		}
		else if (analyseWords[i] == "\n") { 
			//倘若单独读取到了换行符号,直接跳过
			continue;
		}

		else {
			pair<int, string> tempair(_ERROR, analyseWords[i]);
			tempVector.push_back(tempair);
		}
	}


	return tempVector;


}


//寻找非空格部分,即为字母的开头位置
int getFirstString(string str, int i) {
	while (true) {
		if (str[i] != ' ' && str[i] != '\t') {
			return i;
		}
		i++;
	}
}

string getWord(string str, int i, int& j) {

	string s(" \n+-*/%=()[]{},:;<>&|"); //隔断字符串的集合
	j = str.find_first_of(s, i);  //find_first_of为找到第一个s中的元素的位置
	
	if (j == -1) {  //假如找不到时返归空
		return " ";
	}
	else if (i != j) {  //假如找到了就获取有效字符的最后一位的字符
		j--;
	}

	return str.substr(i, j - i + 1); //从i位置开始找j-i+1个单词

	

}


int main() {

	string filename = "test.txt";
	ifstream infile(filename.c_str());
	if (!infile)
	{
		cerr << "无法打开文件! " << filename.c_str() << endl;
		exit(-1);
	}
	cout << endl;

	char containText[10000];
	infile.getline(containText, 10000, EOF); //将文件的内容全部赋值给containText
	infile.close();

	//输出文件中的内容
	cout << containText << endl;

	//有效字符串是指空格或者换行符号之间的字符串
	int a = 0;//有效字符串的头部
	int b = 0;//有效字符串的尾部后的空格或者换行符号的位置

	vector<string> restore; //用来存放搜寻到的有效字符串。

	while (true) {
		a = getFirstString(containText, a);
		string s = getWord(containText, a, b);

		if (b == -1) {
			//假如最后一个位置为-1的时候,证明已经没有有效字符串可以读取了,故退出循环
			break;
		}

		if (s.compare(" ") && s.compare("\t")) { //相等为0 不等为1  \t就是table键
			//假如是有效字符串,则存进去。
			restore.push_back(s);
		}

		a = b + 1;  //从空格或者换行符号的下一个开始取下一个有效字符串的头,(但不一定下一个位置就是有效字符串头)
	}

	vector< pair<int, string> >  v = analyse(restore);

	cout << "输出结果:从左到右分别为  类号  有效字符   类型" << endl;
	cout << endl;
	for (int i = 0; i < v.size(); i++)
	{
		cout << left << setw(10) << v[i].first << " " << left << setw(15) << v[i].second;
		if (v[i].first > 0 && v[i].first < 30) {
			cout<< left << setw(20) <<"关键字"<< endl;
		}
		else if (v[i].first == 30) {
			cout<< left << setw(20) << "常量" << endl;
		}
		else if (v[i].first == 40) {
			cout<<left << setw(20) << "标识符" << endl;
		}
		else if (v[i].first >=50 && v[i].first <=70 ) {
			cout << left << setw(20) << "运算符" << endl;
		}
		else if (v[i].first >= 80 && v[i].first <= 100) {
			cout << left << setw(20) << "界限符" << endl;
		}
		else if (v[i].first > 100) {
			cout << left << setw(20) << "注释内容！！！" << endl;
		}
		else {
			cout << left << setw(20) << "错误符号！！！" << endl;
		}
		
	}

	system("pause");
	return 0;
}