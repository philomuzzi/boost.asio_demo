#include "CsvReader.h"
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp> 

using namespace std;

CsvReader::CsvReader()
{
}

CsvReader::~CsvReader()
{
}

void CsvReader::readLine(string s, vector<string>& header) {
	boost::split(header, s, boost::is_any_of(","), boost::token_compress_on);

	for (size_t i = 0; i < header.size(); ++i) {
		boost::trim(header[i]);
		boost::to_lower(header[i]);
	}
}

int CsvReader::getItemDataInt(int lineID, std::string name) {
	auto p = m_header_map.find(name);
	if (p == m_header_map.end()) {
		cout << "��������: " << name;
		return (unsigned int)-1;
	}

	auto p2 = m_body_map.find(lineID);
	if (p2 == m_body_map.end()) {
		cout << "�кŴ���: " << lineID;
		return (unsigned int)-1;
	}

	return boost::lexical_cast<int>(p2->second.at(p->second));
}

std::string CsvReader::getItemDataString(int lineID, std::string name) {
	auto p = m_header_map.find(name);
	if (p == m_header_map.end()) {
		cout << "��������: " << name;
		return "";
	}

	auto p2 = m_body_map.find(lineID);
	if (p2 == m_body_map.end()) {
		cout << "�кŴ���: " << lineID;
		return "";
	}

	return p2->second.at(p->second);
}

void CsvReader::init(const std::string filename) {
	std::ifstream is(filename);
	if (!is) {
		std::cout << "���ļ�" << filename << "ʧ��" << endl;
	}

	string line;
	vector<string> header;
	if (getline(is, line)) {
		readLine(line, header);
		cout << line << endl;
	}

	map<string, int> m_head_map;
	for (size_t i = 0; i < header.size(); ++i) {
		auto a = m_head_map.insert({ header[i], i });
		if (!a.second) {
			cout << "���" << filename << "����ͬ�ı�ͷID: " << header[i] << endl;
		}
	}

	// ��������˵����
	getline(is, line);

	map<int, vector<string>> m_body_map;
	while (getline(is, line)) {
		vector<string> body;
		readLine(line, body);

		auto a = m_body_map.insert({ boost::lexical_cast<int>(body.front()), body });
		if (!a.second) {
			cout << "���" << filename << "��ID��ͬ����: " << body.front() << endl;
		}

		cout << line << endl;
	}
}