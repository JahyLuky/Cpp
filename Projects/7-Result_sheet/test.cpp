#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

// (a, b) == (b, a)
struct pair_compare {
    bool operator()(const pair<string, string> &a, const pair<string, string> &b) const {
        if (a.first < b.first) {
            return true;
        } else if (a.first > b.first) {
            return false;
        } else {
            if (a.second < b.second) {
                return true;
            } else {
                return false;
            }
        }
    }
};

// my algorithm is based on pseudocode from BI-AG1: TopSort (3rd handout)
class Graph {
public:
    map<string, pair<int, vector<string>>> edges;

    void add_edge(const string &name, int loses, const string &name2) {
        auto itr = edges.find(name);

        if (itr == edges.end()) {
            vector<string> tmp;
            tmp.push_back(name2);
            edges.insert({name, make_pair(loses, tmp)});
        } else {
            vector<string> &tmp = itr->second.second;
            tmp.push_back(name2);
            itr->second.first += loses;
        }
    }
};

template<typename M_>
class CContest {
public:
    // default constructor
    explicit CContest() = default;

    // destructor
    ~CContest() = default;

    // addMatch ( contestant1, contestant2, result )
    CContest &addMatch(const string &contestant1, const string &contestant2, const M_ &result) {
        auto itr1 = data_.find(make_pair(contestant1, contestant2));
        auto itr2 = data_.find(make_pair(contestant2, contestant1));

        if (itr1 != data_.end() || itr2 != data_.end()) {
            throw logic_error("duplicate match");
        }

        data_.insert({make_pair(contestant1, contestant2), result});

        return *this;
    }

    // isOrdered ( comparator )
    bool isOrdered(function<int(M_)> cmp) const noexcept {
        Graph graph;

        // create graph
        for (const auto &i: data_) {
            const auto &contestant1 = i.first.first;
            const auto &contestant2 = i.first.second;
            const auto &result = i.second;

            if (cmp(result) == 0) {
                return false;
            } else if (cmp(result) > 0) {
                graph.add_edge(contestant1, 0, contestant2);
                graph.add_edge(contestant2, 1, contestant1);
            } else {
                graph.add_edge(contestant1, 1, contestant2);
                graph.add_edge(contestant2, 0, contestant1);
            }
        }

        int flag = 0;
        string winner;

        // find path
        for (const auto &i: graph.edges) {
            if (i.second.first == 0) {
                winner = i.first;
                flag++;
            }
        }

        // multiple winners (0 loses) or no winner
        if (flag > 1 || flag == 0) {
            return false;
        }

        queue<string> Q;
        Q.push(winner);
        flag = 0;
        size_t max_loses = 1;
        while (!Q.empty()) {
            auto itr = graph.edges.find(Q.front());
            cout << "win: " << itr->first;
            Q.pop();
            for (const auto &j: itr->second.second) {
                auto itr2 = graph.edges.find(j);
                if (itr2->second.first == 0) {
                    continue;
                }
                itr2->second.first--;
                if (itr2->second.first == 0) {
                    max_loses++;
                    Q.push(j);
                    flag++;
                }
                cout << ", lose: " << itr2->first;
            }
            cout << endl;
            if (flag > 1) {
                return false;
            }
            flag = 0;
        }

        if (max_loses != graph.edges.size())
            return false;
        return true;
    }

    // results ( comparator )
    list<string> results(function<int(M_)> cmp) const {
        list<string> res;
        Graph graph;

        // create graph
        for (const auto &i: data_) {
            const auto &contestant1 = i.first.first;
            const auto &contestant2 = i.first.second;
            const auto &result = i.second;

            if (cmp(result) == 0) {
                throw logic_error("tie match");
            } else if (cmp(result) > 0) {
                graph.add_edge(contestant1, 0, contestant2);
                graph.add_edge(contestant2, 1, contestant1);
            } else {
                graph.add_edge(contestant1, 1, contestant2);
                graph.add_edge(contestant2, 0, contestant1);
            }
        }

        int flag = 0;
        string winner;

        // find path
        for (const auto &i: graph.edges) {
            if (i.second.first == 0) {
                winner = i.first;
                flag++;
            }
        }

        // multiple winners (0 loses) or no winner
        if (flag > 1 || flag == 0) {
            throw logic_error("no winner");
        }

        queue<string> Q;
        Q.push(winner);
        flag = 0;
        size_t max_loses = 1;
        while (!Q.empty()) {
            auto itr = graph.edges.find(Q.front());
            res.push_back(itr->first);
            cout << "win: " << itr->first;
            Q.pop();
            for (const auto &j: itr->second.second) {
                auto itr2 = graph.edges.find(j);
                if (itr2->second.first == 0) {
                    continue;
                }
                itr2->second.first--;
                if (itr2->second.first == 0) {
                    Q.push(j);
                    flag++;
                    max_loses++;
                }
                cout << ", lose: " << itr2->first;
            }
            cout << endl;
            if (flag > 1) {
                throw logic_error("no winner");
            }
            flag = 0;
        }
        if (max_loses != graph.edges.size())
            throw logic_error("no winner");
        return res;
    }

private:
    map<pair<string, string>, M_, pair_compare> data_;
};

#ifndef __PROGTEST__

struct CMatch {
public:
    CMatch(int a, int b)
            : m_A(a), m_B(b) {}

    int m_A;
    int m_B;
};

class HigherScoreThreshold {
public:
    HigherScoreThreshold(int diffAtLeast)
            : m_DiffAtLeast(diffAtLeast) {}

    int operator()(const CMatch &x) const {
        return (x.m_A > x.m_B + m_DiffAtLeast) - (x.m_B > x.m_A + m_DiffAtLeast);
    }

private:
    int m_DiffAtLeast;
};

int HigherScore(const CMatch &x) {
    return (x.m_A > x.m_B) - (x.m_B > x.m_A);
}

int main() {
    CContest<CMatch> x;

    x.addMatch("C++", "Pascal", CMatch(10, 3))
            .addMatch("C++", "Java", CMatch(8, 1))
            .addMatch("Pascal", "Basic", CMatch(40, 0))
            .addMatch("Java", "PHP", CMatch(6, 2))
            .addMatch("Java", "Pascal", CMatch(7, 3))
            .addMatch("PHP", "Basic", CMatch(10, 0));


    assert (!x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    x.addMatch("PHP", "Pascal", CMatch(3, 6));

    assert (x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }


    assert (!x.isOrdered(HigherScoreThreshold(3)));
    try {
        list<string> res = x.results(HigherScoreThreshold(3));
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    assert (x.isOrdered([](const CMatch &x) {
        return (x.m_A < x.m_B) - (x.m_B < x.m_A);
    }));
    try {
        list<string> res = x.results([](const CMatch &x) {
            return (x.m_A < x.m_B) - (x.m_B < x.m_A);
        });
        assert ((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    CContest<bool> y;

    y.addMatch("Python", "PHP", true)
            .addMatch("PHP", "Perl", true)
            .addMatch("Perl", "Bash", true)
            .addMatch("Bash", "JavaScript", true)
            .addMatch("JavaScript", "VBScript", true);

    assert (y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    y.addMatch("PHP", "JavaScript", false);
    assert (!y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("PHP", "JavaScript", false);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("JavaScript", "PHP", true);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */