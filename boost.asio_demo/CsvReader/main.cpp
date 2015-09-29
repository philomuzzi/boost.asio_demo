#include <iostream>
#include <fstream>
#include <vector>
#include <boost\algorithm\string.hpp>
#include <boost/lexical_cast.hpp> 
#include <map>

using namespace std;

void readLine(string s, vector<string>& header) {
	boost::split(header, s, boost::is_any_of(","), boost::token_compress_on);

	for (size_t i = 0; i < header.size(); ++i) {
		boost::trim(header[i]);
		boost::to_lower(header[i]);
	}
}

int main() {
	string filename = "csv/BuyDiamond.csv";
	ifstream is(filename);
	if (!is) {
		cout << "���ļ�" << filename << "ʧ��" << endl;
	}
	string s;
	vector<string> header;
	if (getline(is, s)) {
		readLine(s, header);
	}

	map<string, int> m_head_map;
	for (size_t i = 0; i < header.size(); ++i) {
		auto a = m_head_map.insert({ header[i], i });
		if (!a.second) {
			cout << "���" << filename << "����ͬ�ı�ͷID: " << header[i] << endl;
		}
	}

	for (auto i : header) {
		cout << i;
	}
	cout << endl;

	// ��������˵����
	getline(is, s);

	map<int, vector<string>> m_body_map;
	while (getline(is, s)) {
		vector<string> body;
		readLine(s, body);

		auto a = m_body_map.insert({boost::lexical_cast<int>(body.front()), body});
		if (!a.second) {
			cout << "���" << filename << "��ID��ͬ����: " << body.front() << endl;
		}
		
		cout << boost::lexical_cast<int>(body.front()) << endl;
		for(auto i : body) {
			cout << i << "\t";
		}
		cout << endl;
	}

	getchar();
	return 0;
}