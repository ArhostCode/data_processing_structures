#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include "windows.h"
#include "map"

using namespace std;

struct cell {
	char ch;
	int freq;
	cell *left, *right;

	cell(char ch, int freq, cell *left, cell *right) : ch(ch), freq(freq), left(left), right(right) {}
};


struct comp {
	bool operator()(cell *l, cell *r) {
		return l->freq > r->freq;
	}
};


void encode(cell *root, string str, unordered_map<char, string> &huffmanCode) {
	if (root == nullptr)
		return;

	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}


string huffmanAlgorithm(string text, unordered_map<char, string> &codesHuffman) {
	unordered_map<char, int> frequency;
	for (char ch: text) {
		frequency[ch]++;
	}

	priority_queue<cell *, vector<cell *>, comp> tree;

	for (auto pair: frequency) {
		tree.push(new cell(pair.first, pair.second, nullptr, nullptr));
	}
	while (tree.size() != 1) {
		cell *left = tree.top();
		tree.pop();
		cell *right = tree.top();
		tree.pop();
		int sum = left->freq + right->freq;
		tree.push(new cell('\0', sum, left, right));
	}

	cell *root = tree.top();

	encode(root, "", codesHuffman);

	cout << "Символы по коду Хаффмана:" << endl;
	for (auto code: codesHuffman) {
		cout << code.first << " " << code.second << endl;
	}

	string str = "";
	for (char ch: text) {
		str += codesHuffman[ch];
	}

	return str;
}

char findAtMap(string str, unordered_map<char, string> &codesHuffman) {
	for (auto pair: codesHuffman) {
		if (pair.second == str)
			return pair.first;
	}
	return '\0';
}

string decode(string str, unordered_map<char, string> &codesHuffman) {
	string decoded;
	string currentSequence;
	for (char letter: str) {
		currentSequence += letter;
		char crnt = findAtMap(currentSequence, codesHuffman);
		if (crnt != '\0') {
			decoded += crnt;
			currentSequence = "";
		}
	}
	return decoded;
}


int main() {
	SetConsoleOutputCP(CP_UTF8);
	string text = "Gulyakin Egor Vladimirovich";
	unordered_map<char, string> codesHuffman;
	string encoded = huffmanAlgorithm(text, codesHuffman);
	cout << "Закодированная строка : " << encoded << endl;
	cout << "Pаcкодированная строка: " << decode(encoded, codesHuffman) << endl;

	map<char, int> freq;
	for (char c: text) {
		freq[c] = 0;
	}
	for (char c: text) {
		freq[c] += 1;
	}
	int ravn = 0;
	int ascii = 0;
	int sumCodesLength = 0;
	for (auto pair: codesHuffman) {
		ravn += 6 * freq[pair.first];
		ascii += 7 * pair.second.length() * freq[pair.first];
		sumCodesLength += pair.second.length();
	}

	cout << "Коэфициент Равномерный код / ASCII: " << (double) ravn / ascii << endl;
	cout << "Средняя длина кода: " << sumCodesLength / codesHuffman.size() << endl;
	return 0;
}