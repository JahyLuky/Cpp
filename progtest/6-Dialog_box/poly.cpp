#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>

using namespace std;

class CRect {
public:
    int x;
    int y;

    CRect(int a, int b) : x(a), y(b) {}
};

class superParent {
public:
    superParent() = default;

    superParent(int id, const CRect &relPos)
            : id_(id), rect_(relPos) {}

    virtual void changeID(int newID) {
        id_ = newID;
    }

    int getID() const {
        return id_;
    }

private:
    int id_;
    CRect rect_;
};

class Parent : public superParent {
public:
    Parent(int id, const CRect &relPos, const string &str)
            : superParent(id, relPos), str_(str) {}

    void changeName(const string &newName) {
        str_ = newName;
    }

    string getName() const {
        return str_;
    }

private:
    string str_;
};

class Child : public Parent {
public:
    Child(int id, const CRect &relPos, const string &str)
            : Parent(id, relPos, str) {}

    int getID() const {
        return superParent::getID();
    }

private:

};

int main() {
    Child kid(0, CRect(1, 1), "lukas");
    cout << kid.getID() << endl;
    kid.changeID(42);
    cout << kid.getID() << endl;
    cout << kid.getName() << endl;
    kid.changeName("veru");
    cout << kid.getName() << endl;

    return 1;
}