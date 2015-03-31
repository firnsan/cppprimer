#include <map>
#include <set>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;

class QueryResult;
class TextQuery {
public:
	typedef  vector<string>::size_type line_no;
	TextQuery(ifstream&);
	QueryResult query(const string&) const;

private:
	shared_ptr<vector<string>> file_;
	map<string, shared_ptr<set<line_no>>> wm_;
};

class QueryResult {
	friend ostream& print(ostream&, const QueryResult&);
public:
	typedef  vector<string>::size_type line_no;
	QueryResult(string s,
		shared_ptr<set<line_no>> l,
		shared_ptr<vector<string>> f)
		: sought_(s),
		  lines_(l),
		  file_(f) {}
private:
	string sought_;
	shared_ptr<set<line_no>> lines_;
	shared_ptr<vector<string>> file_;
	
};

TextQuery::TextQuery(ifstream &is)
	: file_(new vector<string>) {
	string text;
	while (getline(is, text)) {
		file_->push_back(text);
		int n = file_->size() - 1;
		istringstream line(text);
		string word;
		while (line >> word) {
			auto &lines = wm_[word];
			if (!lines)
				lines.reset(new set<line_no>);
			lines->insert(n);
		}
	}
}

QueryResult TextQuery::query(const string &sought) const {
	static shared_ptr<set<line_no>> nodata(new set<line_no>);

	auto loc = wm_.find(sought);
	if (loc == wm_.end())
		return QueryResult(sought, nodata, file_);
	else
		return QueryResult(sought, loc->second, file_);
}


ostream &print(ostream &os, const QueryResult &qr) {
	os << qr.sought_ << " occurs " << qr.lines_->size() << " times" << endl;
	for (auto num : *qr.lines_)
		os << "\t(line)" << num+1 << ") "
		   << *(qr.file_->begin() + num) << endl;
	return os;
}

int main(int argc, char *argv[])
{
	ifstream infile("test.txt");
    TextQuery tq(infile);
	while (true)	 {
		cout << "enter the needle : ";
		string s;
		if (!(cin >> s) || s=="q")
			break;
		print(cout, tq.query(s)) << endl;
	}
    return 0;
}
