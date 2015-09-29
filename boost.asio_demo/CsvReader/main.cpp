#include <iostream>
#include "TableManager.h"
using namespace std;

int main() {
	shared_ptr<TableManager> ptr_table = make_shared<TableManager>();
	ptr_table->init("BuyDiamond.csv");

	auto ptr = ptr_table->getManager("BuyDiamond.csv");

	auto value = ptr->getItemDataInt(2, "Diamond");

	cout << value << endl;

	getchar();
	return 0;
}