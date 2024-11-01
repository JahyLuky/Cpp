#ifndef __PROGTEST__

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
    CRect(double x, double y, double w, double h)
            : m_X(x), m_Y(y), m_W(w), m_H(h) {}

    friend ostream &operator<<(ostream &os, const CRect &x) {
        return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
    }

    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};

#endif /* __PROGTEST__ */

class Component {
public:
    Component(int id, const CRect &rect)
            : id_(id), rect_(rect), abs_rect_(rect) {}

    //virtual int getID() const = 0;
    virtual int getID() const {
        return id_;
    }

    virtual string getType() const = 0;

    //virtual CRect getRect() const = 0;
    virtual CRect getRect() const {
        return rect_;
    }

    virtual Component *clone() const = 0;

    virtual void changeRel(const CRect &abs) = 0;

    virtual void print(ostream &out, const Component &item, int text) const = 0;

    friend ostream &operator<<(ostream &out, const Component &item);

    virtual ~Component() = default;

protected:
    int id_;
    CRect rect_;
    CRect abs_rect_;
};

class namedComponent : public Component {
public:
    namedComponent(int id, const CRect rect, const string &str)
            : Component(id, rect), str_(str) {}

    virtual string getName() const {
        return str_;
    }

protected:
    string str_;
};

class CWindow : public namedComponent {
public:
    //CWindow() = default;

    CWindow(int id, const string &title, const CRect &absPos)
            : namedComponent(id, absPos, title) {}

    CWindow(const CWindow &other)
            : namedComponent(other) {
        for (const auto &item: other.components_) {
            components_.emplace_back(shared_ptr<Component>(item->clone()));
        }
    }

    CWindow &operator=(const CWindow &other) {
        if (this != &other) {
            components_.clear();
            namedComponent::rect_ = other.rect_;
            namedComponent::str_ = other.str_;
            namedComponent::id_ = other.id_;
            namedComponent::abs_rect_ = other.abs_rect_;
            for (const auto &item: other.components_) {
                components_.emplace_back(shared_ptr<Component>(item->clone()));
            }
        }
        return *this;
    }

    ~CWindow() override = default;

    // add
    CWindow &add(const Component &item) {
        components_.emplace_back(item.clone());
        components_.back()->changeRel(this->rect_);
        return *this;
    }

    int getID() const override {
        return namedComponent::getID();
    }

    string getName() const override {
        return namedComponent::getName();
    }

    string getType() const override {
        string name = "Window";
        return name;
    }

    CRect getRect() const override {
        return namedComponent::getRect();
    }

    CWindow *clone() const override {
        return new CWindow(*this);
    }

    void changeRel(const CRect &abs) override {
        this->rect_.m_X = abs.m_X;
        this->rect_.m_Y = abs.m_Y;
        this->rect_.m_W = abs.m_W;
        this->rect_.m_H = abs.m_H;
    }

    void print(ostream &out, const Component &item, int text) const override {
        out << "[" << namedComponent::getID() << "] "
            << item.getType() << " \""
            << namedComponent::getName() << "\" "
            << namedComponent::getRect() << endl;
        for (const auto &i: components_) {
            out << "+- ";
            if (i != *(components_.end() - 1) && i->getType() == "ComboBox") {
                i->print(out, *i, 2);
            } else if (i == *(components_.end() - 1)) {
                if (i->getType() == "ComboBox") {
                    i->print(out, *i, 1);
                } else {
                    i->print(out, *i, text);
                }
            } else {
                if (i->getType() == "ComboBox") {
                    i->print(out, *i, 1);
                } else {
                    i->print(out, *i, 0);
                }
            }
        }
    }

    // search
    Component *search(int id) {
        for (auto &i: components_) {
            if (i->getID() == id) {
                return i.get();
            }
        }
        return nullptr;
    }


    // setPosition
    void setPosition(const CRect &absPos) {
        this->changeRel(absPos);
        for (const auto &i: components_) {
            i->changeRel(this->rect_);
        }
    }

private:
    vector<shared_ptr<Component>> components_;
};

class CButton : public namedComponent {
public:
    CButton() = default;

    CButton(int id, const CRect &relPos, const string &name)
            : namedComponent(id, relPos, name) {}

    int getID() const override {
        return namedComponent::getID();
    }

    string getType() const override {
        string name = "Button";
        return name;
    }

