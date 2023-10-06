#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include "windows.h"

using namespace std;

static string *companies = new string[]{"OOO GAZPROM", "ZAO PATOROCHKA", "RTU MIREA", "NIU NASA", "TESLA", "MAGNIT"};
static string *creator = new string[]{"Ilon Mask", "Mark Zukenberg", "Anton Vlasov", "Nikolay Drozdov",
									  "Dmirtiy Gavrilov", "Evgeniy Krupnov"};

struct SmallBusiness {
	int licenseNumber;
	string name;
	string creator;

	void print() {
		cout << licenseNumber << " " << name << " " << creator << endl;
	}

	SmallBusiness(int licenseNumber, string name, string creator) : licenseNumber(licenseNumber),
																	name(std::move(name)),
																	creator(std::move(creator)) {}
};

struct HashTable {
	int size;
	vector<SmallBusiness *> table;

	int hashValue(int k) {
		return k % size;
	}

	int hashStep(int k) {
		k *= 2654435761;
		int res = (k >> 32) % size;
		if (res < 0)
			res *= -1;
		return res;
	}

	HashTable(int size) {
		this->size = size;
		table = vector<SmallBusiness *>(size);
	}

	HashTable() {
		this->size = 1000000;
		table = vector<SmallBusiness *>(size);
	}

	int searchKey(int licenseNumber) {
		int hashVal = hashValue(licenseNumber);
		int stepSize = hashStep(licenseNumber);
		while (table[hashVal] != nullptr &&
			   table[hashVal]->licenseNumber != licenseNumber) {
			hashVal = hashVal + stepSize;
			hashVal = hashVal % size;
		}
		return hashVal;
	}


	SmallBusiness *search(int licenseNumber) {
		int hashVal = hashValue(licenseNumber);
		int stepSize = hashStep(licenseNumber);

		int initialValue = hashVal;

		while (table[hashVal] == nullptr || table[hashVal]->licenseNumber != licenseNumber) {
			hashVal = hashVal + stepSize;
			hashVal = hashVal % size;
			if (initialValue == hashVal)
				return nullptr;
		}
		return table[hashVal];
	}

	void insert(SmallBusiness *entry) {
		int pos = searchKey(entry->licenseNumber);
		if (table[pos] == nullptr) {
			table[pos] = entry;
		}
	}

	bool deleteBusiness(int licenseNumber) {
		int pos = searchKey(licenseNumber);
		if (table[pos] == nullptr)
			return false;
		table[pos] = nullptr;
		return true;
	}

	void rehash(int increase) {
		vector<SmallBusiness *> previous = table;
		size *= increase;
		table = vector<SmallBusiness *>(size);
		for (int i = 0; i < previous.size(); ++i) {
			if (previous[i] != nullptr)
				insert(previous[i]);
		}
	}

	void display() {
		for (int i = 0; i < size; i++) {
			if (table[i] == nullptr) {
				continue;
			}
			table[i]->print();
		}
	}
};

void generate(HashTable *hashTable) {
	srand(time(NULL));
	for (int i = 0; i < 6; ++i) {
		hashTable->insert(new SmallBusiness(rand() % 100000, companies[rand() % 6], creator[rand() % 6]));
	}
}

int main() {
	SetConsoleOutputCP(CP_UTF8);

	HashTable *hashTable = new HashTable();

	string command = "";
	while (command != "exit") {
		cout
				<< "Введите команду: generate - для генерации нескольких рандомных компаний, \ninsert - для вставки значения, \ndelete - для удаления, \nsearch - для поиска, \nrehash - для рехеширования (необязательно для открытой адресации), \ndisplay - для вывода, \nexit - для выхода"
				<< endl;
		cin >> command;
		cout << "_________________________________________" << endl;
		if (command == "generate") {
			generate(hashTable);
		} else if (command == "insert") {
			cout << "Введите номер компании (число)" << endl;
			int num;
			cin >> num;
			cout << "Введите название компании" << endl;
			string name;
			cin >> name;
			cout << "Введите создателя компании" << endl;
			string creatorName;
			cin >> creatorName;
			hashTable->insert(new SmallBusiness(num, name, creatorName));
			cout << "Запись добавлена" << endl;
		} else if (command == "delete") {
			cout << "Введите номер компании (число)" << endl;
			int num;
			cin >> num;
			if (hashTable->deleteBusiness(num)) {
				cout << "Запись удалена" << endl;
			} else {
				cout << "Запись не найдена" << endl;
			}
		} else if (command == "search") {
			cout << "Введите номер компании (число)" << endl;
			int num;
			cin >> num;
			SmallBusiness *smallBusiness = hashTable->search(num);
			if (smallBusiness != nullptr) {
				cout << "Запись найдена" << endl;
				smallBusiness->print();
			} else {
				cout << "Запись не найдена" << endl;
			}
		} else if (command == "rehash") {
			hashTable->rehash(2);
		} else if (command == "display") {
			hashTable->display();
		} else if (command == "exit") {
			break;
		} else {
			cout << "Неверная комманда" << endl;
		}
		cout << "_________________________________________" << endl;
	}

	hashTable->display();
	return 0;
}