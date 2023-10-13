#include <windows.h>
#include "iostream"

using namespace std;

static std::string ch_udia_hor = "\\", ch_ddia_hor = "/", ch_ver_spa = "| ";


struct node {
	int key;
	int depth;
	node *left;
	node *right;

	node(int k) {
		key = k;
		left = right = 0;
		depth = 1;
	}
};

struct Tree {

	node *mainNode = nullptr;

	Tree(int k) {
		mainNode = new node(k);
	}

	int depth(node *p) {
		return p ? p->depth : 0;
	}

	int bfactor(node *p) {
		return depth(p->right) - depth(p->left);
	}

	void fixDepth(node *p) {
		int hl = depth(p->left);
		int hr = depth(p->right);
		p->depth = (hl > hr ? hl : hr) + 1;
	}

	node *rotateRight(node *p) {
		node *q = p->left;
		p->left = q->right;
		q->right = p;
		fixDepth(p);
		fixDepth(q);
		return q;
	}

	node *rotateLeft(node *q) {
		node *p = q->right;
		q->right = p->left;
		p->left = q;
		fixDepth(q);
		fixDepth(p);
		return p;
	}

	node *balance(node *p) {
		fixDepth(p);
		if (bfactor(p) == 2) {
			if (bfactor(p->right) < 0)
				p->right = rotateRight(p->right);
			return rotateLeft(p);
		}
		if (bfactor(p) == -2) {
			if (bfactor(p->left) > 0)
				p->left = rotateLeft(p->left);
			return rotateRight(p);
		}
		return p;
	}

	node *insert(node *p, int k) {
		if (!p) return new node(k);
		if (k < p->key)
			p->left = insert(p->left, k);
		else
			p->right = insert(p->right, k);
		return balance(p);
	}


	node *insert(int k) {
		mainNode = insert(mainNode, k);
		return mainNode;
	}

	void print(node const *node, std::string const &rpref = "", std::string const &cpref = "",
			   std::string const &lpref = "") {
		if (!node) return;
		if (node->right)
			print(node->right, rpref + "  ", rpref + ch_ddia_hor, rpref + ch_ver_spa);
		std::cout << cpref << std::to_string(node->key) << std::endl;
		if (node->left)
			print(node->left, lpref + ch_ver_spa, lpref + ch_udia_hor, lpref + "  ");
	}

	void postOrder() {
		postOrder(mainNode);
	}

	void middleOrder() {
		middleOrder(mainNode);
	}

	void postOrder(node *node) {
		if (node->left)
			postOrder(node->left);
		if (node->right)
			postOrder(node->right);
		cout << node->key << " ";
	}

	void middleOrder(node *node) {
		if (node->left)
			middleOrder(node->left);
		cout << node->key << " ";
		if (node->right)
			middleOrder(node->right);
	}

	int listSum() {
		return listSum(mainNode);
	}

	int listSum(node *node) {
		if (!node->left && !node->right)
			return node->key;
		int sum = 0;
		if (node->left)
			sum += listSum(node->left);
		if (node->right)
			sum += listSum(node->right);
		return sum;
	}

	int height(node *node) {
		if (!node->left && !node->right)
			return 0;
		int h1 = 0;
		if (node->left)
			h1 = height(node->left);
		if (node->right)
			h1 = max(height(node->right), h1);
		return h1 + 1;
	}

	int height() {
		return height(mainNode);
	}

};

Tree *generate() {
	Tree *tree = new Tree(5);
	tree->insert(6);
	tree->insert(9);
	tree->insert(1);
	tree->insert(3);
	tree->insert(-5);
	tree->insert(44);
	tree->insert(64);
	tree->insert(100);
	tree->insert(150);
	tree->insert(250);
	tree->insert(350);
	tree->insert(450);
	tree->insert(550);
	tree->insert(16);
	return tree;
}

int main() {

	Tree *tree = generate();
	SetConsoleOutputCP(CP_UTF8);
	string choice;
	while (choice != "exit") {
		cout
				<< "Выберите задание. generate для создания своего дерева, print - для вывода дерева, back - обратный ход, middle - симметричный ход, sum - сумма листьев, height - высота дерева"
				<< endl;
		getline(cin, choice);
		if (choice == "back") {
			cout << "Обратный обход - ";
			tree->postOrder();
			cout << endl;
		}
		if (choice == "middle") {
			cout << "Симметричный обход - ";
			tree->middleOrder();
			cout << endl;
		}
		if (choice == "sum") {
			cout << "Сумма листьев: " << tree->listSum() << endl;
		}
		if (choice == "height") {
			cout << "Высота дерева: " << tree->height() << endl;
		}
		if (choice == "print") {
			tree->print(tree->mainNode);
		}
		if (choice == "generate") {
			cout << "Введите значенияя дерева заканчивая 0" << endl;
			int a;
			cin >> a;
			tree = new Tree(a);
			while (cin >> a && a != 0) {
				tree->insert(a);
			}
		}
	}
	return 0;
}