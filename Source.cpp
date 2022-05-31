#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;
class Node //создаем структуру с двумя переменными
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
void BuildTable(Node* root)
{/*идем от корня дерева, если слева есть элемент
 то выводим 0 в вектор*/
	if (root->left != NULL)
	{
		code.push_back(0);
		//для левого узла запускаем рекурсию
		BuildTable(root->left);
	}
	//теперь смотрим правую ветку
	if (root->right != NULL)
	{
		code.push_back(1);
		BuildTable(root->right);
	}
	/*если правая и левая ветки не имеют потомка, то выводим символ
	ассоциируюя с кодом с помощью мапы*/
	if (root->left == NULL && root->right == NULL) 
		symbol[root->s] = code;

	//убираем крайний справа символ(0 1) и продолжаем обход
	code.pop_back();
}

int main()
{
	/*открываем файл*/
	ifstream Text("Text.txt");

	map<char, int> m;
	//считываем символа пока не конец
	//считаем частоты
	while (!Text.eof())
	{//в с записываем переменную
		char c = Text.get();
		//записываем число поторений
		m[c]++;
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
	BuildTable(root);

	
	return 0;
}