    CRect getRect() const override {
        return namedComponent::getRect();
    }

    Component *clone() const override {
        return new CButton(*this);
    }

    void changeRel(const CRect &abs) override {
        this->rect_.m_X = abs.m_X + abs.m_W * this->abs_rect_.m_X;
        this->rect_.m_Y = abs.m_Y + abs.m_H * this->abs_rect_.m_Y;
        this->rect_.m_W = abs.m_W * this->abs_rect_.m_W;
        this->rect_.m_H = abs.m_H * this->abs_rect_.m_H;
    }

    string getName() const override {
        return namedComponent::getName();
    }

    void print(ostream &out, const Component &item, int text) const override {
        out << "[" << item.getID() << "] "
            << item.getType()
            << " \"" << namedComponent::getName() << "\" "
            << item.getRect() << endl;
    }
};

class CInput : public namedComponent {
public:
    CInput() = default;

    CInput(int id, const CRect &relPos, const string &value)
            : namedComponent(id, relPos, value) {}

    int getID() const override {
        return namedComponent::getID();
    }

    string getType() const override {
        string name = "Input";
        return name;
    }

    CRect getRect() const override {
        return namedComponent::getRect();
    }

    Component *clone() const override {
        return new CInput(*this);
    }

    void changeRel(const CRect &abs) override {
        this->rect_.m_X = abs.m_X + abs.m_W * this->abs_rect_.m_X;
        this->rect_.m_Y = abs.m_Y + abs.m_H * this->abs_rect_.m_Y;
        this->rect_.m_W = abs.m_W * this->abs_rect_.m_W;
        this->rect_.m_H = abs.m_H * this->abs_rect_.m_H;
    }

    void print(ostream &out, const Component &item, int text) const override {
        out << "[" << item.getID() << "] "
            << item.getType()
            << " \"" << namedComponent::getName() << "\" "
            << item.getRect() << endl;
    }

    string getName() const override {
        return namedComponent::getName();
    }

    // setValue
    Component &setValue(const string &text) {
        namedComponent::str_ = text;
        return *this;
    }

    // getValue
    string getValue() const {
        return namedComponent::str_;
    }
};

class CLabel : public namedComponent {
public:
    CLabel() = default;

    CLabel(int id, const CRect &relPos, const string &label)
            : namedComponent(id, relPos, label) {}

    int getID() const override {
        return namedComponent::getID();
    }

    string getType() const override {
        string name = "Label";
        return name;
    }

    CRect getRect() const override {
        return namedComponent::getRect();
    }

    Component *clone() const override {
        return new CLabel(*this);
    }

    void changeRel(const CRect &abs) override {
        this->rect_.m_X = abs.m_X + abs.m_W * this->abs_rect_.m_X;
        this->rect_.m_Y = abs.m_Y + abs.m_H * this->abs_rect_.m_Y;
        this->rect_.m_W = abs.m_W * this->abs_rect_.m_W;
        this->rect_.m_H = abs.m_H * this->abs_rect_.m_H;
    }

    void print(ostream &out, const Component &item, int text) const override {
        out << "[" << item.getID() << "] "
            << item.getType()
            << " \"" << namedComponent::getName() << "\" "
            << item.getRect() << endl;
    }

    string getName() const override {
        return namedComponent::getName();
    }
};

class CComboBox : public Component {
public:
    CComboBox() = default;

    CComboBox(int id, const CRect &relPos)
            : Component(id, relPos) {}

    int getID() const override {
        return Component::getID();
    }

    string getType() const override {
        string name = "ComboBox";
        return name;
    }

    CRect getRect() const override {
        return Component::getRect();
    }

    void changeRel(const CRect &abs) override {
        this->rect_.m_X = abs.m_X + abs.m_W * this->abs_rect_.m_X;
        this->rect_.m_Y = abs.m_Y + abs.m_H * this->abs_rect_.m_Y;
        this->rect_.m_W = abs.m_W * this->abs_rect_.m_W;
        this->rect_.m_H = abs.m_H * this->abs_rect_.m_H;
    }

    Component *clone() const override {
        return new CComboBox(*this);
    }

    void print(ostream &out, const Component &item, int text) const override {
        out << "[" << item.getID() << "] "
            << item.getType() << " "
            << item.getRect() << endl;
        int count = 0;
        for (const auto &i: combobox_) {
            if (count == selected_) {
                if (text == 1) {
                    out << "   +->" << i << "<" << endl;
                } else if (text == 2) {
                    out << "|  +->" << i << "<" << endl;
                } else {
                    out << "+->" << i << "<" << endl;
                }
            } else {
                if (text == 1) {
                    out << "   +- " << i << endl;
                } else if (text == 2) {
                    out << "|  +- " << i << endl;
                } else {
                    out << "+- " << i << endl;
                }
            }
            count++;
        }
    }

    // add
    CComboBox &add(const string &item) {
        combobox_.emplace_back(item);
        return *this;
    }

    // setSelected
    int setSelected(int index) {
        selected_ = index;
        return selected_;
    }

    // getSelected
    int getSelected() const {
        return selected_;
    }

private:
    vector<string> combobox_;
    int selected_ = 0;
};

// output operators
ostream &operator<<(ostream &out, const Component &item) {
    item.print(out, item, 0);
    //item.print(cout, item, 0);
    return out;
}

#ifndef __PROGTEST__

template<typename _T>
string toString(const _T &x) {
    ostringstream oss;
    oss << x;
    //cout << x;
    return oss.str();
}

int main() {
    cout << sizeof(CButton) << ", " << sizeof(string) << " < "
         << sizeof(CComboBox) << ", " << sizeof(vector<string>) << endl;

    assert (sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    CWindow a(0, "Sample window", CRect(10, 10, 600, 480));

    a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
    a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
    a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
    a.add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest"));

    assert (toString(a) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Button \"Ok\" (70,394,180,48)\n"
            "+- [2] Button \"Cancel\" (370,394,180,48)\n"
            "+- [10] Label \"Username:\" (70,58,120,48)\n"
            "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
            "+- [20] ComboBox (70,154,480,48)\n"
            "   +->Karate<\n"
            "   +- Judo\n"
            "   +- Box\n"
            "   +- Progtest\n");

    CWindow b = a;
    assert (toString(*b.search(20)) ==
            "[20] ComboBox (70,154,480,48)\n"
            "+->Karate<\n"
            "+- Judo\n"
            "+- Box\n"
            "+- Progtest\n");

    assert (dynamic_cast<CComboBox &> ( *b.search(20)).getSelected() == 0);
    dynamic_cast<CComboBox &> ( *b.search(20)).setSelected(3);
    assert (dynamic_cast<CInput &> ( *b.search(11)).getValue() == "chucknorris");
    dynamic_cast<CInput &> ( *b.search(11)).setValue("chucknorris@fit.cvut.cz");
    b.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
    assert (toString(b) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Button \"Ok\" (70,394,180,48)\n"
            "+- [2] Button \"Cancel\" (370,394,180,48)\n"
            "+- [10] Label \"Username:\" (70,58,120,48)\n"
            "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
            "+- [20] ComboBox (70,154,480,48)\n"
            "|  +- Karate\n"
            "|  +- Judo\n"
            "|  +- Box\n"
            "|  +->Progtest<\n"
            "+- [21] ComboBox (70,250,480,48)\n"
            "   +->PA2<\n"
            "   +- OSY\n"
            "   +- Both\n");
    assert (toString(a) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Button \"Ok\" (70,394,180,48)\n"
            "+- [2] Button \"Cancel\" (370,394,180,48)\n"
            "+- [10] Label \"Username:\" (70,58,120,48)\n"
            "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
            "+- [20] ComboBox (70,154,480,48)\n"
            "   +->Karate<\n"
            "   +- Judo\n"
            "   +- Box\n"
            "   +- Progtest\n");
    b.setPosition(CRect(20, 30, 640, 520));
    assert (toString(b) ==
            "[0] Window \"Sample window\" (20,30,640,520)\n"
            "+- [1] Button \"Ok\" (84,446,192,52)\n"
            "+- [2] Button \"Cancel\" (404,446,192,52)\n"
            "+- [10] Label \"Username:\" (84,82,128,52)\n"
            "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
            "+- [20] ComboBox (84,186,512,52)\n"
            "|  +- Karate\n"
            "|  +- Judo\n"
            "|  +- Box\n"
            "|  +->Progtest<\n"
            "+- [21] ComboBox (84,290,512,52)\n"
            "   +->PA2<\n"
            "   +- OSY\n"
            "   +- Both\n");

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */