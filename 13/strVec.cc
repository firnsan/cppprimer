/**
 * @file   strVec.cc
 * @brief  cpp primer page 465
 *
 * <long description>
 *
 * @author ubuntu
 * @date   2015-03-31
 */

#include <memory>
#include <utility>
#include <string>

using namespace std;

class StrVec {
public:
	StrVec()
		: elements_(nullptr),
		  first_free_(nullptr),
		  cap_(nullptr) {}
	StrVec(const StrVec&);
	StrVec &operator=(const StrVec&);
	~StrVec();
	
	void push_back(const string&);
	size_t size() const {return first_free_ - elements_;}
	size_t capacity() const {return cap_ - elements_;}
	string *begin() const {return elements_;}
	string *end() const {return first_free_;}

private:

	void chk_n_alloc() {
		if (size()==capacity())
			reallocate();
	}
	pair<string*, string*> alloc_n_copy(const string*, const string*);
	void free();
	void reallocate();

	string *elements_;
	string *first_free_;
	string *cap_;
	
	static allocator<string> alloc_;
	
};

void StrVec::push_back(const string &s) {
	chk_n_alloc();
	alloc_.construct(first_free_++, s);
}

pair<string*, string*> StrVec::alloc_n_copy(const string *b, const string *e) {
	auto data = alloc_.allocate(e-b);
	return {data, uninitialized_copy(b, e, data)};
}

void StrVec::free() {
	if (elements_) {
		for (auto p=first_free_; p!=elements_; )
			alloc_.destroy(p--);
		alloc_.deallocate(elements_, cap_-elements_);
	}
}


StrVec::StrVec(const StrVec &s) {
	auto newdata = alloc_n_copy(s.begin(), s.end());
	elements_ = newdata.first;
	first_free_ = cap_ = newdata.second;
}

StrVec::~StrVec() {
	free();
}

StrVec &StrVec::operator=(const StrVec &rhs) {
	auto newdata = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	elements_ = newdata.first;
	first_free_ = cap_ = newdata.second;
	return *this;
}

void StrVec::reallocate() {
	auto newcapacity = size() ? 2*size() : 1;
	auto newdata = alloc_.allocate(newcapacity);

	auto dest = newdata, p=elements_;
	while (p != first_free_) {
		alloc_.construct(dest++, std::move(*p++));
	}
	free();
	elements_ = newdata;
	first_free_ = dest;
	cap_ = newdata + newcapacity;
}