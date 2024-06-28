#ifndef __PROGTEST__

#include <cstring>

using namespace std;

class CLinkedSetTester;

#endif /* __PROGTEST__ */

class CLinkedSet {
private:
    struct CNode {
        CNode *m_Next;
        const char *name;

        const char *Value() const;

        CNode(CNode *next, const char *value) :
                m_Next(next), name(value) {}
    };

    CNode *m_Begin = nullptr;
    size_t size_ = 0;

public:
    // default constructor
    CLinkedSet() = default;

    // copy constructor

    // operator=

    // destructor
    ~CLinkedSet(){
        CNode *tmp = m_Begin;
        CNode *node;
        while (m_Begin) {
            node = tmp;
            tmp = tmp->m_Next;
            delete node;
        }

    }
    bool Insert(const char *value) {
        CNode *node = new CNode(nullptr, value);
        CNode *tmp = m_Begin;
        if (m_Begin == nullptr) {
            m_Begin = node;
        } else {
            while (tmp) {
                tmp = tmp->m_Next;
            }
            tmp = node;
        }
        size_++;
        return true;
    }

    bool Remove(const char *value) {
        CNode *tmp = m_Begin;
        CNode *prev;
        while (tmp) {
            if (tmp->name == value) {
                prev->m_Next = tmp->m_Next;
                delete tmp;
                return true;
            }
            prev = tmp;
            tmp = tmp->m_Next;
        }
        return false;
    }

    bool Empty() const {
        if (m_Begin == nullptr)
            return true;
        return false;
    }

    size_t Size() const {
        return size_;
    }

    bool Contains(const char *value) const {
        CNode *tmp = m_Begin;
        while (tmp) {
            if (tmp->name == value)
                return true;
            tmp = tmp->m_Next;
        }
        return false;
    }

    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__

#include <cassert>

struct CLinkedSetTester {
    static void test0() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        assert(!x0.Empty());
    }

    static void test1() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        CLinkedSet x1(x0);
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test2() {
        CLinkedSet x0;
        CLinkedSet x1;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        x1 = x0;
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

};

int main() {
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    return 0;
}

#endif /* __PROGTEST__ */