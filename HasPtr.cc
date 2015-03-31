#include <iostream>
#include <string>

using namespace std;

class HasPtr
{
public:
    HasPtr(const std::string &s= std::string())
        : ps(new std::string(s)),
          i(0),
          use(new std::size_t(1))
    {
    }
    HasPtr(const HasPtr &p)
        : ps(p.ps),
          i(p.i),
          use(p.use)
    {
        ++*use;
    }

    HasPtr& operator=(const HasPtr&);
    ~HasPtr();

    std::string show()
    {
        return *ps;
    }

private:
    std::string *ps;
    int i;
    std::size_t *use;
};

HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
    ++*rhs.use;
    if (--*use==0) {
        delete ps;
        delete use;
    }
    ps = rhs.ps;
    i = rhs.i;
    use = rhs.use;
    return *this;
}


HasPtr::~HasPtr()
{
    if (--*use==0) {
        delete ps;
        delete use;
    }
}


int main(int argc, char *argv[])
{
    HasPtr h1("fuck");
    HasPtr h2(h1);

    std::cout << "h1: " << h1.show() << endl;
    std::cout << "h2: " << h2.show() << endl;
    return 0;
}
