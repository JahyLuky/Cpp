#include <iostream>
#include <sstream>
#include <cassert>

#define LVL 99

struct LinkedList {
  using T = int;

  LinkedList() = default;

  // bonus: konstruktor pro lvl 99
  LinkedList(std::initializer_list<T> l) {
    for (auto&& x : l) push(x);
  }

  LinkedList(const LinkedList&) = delete; // zakážeme kopírování
  void operator =(const LinkedList&) = delete; // i přiřazení

  ~LinkedList() {
    while (!empty()) pop();
  }


  void push(const T& x) {
    head = new Node{ head, x };
  }

  T pop() {
    _check_nonempty();
    Node *tmp = head;
    head = head->next;
    T ret = tmp->value;
    delete tmp;
    return ret;
  }

  const T& top() const {
    _check_nonempty();
    return head->value;
  }

  T& top() {
    _check_nonempty();
    return head->value;
  }


  size_t size() const {
    size_t ret = 0;
    for (Node *n = head; n; n = n->next) ret++;
    return ret;
  }

  bool empty() const { return !head; }


  void swap(LinkedList& other) { // prohodí obsah s jiným seznamem
    std::swap(head, other.head);
  }

  void reverse() {
    LinkedList tmp;
    while (!empty()) *this ->* tmp;
    swap(tmp);
  }

  // funguje jako other.push(this->pop()), ale bez zbytečných alokací
  void operator ->*(LinkedList& other) {
    _check_nonempty();
    std::swap(other.head, head->next); // magie :-)
    std::swap(other.head, head);
  }


  void print(std::ostream& out) const {
    for (Node *n = head; n; n = n->next) {
      if (n != head) out << " ";
      out << n->value;
    }
  }

  friend std::ostream& operator << (std::ostream& out, const LinkedList& l) {
    l.print(out);
    return out;
  }


  void give(LinkedList& other, size_t k = 1) {
    while (k-- > 0) *this ->* other;
  }


  // Nemůžeme vracet LinkedList, protože ho neumíme kopírovat.
  // Proto použijeme výstupní parametr.
  static void merge(LinkedList& a, LinkedList& b, LinkedList& out) {
    while (!a.empty() && !b.empty())
      (a.top() <= b.top() ? a : b) ->* out;

    while (!a.empty()) a ->* out;
    while (!b.empty()) b ->* out;

    out.reverse();
  }

  private:
  void _check_nonempty() const {
    if (empty()) throw std::out_of_range("The stack is empty");
  }

  struct Node {
    Node *next;
    T value;
  };

  Node *head = nullptr;
};


#if LVL >= 6
bool check_list(const LinkedList& L, const std::string& ref) {
  std::ostringstream out;
  L.print(out);
  return out.str() == ref;
}
#endif

int main() {
  LinkedList L;
  (void)L; // unused warning...


  // LVL 1: konstruktor, empty, push a pop
# if LVL >= 1
  assert(L.empty());

  for (int i = 0; i < 10; i++) L.push(i);
  assert(!L.empty());

  for (int i = 0; i < 5; i++) {
    int x = L.pop();
    assert(x == 9 - i);
  }
  assert(!L.empty());
# endif


  // LVL 2: destruktor
# if LVL == 1
  while (!L.empty()) L.pop();
# endif


  // Mimosoutěžní bonus: výjimky
# if 1
  try {
    LinkedList L2;
    L2.pop();
    assert(0);
  } catch (const std::out_of_range&) {
    // OK
  }
# endif


  // LVL 3: top a size
# if LVL >= 3
  assert(L.size() == 5);
  assert(L.top() == 4);
  L.push(-10);
  assert(L.size() == 6);
  assert(L.top() == -10);
# endif


  // LVL 4: reverse
# if LVL >= 4
  L.reverse();
  assert(L.size() == 6);
  assert(L.top() == 0);
  L.pop();
  assert(L.top() == 1);
# endif


  // LVL 5: print
# if LVL >= 5
  L.print(std::cout);
  std::cout << std::endl;

  std::ostringstream out;
  L.print(out);
  assert(out.str() == "1 2 3 4 -10");
# endif


  // LVL 6: merge
# if LVL >= 6
  {
    LinkedList a, b;
    for (int i = 0; i < 10; i += 2) a.push(i);
    for (int i = 1; i < 10; i += 2) b.push(i);
    a.reverse();
    b.reverse();
    assert(check_list(a, "0 2 4 6 8"));
    assert(check_list(b, "1 3 5 7 9"));

    LinkedList m;
    LinkedList::merge(a, b, m);
    assert(check_list(m, "0 1 2 3 4 5 6 7 8 9"));
    assert(a.empty());
    assert(b.empty());
  }
# endif


  // LVL 7: consty
# if LVL >= 7
  {
    const auto& constL = L;
    assert(constL.top() == 1);
    constL.empty();
    constL.size();
    constL.print(std::cout);
    std::cout << std::endl;

    // constL.top() = 42; // NESMÍ fungovat
    // constL.push(); // NESMÍ fungovat
    // constL.pop(); // NESMÍ fungovat

    L.top() = 43; // musí fungovat
    assert(constL.top() == 43);
  }
# endif


  // LVL 8: give
# if LVL >= 8
  {
    LinkedList a, b;
    for (int i = 0; i < 10; i++) a.push(i);
    a.give(b, 3);
    assert(check_list(a, "6 5 4 3 2 1 0"));
    assert(check_list(b, "7 8 9"));
  }
# endif


  // LVL 97: Výpis operátorem <<
# if LVL >= 97
  std::cout << L << std::endl;
# endif


  // LVL 98: give(., 1) pomocí operátoru ->*
# if LVL >= 98
  LinkedList L2;
  L ->* L2;
  assert(L2.size() == 1 && L2.top() == 43);
# endif


  // LVL 99: inicializace pomocí std::initializer_list
# if LVL >= 99
  LinkedList L3 = { 1, 2, 3, 4 };
  assert(check_list(L3, "4 3 2 1"));
# endif

  std::cout << "Successfully completed level " << LVL << ".\n";
}

