#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <string>
#include <sstream>


#define LVL 999

struct Set {
  using T = int;

  Set() = default;
  Set(const T& t) { insert(t); }


  bool empty() const { return _data.empty(); }
  size_t size() const { return _data.size(); }

  bool insert(const T& t) { // true, pokud vložil
    auto it = std::lower_bound(_data.begin(), _data.end(), t);
    if (it != _data.end() && *it == t) return false;
    _data.insert(it, t);
    return true;
  }

  bool remove(const T& t) { // true, pokud smazal
    auto it = std::lower_bound(_data.begin(), _data.end(), t);
    if (it == _data.end() || *it != t) return false;
    _data.erase(it);
    return true;
  }

  bool contains(const T& t) const {
    auto it = std::lower_bound(_data.begin(), _data.end(), t);
    return it != _data.end() && *it == t;
    // Alternativně můžeme použít binary_search
    // return std::binary_search(_data.begin(), _data.end(), t);
  }

  
  // Existence metod begin() a end() umožní použít
  // Set s range-for. Zde stačí vrátit const_iterátor
  // vektoru.
  auto begin() const { return _data.begin(); }
  auto end() const { return _data.end(); }


  const T& operator [] (size_t i) const { return _data.at(i); }


  explicit operator bool() const { return !empty(); }

  Set& operator |= (const Set& other) {
    *this = *this | other;
    return *this;
  }

  Set& operator &= (const Set& other) {
    *this = *this & other;
    return *this;
  }

  Set& operator -= (const Set& other) {
    // 3. argument remove_if je lambda funkce
    // - [&] znamená, že může přistupovat k lokálním
    //   priměnných funkce, ve které je definována
    // - bere jeden argument typu const T&
    auto new_end = std::remove_if(_data.begin(), _data.end(), [&](const T& x) {
      return other.contains(x);
    });

    _data.erase(new_end, _data.end());

    return *this;
  }

  friend Set operator & (const Set& A, const Set& B) {
    if (A.size() < B.size()) return B & A;

    Set ret;
    for (const auto& b : B) if (A.contains(b)) ret._data.push_back(b);
    return ret;
  }

  friend Set operator | (const Set& A, const Set& B) {
    Set ret;
    // Pokud chceme iterátorem přidávat do kontejneru,
    // je potřeba vyrobit speiální iterátor. Zde
    // pomocí back_inserter.
    std::set_union(
      A.begin(), A.end(),
      B.begin(), B.end(),
      std::back_inserter(ret._data)
    );
    return ret;
  }

  friend Set operator - (Set A, const Set& B) {
    return A -= B;
  }

  friend std::ostream& operator << (std::ostream& out, const Set& A) {
    out << "{";
    bool first = true;
    for (const auto& a : A) {
      out << (first ? " " : ", ") << a;
      first = false;
    }
    return out << " }";
  }


  // Porovnání chceme jen delegovat na obsažený
  // vektor. Pomůžeme si makrem, abychom se
  // neupsali.
# define CMP(op) \
  friend bool operator op (const Set& A, const Set& B) { return A._data op B._data; }

  CMP(==)
  CMP(!=)
  CMP(<)
  CMP(<=)
  CMP(>)
  CMP(>=)
# undef CMP

  private:
  std::vector<T> _data;
};


int main() {
  Set A;
  (void)A;


  // empty, size, insert
# if LVL >= 1
  assert(A.empty());
  bool s = A.insert(5);
  assert(s);
  s = A.insert(5);
  assert(!s);
  assert(A.size() == 1);
  s = A.insert(42);
  assert(s);
  assert(A.size() == 2);
  assert(!A.empty());
# endif


  // contains
# if LVL >= 2
  assert(A.contains(5));
  assert(A.contains(42));
  assert(!A.contains(43));
  assert(!A.contains(6));
  assert(!A.contains(4));
# endif


  // remove
# if LVL >= 3
  for (int i = 0; i < 10; i++) A.insert(2*i);
  for (int i = 0; i < 10; i++) assert(A.contains(2*i));
  for (int i = 3; i < 10; i++) {
    s = A.remove(2*i);
    assert(s);
  }
  for (int i = 0; i < 10; i++) assert(A.contains(2*i) == (i < 3));
# endif


  // výpis oprátorem <<
# if LVL >= 4
  {
    std::ostringstream out;
    out << A;
    assert(out.str() == "{ 0, 2, 4, 5, 42 }");
  }
  std::cout << A << std::endl;
# endif


  // operátor [] pro přístup k prvkům
# if LVL >= 5
  assert(A[0] == 0);
  assert(A[4] == 42);
  // A[1] = 5 NEMÁ fungovat, protože může porušit uspořádání
# endif


  // operátor bool
# if LVL >= 6
  {
    Set B;
    assert(!B);
    B.insert(0);
    assert(B);
    B.remove(1);
    assert(B);
    B.remove(0);
    assert(!B);
  }
#endif


  // operátory | a |= (sjednocení)
# if LVL >= 7
  Set B;
  for (int x : {3, 5, 17, 24}) B.insert(x);
  std::ostringstream out;
  out << (A | B);
  std::cout << out.str() << std::endl;
  assert(out.str() == "{ 0, 2, 3, 4, 5, 17, 24, 42 }");

  Set C;
  C.insert(15);
  C |= B;
  out.str("");
  out << C;
  std::cout << out.str() << std::endl;
  assert(out.str() == "{ 3, 5, 15, 17, 24 }");
# endif


  // operátory & a &= (průnik)
# if LVL >= 8
  out.str("");
  out << (B & A);
  std::cout << out.str() << std::endl;
  assert(out.str() == "{ 5 }");

  out.str("");
  out << (C &= B);
  std::cout << out.str() << std::endl;
  assert(out.str() == "{ 3, 5, 17, 24 }");
# endif


  // operátory - a -= (rozdíl)
# if LVL >= 9
  out.str("");
  out << (A - B);
  std::cout << out.str() << std::endl;
  assert(out.str() == "{ 0, 2, 4, 42 }");

  A -= B;
  out.str("");
  out << A;
  assert(out.str() == "{ 0, 2, 4, 42 }");
# endif


  // bonus: iterace pomocí range-for
  // jsou potřeba metody begin a end
# if 1
  for (auto x : A) std::cout << x << std::endl;
# endif


  // operátory porovnání
# if LVL >= 10
  assert(A < B);
  assert(B >= A);
  assert(A != B);
  assert(A == A);
  assert(!(A == B));
  assert(!(A > B));
  assert(!(A >= B));
  assert(A <= B);
#endif


  std::cout << "Successfully completed level " << LVL << ".\n";
}

