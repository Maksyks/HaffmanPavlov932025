#include <stdio.h>
#include <stdlib.h>
struct Node //������� ��������� � ����� �����������
{
	char key;
	int size;
	Node* next = NULL;
	Node* right = NULL;
	Node* left = NULL;
};
//������� ���������� �������
void Frequence()
{
	FILE* Start = NULL;
	FILE* Result = NULL;
	fopen_s(&Start, "Start.txt", "r");//��������� ����
	unsigned int FrQ[256] = { 0 }; //������ ������� ������
	unsigned char s = 0;
	//��������� ������� ������
	if (Start)
		while (fscanf_s(Start, "%c", &s) != -1)
			FrQ[s]++;

	/*fopen_s(&Result, "Result.txt", "w");//��������� ����
	for (int i = 0;i < 256;i++)
		fprintf(Result, "%d", FrQ[i]);*/

}

int main()
{
	Frequence();
}