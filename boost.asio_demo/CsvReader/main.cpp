#include <iostream>
#include "CsvReader.h"
#include <boost/shared_ptr.hpp>
using namespace std;

int main() {
	shared_ptr<CsvReader> buy_diamond_ptr = make_shared<CsvReader>();
	string filename = "csv/BuyDiamond.csv";
	buy_diamond_ptr->init(filename);
	
	getchar();
	return 0;
}