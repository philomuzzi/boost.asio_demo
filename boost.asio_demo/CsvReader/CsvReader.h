#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>

class CsvReader : public std::enable_shared_from_this<CsvReader> {
public:
	CsvReader();
	~CsvReader();

	void init(const std::string filename);

	int getItemDataInt(int lineID, std::string name);
	std::string getItemDataString(int lineID, std::string name);

private:
	void readLine(std::string s, std::vector<std::string>& header);

private:
	std::map<std::string, int> m_header_map;
	std::map<int, std::vector<std::string>> m_body_map;
};
