#include <iostream>
#include <sstream>
#include <cassert>
// Žádné unique_ptr nebo shared_ptr


#define LVL 1


struct List {
    using T = int;

    List() = default;

    List(std::initializer_list<T> l) {
        for (const T &t: l) push(t);
    }

    // TODO kopírování
    List(const List &other) {

    }

    const List &operator=(const List &a) {

    }

    ~List() { while (!empty()) pop(); }


    void push(const T &x) {
        head = new Node{head, x};
    }

    T pop() {
        _check_nonempty();
        Node *tmp = head;
        head = head->next;
        T ret = tmp->value;
        delete tmp;
        return ret;
    }


    const T &top(size_t n = 0) const { return _at(head, n); }

    T &top(size_t n = 0) { return _at(head, n); }


    size_t size() const {
        size_t ret = 0;
        for (Node *n = head; n; n = n->next) ret++;
        return ret;
    }

    bool empty() const { return !head; }


    void swap(List &other) { // prohodí obsah s jiným seznamem
        using std::swap;
        swap(head, other.head);
    }

    void reverse() {
        List tmp;
        while (!empty()) *this->*tmp;
        swap(tmp);
    }


    // funguje jako other.push(this->pop()), ale bez zbytečných alokací
    friend void operator->*(List &l, List &r) {
        l._check_nonempty();
        if (&l == &r) return;
        using std::swap;
        swap(r.head, l.head->next); // magie :-)
        swap(r.head, l.head);
    }


    friend std::ostream &operator<<(std::ostream &out, const List &l) {
        for (Node *n = l.head; n; n = n->next) {
            if (n != l.head) out << " ";
            out << n->value;
        }
        return out;
    }

private:
    struct Node {
        Node *next;
        T value;
    };

    static T &_at(Node *n, size_t i) {
        while (1) {
            if (!n) throw std::out_of_range("");
            if (i-- == 0) return n->value;
            n = n->next;
        }
    }

    void _check_nonempty() const {

        if (!head) throw std::out_of_range("Stack is empty");
    }

    Node *head = nullptr;
};


template<typename T>
std::string to_string(const T &t) {
    std::ostringstream out;
    out << t;
    return out.str();
}


void test_list() {
    List A = {1, 5, 7, 9, 11};
    List B;

    (void) A;
    (void) B;


    // Copy constructor
# if LVL >= 1
    List C = A;
    C.top(1) = -5;
    C.push(13);
    std::cout << A << std::endl;
    assert(to_string(A) == "11 9 7 5 1");
    assert(to_string(C) == "13 11 -5 7 5 1");
# endif


    // Operátor přiřazení
# if LVL >= 2
    B.push(15);
  B.push(16);
  assert(to_string(B) == "16 15");

  B = C;
  B.top(2) = 42;
  assert(to_string(C) == "13 11 -5 7 5 1");
  assert(to_string(B) == "13 11 42 7 5 1");
# endif
}


#if LVL >= 3
// Jako List, ale přiřazení / kopírování dělá
// mělkou kopii s počítanými referencemi
struct SharedList {
  using T = int;

  // TODO
};
#endif


void test_shared_list() {

    // Copy constructor
# if LVL >= 3
    SharedList A = { 1, 2, 3 };
  SharedList B;

  B.push(12);
  B.push(11);

  std::cout << A << std::endl;
  assert(to_string(A) == "3 2 1");
# endif


    // Operátor =
# if LVL >= 4
    B = A;
  B.pop();
  assert(to_string(A) == "2 1");
  assert(to_string(B) == "2 1");
# endif
}


#if LVL >= 5
template < typename T >
struct SharedPtr {
  // TODO
};
#endif


void test_shared_ptr() {

    // konstruktory & destruktor
# if LVL >= 5
    SharedPtr<int> a;
  SharedPtr<int> b(new int{5});
  SharedPtr<int> c(b);
# endif


    // operátor bool a *
# if LVL >= 6
    assert(!a);
  assert(b);
  assert(*b == 5);
  *c = 4;
  assert(*c == 4);
  assert(*b == 4);
# endif


    // operátor =
# if LVL >= 7
    a = c;
  c = nullptr;
  // c = new int; // NESMÍ fungovat
  assert(!c);
  assert(*a == 4);
# endif


    // operátor ->
# if LVL >= 8
    SharedPtr<List> l(new List);
  l->push(1);
  l->push(2);
  assert(to_string(*l) == "2 1");
# endif
}


int main() {
    test_list();
    test_shared_list();
    test_shared_ptr();
    std::cout << "Successfully completed level " << LVL << ".\n";
}