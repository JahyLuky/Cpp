#include <iostream>
#include <sstream>
#include <cassert>

#define LVL 3

struct LinkedList {
    using T = int;

    // konstruktor
    //LinkedList() : head_{nullptr} {}
    LinkedList() = default;

    // destruktor
    ~LinkedList() {
        while (!empty()) {
            pop();
        }
    }

    // push
    void push(const T &new_value) {
        Node *n = new Node;
        n->value = new_value;
        n->next = head_;
        head_ = n;
    }

    // pop
    T pop() {
        Node *old_head = head_;
        head_ = head_->next;
        T old_val = old_head->value;
        delete old_head;
        return old_val;
    }

    // top -- první prvek, ale bez odebrání
    T top () const {
        return head_->value;
    }

    // size
    size_t size() const {
        size_t cnt = 0;
        Node *tmp = head_;
        while (tmp) {
            cnt++;
            tmp = tmp->next;
        }
        return cnt;
    }

    // empty
    bool empty() const {
        if (head_ == nullptr) {
            return true;
        }
        return false;
    }

    // reverse -- otočí seznam

    // print -- vypíše seznam do zadaného streamu,
    // top jako první, typ streamu je std::ostream&

    // give -- přesune k prvků do zadaného
    // LinkedListu bez alokací (k je druhý argument,
    // defaultně 1)

    // třídní metoda merge, která slije dva setříděné
    // seznamy (a tím je vyprázdní)

private:
    struct Node {
        T value;
        Node *next;
    };
    Node *head_ = nullptr;
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
    (void) L; // unused warning...


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
# if LVL == 2
    while (!L.empty()) L.pop();
# endif


    // Mimosoutěžní bonus: výjimky
# if 0
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

      auto m = LinkedList::merge(a, b);
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

