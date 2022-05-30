# HaffmanPavlov932025
#include <stdio.h>
#include <stdlib.h>
struct Node //создаем структуру с двумя переменными
{
	char key;
	int size;
	Node* next = NULL;
	Node* right = NULL;
	Node* left = NULL;
};
//Функция определяет частоты
void Frequence()
{
	FILE* Start = NULL;
	FILE* Result = NULL;
	fopen_s(&Start, "Start.txt", "r");//открываем файл
	unsigned int FrQ[256] = { 0 }; //пустая таблица частот
	unsigned char s = 0;
	//заполняем таблицу частот
	if (Start)
		while (fscanf_s(Start, "%c", &s) != -1)
			FrQ[s]++;

	/*fopen_s(&Result, "Result.txt", "w");//открываем файл
	for (int i = 0;i < 256;i++)
		fprintf(Result, "%d", FrQ[i]);*/

}

int main()
{
	Frequence();
}
