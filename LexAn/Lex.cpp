#include<iostream>
#include<string>
using namespace std;
#define Key_MAX 5
char ch = ' ';
string key[5] = { "if","then","else","while","do" };
int IsKey(string c)											//判断是否是关键字
{
	int i;
	for (i = 0;i < Key_MAX;i++)
	{
		if (key[i].compare(c) == 0) return 1;
	}
	return 0;
}
int IsLetter(char c)										//判断是否是单词
{
	if (((c <= 'z') && (c >= 'a')) || ((c <= 'Z') && (c >= 'A'))) return 1;
	else return 0;
}
int HexLetter(char c)										//十六进制的符号
{
	if (((c <= 'f') && (c >= 'a')) || ((c <= 'F') && (c >= 'A'))) return 1;
	else return 0;
}
int IsDigit(char c)											//判断是否是自然数
{
	if (c >= '0'&&c <= '9')  return 1;
	else return 0;
}
int Oct2Dec(string temp)									//整数八进制转十进制
{
	int sum = 0;
	for (int i = 1;i < temp.length();i++)
	{
		sum = sum * 8 + temp[i] - '0';
	}
	return sum;
}
float d_Oct2Dec(string temp)								//小数八进制转十进制
{
	float sum1 = 0,sum2=temp[temp.length() - 1] - '0';
	int i;
	sum2 = sum2 / 8;
	for (i = 1;i < temp.length()&&temp[i]!='.';i++)
	{
		sum1 = sum1 * 8 + temp[i] - '0';
	}
	for (int j = temp.length() - 2;j > i;j--)
	{
		sum2=sum2/8+ temp[j] - '0';
	}
	return sum1+sum2;
}
int Hex2Dec(string temp)								//整数十六进制转十进制
{
	int sum = 0;
	for (int i = 2;i < temp.length();i++)
	{
		sum = sum * 16 + temp[i] - '0';
	}
	return sum;
}
float d_Hex2Dec(string temp)							//小数十六进制转十进制
{
	int mid[100];
	for (int ii = 0;ii < temp.length();ii++)
	{
		if (temp[ii] == 'a' || temp[ii] == 'A')
			mid[ii] = 10;
		else if (temp[ii] == 'b' || temp[ii] == 'B')
			mid[ii] = 11;
		else if (temp[ii] == 'c' || temp[ii] == 'C')
			mid[ii] = 12;
		else if (temp[ii] == 'd' || temp[ii] == 'D')
			mid[ii] = 13;
		else if (temp[ii] == 'e' || temp[ii] == 'E')
			mid[ii] = 14;
		else if (temp[ii] == 'f' || temp[ii] == 'F')
			mid[ii] = 15;
		else mid[ii] = temp[ii] - '0';
	}
	float sum1 = 0, sum2 = mid[temp.length()-1];
	int i;
	sum2 = sum2 / 16;
	for (i = 2;i < temp.length() && mid[i] != '.'-'0';i++)
	{
		sum1 = sum1 * 16 + mid[i];
	}
	for (int j = temp.length() - 2;j > i;j--)
	{
		sum2 = sum2 / 16 + mid[j];
	}
	return sum1 + sum2;
}
void scan(FILE* fp)								//扫描函数
{
	string temp = "";	//缓存表
	while ((ch = fgetc(fp)) != EOF)
	{
		temp = "";
		if (ch == ' ' || ch == '\t' || ch == '\n')			//过滤掉空格，换行等转义符号
		{ }
		else if (IsLetter(ch) || ch == '_')					//判断首字母是否是单词
		{
			temp += ch;
			ch = fgetc(fp);
			while (IsLetter(ch) || IsDigit(ch))
			{
				if ((ch <= 'Z') && (ch >= 'A')) ch = ch + 32;
				temp += ch;
				ch = fgetc(fp);
			}
			fseek(fp, -1L, SEEK_CUR);
			if (IsKey(temp)) { cout << temp << "\t__" << endl; }
			else cout << "IDN" << "\t" << temp << endl;
		}
		else if (IsDigit(ch))									//判断首字母是整数，则进入整数的DFA转换
		{
			int flag = 0;
			if (ch == '0')
			{
				temp += ch;
				ch = fgetc(fp);
				if (ch >= '0'&&ch <= '7')						//判断得到八进制
				{
					flag = 1;
					temp += ch;
					ch = fgetc(fp);
					while ((ch >= '0'&&ch <= '7')||ch=='.')
					{
						
						if (ch == '.') flag = 2;
						temp += ch;
						ch = fgetc(fp);
					}
				}
				else if (ch == 'x' || ch == 'X')				//判断得到十六进制
				{
					flag = 3;
					temp += ch;
					ch = fgetc(fp);
					temp += ch;
					ch = fgetc(fp);
					while (IsDigit(ch) || HexLetter(ch)||ch=='.')
					{																//flag变量判断为哪种类型
						if (ch == '.') flag = 4;									//flag=1，INT8  八进制整数
						temp += ch;													//flag=2，REAL8 八进制浮点数
						ch = fgetc(fp);												//flag=3，INT16 十六进制整数
					}																//flag=4, REAL16 十六进制浮点数
				}																	//flag=5，INT10 十进制整数
				else if (ch == ' ' || ch == ',' || ch == ';')						//flag=6， REAL16 十进制浮点数
				{
					cout << "INT10" << "\t" << 0 << endl;
				}
				fseek(fp, -1L, SEEK_CUR);
				if (flag == 1)	cout << "INT8" << "\t" << Oct2Dec(temp) << endl;
				else if (flag == 2) cout << "REAL8" << "\t" << d_Oct2Dec(temp) << endl;
				else if (flag == 3) cout << "INT16" << "\t" << Hex2Dec(temp) << endl;
				else if (flag == 4) cout << "REAL16" << "\t" << d_Hex2Dec(temp) << endl;
			}
			else
			{																			//最后判断得到十进制
				temp += ch;
				ch = fgetc(fp);
				flag = 5;
				while (IsDigit(ch)||ch=='.')
				{
					if (ch == '.') flag = 6;
					temp += ch;
					ch = fgetc(fp);
				}
				fseek(fp, -1L, SEEK_CUR);
				if(flag==5) cout << "INT10" << "\t" << temp << endl;
				else if (flag == 6) cout << "REAL10" << "\t" << temp << endl;
			}
		}
		else switch(ch)											//运算符的判断
		{
			case'+':
			case'-':
			case'*':
			case'=':
			case'>':
			case'<':
			case'(':
			case')':
			case';':
				cout << ch << "\t__" << endl;
				break;
			case'/':														//对“//”型注释进行处理
			{
				ch = fgetc(fp);
				if (ch == '/')
				{
					while (ch != '\n')
					{
						ch = fgetc(fp);
					}
				}
				else if (ch == '*')												//对“*/”型注释进行处理
				{
					ch = fgetc(fp);
					char temp1 = fgetc(fp);
					while (ch != '*' || temp1 != '/')				
					{
						if (ch == EOF)
						{
							cout << "注释出错" << endl;
							exit(0);
						}
						ch = fgetc(fp);
						temp1 = fgetc(fp);
					}
					fseek(fp, 1L, SEEK_CUR);
				}
				else
				{
					cout <<"/\t__" << endl;
					fseek(fp, -1L, SEEK_CUR);
				}
					
			}
				break;

			default: cout << ch << "\t无法识别的字符" << endl;
		}
	}
}
int main()
{
	char in_temp[30];
	FILE *fp;
	cout << "请输入源文件名:";
	for (;;)
	{
		cin >> in_temp;					//此处输入测试文件名
		if ((fp = fopen(in_temp, "r")) != NULL) break;
		else cout << "文件路径错误！请输入源文件名:";

	}
	cout << "\n********************分析如下************************" << endl;
	scan(fp);
	fclose(fp);
	system("pause");
	return 0;
}