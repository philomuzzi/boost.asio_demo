#pragma once
#include <map>
#include "CsvReader.h"

class CsvReader;
class TableManager
{
public:
	TableManager();
	~TableManager();

	std::shared_ptr<CsvReader> getManager(const std::string csv_string);

	void init(const std::string filename);

private:
	std::map<const std::string, std::shared_ptr<CsvReader>> m_tableMap;
};

