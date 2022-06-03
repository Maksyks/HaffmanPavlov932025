#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;

class Node 
{
public:
	int n;//число
	char s;//символ
	Node* left, * right;

	Node() { left = right = NULL; }
	//для создания дерева
	Node(Node* L, Node* R)
	{//родителю присваиваем левый и правый указатель
		//числа складываем
		left = L;
		right = R;
		n = L->n + R->n;
	}
};

//функция сортирует по частотам указатели
struct ListSort
{
	bool operator()(const Node* l, const Node* r) const
	{
		return l->n < r->n;
	}
};

//Вектор содержит двоичный код элемента
vector<bool> code;
//мапа ассоциирует символ с кодом(вектором)
map<char, vector<bool> > symbol;

//функция присваивает символам их коды
void BuildTable(Node* root, map<char, vector<bool>>& symbol)
{
	/*идем от корня дерева, если слева есть элемент
	то выводим 0 в вектор*/
	if (root->left)
	{ //для левого узла запускаем рекурсию
		code.push_back(0);
		BuildTable(root->left, symbol);
	}//теперь смотрим правую ветку
	if (root->right) 
	{
		code.push_back(1);
		BuildTable(root->right, symbol);
	}
	/*если правая и левая ветки не имеют потомка, то выводим символ
	ассоциируюя с кодом с помощью мапы*/
	if (((root->right) == NULL) && ((root->left) == NULL))
	{
		symbol[root->s] = code;
	}
	/*убираем крайний справа символ(0 1) и продолжаем обход
	так как возвращаемся назад по дереву*/
	if (!code.empty()) code.pop_back();
}

//Функция выводит коды
void print_vector(vector<bool>code) {
	for (auto it = code.begin();it != code.end();it++) {
		cout << *it;
	}
	cout << endl;
}
//Функция выводи символ-код
void print_table(map<char, vector<bool>>& buff) {
	for (auto it = buff.begin(); it != buff.end(); it++)
	{
		cout << it->first << " - ";
		print_vector(it->second);
	}
}

//Функция выводит коэф сжатия
void compressValue(const char* input_text = "Text.txt", const char* output_text = "Cipher.txt")
{
	long long file_size = 0;
	long long compress_size = 0;

	struct stat s1 {};
	struct stat s2 {};

	if (!stat(input_text, &s1)) {
		file_size = s1.st_size;
	}
	else {
		perror("STAT ERROR ");
	}
	if (!stat(output_text, &s2)) {
		compress_size = s2.st_size;
	}
	else {
		perror("STAT ERROR ");
	}

	cout << "\nCompress value is:" << (compress_size + 0.0) / file_size << "\n";
}

int main()
{
	/*открываем файл*/
	ifstream Text("Text.txt", ios::out );
	if (!Text.is_open()) { cout << "error"; return 0; }
	map<char, int> m;
	//считываем символа пока не конец
	//считаем частоты
	unsigned char lett;
	while (!Text.eof())
	{
		lett = Text.get();
		if (!Text.eof()) 
		{
			m[lett]++;
		}
	}

	//создаю список указателей
	list<Node*> tmp;
	//используя интератор создаю узлы по мапу
	for (map<char, int>::iterator itr = m.begin(); itr != m.end(); ++itr)
	{
		Node* p = new Node;//новый узел
		p->s = itr->first;//символ
		p->n = itr->second;//его частота
		tmp.push_back(p);//указатель на узел
	}

	/*создаем дерево пока не останется 1 символ в списке*/
	while (tmp.size() != 1)
	{// сортируем по частотам список
		tmp.sort(ListSort());
		//записываем в указатель левый элемент
		Node* L = tmp.front();
		tmp.pop_front();//удаляем
		Node* R = tmp.front();
		tmp.pop_front();
		//новый узел с потомками левый правый
		Node* parent = new Node(L, R);
		//заносим новый узел в список
		tmp.push_back(parent);
	}
	//заносим корень дерева
	Node* root = tmp.front();   
	BuildTable(root, symbol);
	
	// перемещаем указатель снова в начало файла
	Text.clear(); Text.seekg(0); 

	ofstream Cipher("Cipher.txt", ios::out | ios::binary);
	if (!Cipher.is_open()) { cout << "error"; return 0; }
	//кол во уникальных символов
	char count_letters = m.size();
	Cipher.put(count_letters);
	
	//выводим шапку в зашифрованный файл
	for (map<char, int>::iterator itr = m.begin(); itr != m.end(); ++itr)
	{
		Cipher.put(itr->first);//символ
		Cipher.put(itr->second);//его частота
	}

	int count = 0; char buf = 0;
	Text.clear(); Text.seekg(0);
	//Вывожу коды символов по 8
	while (!Text.eof())
	{
		char c = Text.get();
		//берем код символа
		vector<bool> x = symbol[c];
		for (int n = 0; n < x.size(); n++)
		{//зансим по 8
			buf = buf | x[n] << (7 - count);
			count++;
			if (count == 8) { count = 0;   Cipher << buf; buf = 0; }
		}
	}
	if (buf) Cipher<<(buf);

	Cipher.seekp(0);
	compressValue();	
	print_table(symbol);
	Cipher.close();
	Text.close();
	return 0;
}
