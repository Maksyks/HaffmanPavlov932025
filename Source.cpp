#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;
class Node //������� ��������� � ����� �����������
{
public:
	int n;//�����
	char s;//������
	Node* left, * right;

	Node() { left = right = NULL; }
	//��� �������� ������
	Node(Node* L, Node* R)
	{//�������� ����������� ����� � ������ ���������
		//����� ����������
		left = L;
		right = R;
		n = L->n + R->n;
	}
};
//������� ��������� �� �������� ���������
struct ListSort
{
	bool operator()(const Node* l, const Node* r) const
	{
		return l->n < r->n;
	}
};
//������ �������� �������� ��� ��������
vector<bool> code;
//���� ����������� ������ � �����(��������)
map<char, vector<bool> > symbol;
//������� ����������� �������� �� ����
void BuildTable(Node* root, map<char, vector<bool> > symbol)
{
	/*���� �� ����� ������, ���� ����� ���� �������
	�� ������� 0 � ������*/
	if (root->left)
	{ //��� ������ ���� ��������� ��������
		code.push_back(0);
		BuildTable(root->left, symbol);
	}//������ ������� ������ �����
	if (root->right)
	{
		code.push_back(1);
		BuildTable(root->right, symbol);
	}
	/*���� ������ � ����� ����� �� ����� �������, �� ������� ������
	����������� � ����� � ������� ����*/
	if (((root->right) == NULL) && ((root->left) == NULL))
	{
		symbol[root->s] = code;
	}
	/*������� ������� ������ ������(0 1) � ���������� �����
	��� ��� ������������ ����� �� ������*/
	if (!code.empty()) code.pop_back();

}
void print_vector(vector<bool>code) {
	for (auto it = code.begin();it != code.end();it++) {
		cout << *it;
	}
	cout << endl;
}

void print_table(map<char, vector<bool>>& buff) {
	for (auto it = buff.begin(); it != buff.end(); it++)
	{
		cout << it->first << " - ";
		print_vector(it->second);
	}
}


int main()
{
	const char* Cipher = "Cipher.txt";
	const char* PlainText = "PlainText.txt";
	const char* Text = "Text.txt";
	FILE* input = fopen(Cipher, "rb");
	if (!input)
	{
		puts("HuffmanDecoder ERROR: Have not found input text\n");
		exit(1);
	}

	unsigned char key;
	map<char, int> frequency;
	unsigned char i = 0;
	unsigned char j = 0;

	//���-�� ���������� ��������
	if (input != NULL) 
		j = fgetc(input);

	//������ ���� � ��������� �� �����
	for(;i!=j;i++)
	{
		key = fgetc(input);
		frequency[key] = fgetc(input);
	}
	
	//������ ����
	for (auto it = frequency.begin(); it != frequency.end(); it++)
				cout << it->first << " - " << it->second << endl;
		

	//������ ������ ����������
	list<Node*> tmp;
	//��������� ��������� ������ ���� �� ����
	for (map<char, int>::iterator itr = frequency.begin(); itr != frequency.end(); ++itr)
	{
		Node* p = new Node;//����� ����
		p->s = itr->first;//������
		p->n = itr->second;//��� �������
		tmp.push_back(p);//��������� �� ����
	}

	/*������� ������ ���� �� ��������� 1 ������ � ������*/
	while (tmp.size() != 1)
	{// ��������� �� �������� ������
		tmp.sort(ListSort());
		//���������� � ��������� ����� �������
		Node* L = tmp.front();
		tmp.pop_front();//�������
		Node* R = tmp.front();
		tmp.pop_front();
		//����� ���� � ��������� ����� ������
		Node* parent = new Node(L, R);
		//������� ����� ���� � ������
		tmp.push_back(parent);
	}
	//������� ������ ������
	Node* root = tmp.front();
	BuildTable(root, symbol);
	print_table(symbol);

	FILE* output = fopen(PlainText, "wb+");
	if (!output)
	{
		puts("HuffmanDecoder ERROR: Have not found input text\n");
		exit(1);
	}

	Node* p = root;
	unsigned char count = 0;
	char byte;
	byte = fgetc(input);
	
	//���-�� ������ ����� ��� ������������
	unsigned long long len = root->n;
	
	while (byte!=EOF&&len>0)
	{
		//���� �� ������ �����
		bool b = byte & (1 << (7 - count));
		//���� �� ������
		if (b) 
			p = p->right; 
		else p = p->left;
		//������� ����(������)
		if (p->left == NULL && p->right == NULL) 
		{
			fputc(p->s, output);
			p = root;len--;
		}
		count++;
		if (count == 8) 
		{ count = 0; byte = fgetc(input); }
	}
	fseek(output,0, SEEK_SET);
	FILE* check1 = fopen(PlainText, "r");
	FILE* check2 = fopen(Text, "r");
	char ck1, ck2;
	while (!feof(check1) && !feof(check2))
	{
		ck1 = fgetc(check1);
		ck2 = fgetc(check2);
		if (ck1 != ck2)
		{
			puts("Files not equal\n");
			
		}
		
	}
	puts("Files are equal");
	fclose(input);
	fclose(output);
	return 0;
}