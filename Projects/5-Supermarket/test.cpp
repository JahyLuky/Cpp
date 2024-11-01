#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

class CDate {
public:
    CDate(int y, int m, int d)
            : year(y), month(m), day(d) {}

            // clion created :)
    bool operator<(const CDate &rhs) const {
        return std::tie(year, month, day) < std::tie(rhs.year, rhs.month, rhs.day);
    }

    bool operator>(const CDate &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const CDate &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const CDate &rhs) const {
        return !(*this < rhs);
    }

private:
    int year;
    int month;
    int day;
};

class CSupermarket {
public:
    CSupermarket() = default;

    ~CSupermarket() = default;

    CSupermarket &store(const string &name, const CDate &expiryDate, const int count) {
        if (m_warehouse.find(name) == m_warehouse.end()) {
            pair<CDate, int> tmp = make_pair(expiryDate, count);
            m_warehouse[name].insert(tmp);
        } else {
            m_warehouse[name][expiryDate] += count;
        }
        return *this;
    }

    static bool find_match(const string &a, const string &b) {
        unsigned int len_a = a.length();
        unsigned int len_b = b.length();
        if (len_a != len_b)
            return false;

        unsigned int cnt = 0;
        for (unsigned int i = 0; i < len_a; i++) {
            if (a[i] != b[i])
                cnt++;
            if (cnt > 1)
                return false;
        }
        return true;
    }


    void sell(list<pair<string, int>> &shoppingList) {
        string list_name;
        string map_name;
        string ahoj;
        int pocet_shod = 0;

        vector<string> name_warehouse_empty;
        list<pair<string, int>> tmp_shoplist;

        //auto toSell = shoppingList.begin();
        //list<pair<string, int>>::iterator toSell;
        // for na nakupni seznam
        for (auto toSell = shoppingList.begin(); toSell != shoppingList.end(); ++toSell) {
            list_name = toSell->first;
            pocet_shod = 0;
            // presna shoda jmen
            if (m_warehouse.count(toSell->first) == 1) {
                auto name_warehouse = m_warehouse[toSell->first].begin();
                //cout << "shoda: " << m_warehouse.find(name_warehouse->first) << endl;

                // for na ruzne data podle jmena co chceme prodat
                while (!m_warehouse[toSell->first].empty()) {
                    // chceme prodat min nebo stejne co mame na sklade
                    if (toSell->second < name_warehouse->second) {
                        name_warehouse->second -= toSell->second;
                        toSell = shoppingList.erase(toSell);
                        --toSell;
                        break;
                    } else { // chceme koupit vic nez mame na sklade
                        toSell->second -= name_warehouse->second;
                        name_warehouse = m_warehouse[toSell->first].erase(name_warehouse);
                        if (m_warehouse[toSell->first].empty()) {
                            name_warehouse_empty.emplace_back(toSell->first);
                        }
                    }
                }
                continue;
            } else {
                // for podle jmen skladu
                for (auto &first_map: m_warehouse) {
                    map_name = first_map.first;
                    // shoda v listu a mape s 1 znakem jinym
                    if (find_match(list_name, map_name)) {
                        pocet_shod++;
                        ahoj = map_name;
                    }
                }
                // mame prave 1 shodu ve jmenech
                if (pocet_shod == 1) {
                    auto name_warehouse = m_warehouse[ahoj].begin();
                    while (!m_warehouse[ahoj].empty()) {
                        if (toSell->second < name_warehouse->second) {
                            name_warehouse->second -= toSell->second;
                            toSell = shoppingList.erase(toSell);
                            --toSell;
                            //tmp_shoplist.emplace_back(make_pair(toSell->first, toSell->second));
                            //toSell->second = 0;
                            break;
                        } else {
                            toSell->second -= name_warehouse->second;
                            name_warehouse = m_warehouse[toSell->first].erase(name_warehouse);
                            if (m_warehouse[ahoj].empty()) {
                                name_warehouse_empty.emplace_back(toSell->first);
                            }
                        }
                    }
                }
            }

        }


        auto tmp = name_warehouse_empty.begin();
        for (unsigned int i = 0; i < name_warehouse_empty.size(); ++i, ++tmp) {
            m_warehouse.erase(*tmp);
        }
    }

    list<pair<string, int>> expired(const CDate &date) const {
        list<pair<string, int>> expired_list;
        multimap<int, string, greater<>> tmp;
        int cnt = 0, was_expired = 0;

        for (const auto &first_map: m_warehouse) {
            for (const auto &second_map: first_map.second) {
                if (second_map.first < date) {
                    cnt += second_map.second;
                    was_expired = 1;
                }
            }
            if (was_expired) {
                tmp.emplace(cnt, first_map.first);
            }
            cnt = 0;
            was_expired = 0;
        }

        for (const auto &itr: tmp) {
            //cout << itr.second << " " << itr.first << " ";
            expired_list.emplace_back(itr.second, itr.first);
        }
        //cout << endl;
        //cout << "expired: " << expired_list.size() << " " << endl;
        return expired_list;
    }

    static void printWarehouse(const CSupermarket &s) {
        int cnt = 0;
        for (const auto &first_map: s.m_warehouse) {
            cout << cnt++ << ". " << first_map.first << ": ";
            for (const auto &second_map: first_map.second) {
                cout << second_map.second << endl;
            }
        }
    }

private:
    unordered_map<string, map<CDate, int>> m_warehouse;
};

#ifndef __PROGTEST__

int main() {
    CSupermarket s;
    s.store("bread", CDate(2016, 4, 30), 100)
            .store("butter", CDate(2016, 5, 10), 10)
            .store("beer", CDate(2016, 8, 10), 50)
            .store("bread", CDate(2016, 4, 25), 100)
            .store("okey", CDate(2016, 7, 18), 5);

    //CSupermarket::printWarehouse(s);

    list<pair<string, int> > l0 = s.expired(CDate(2018, 4, 30));
    assert (l0.size() == 4);
    assert ((l0 == list<pair<string, int> >{{"bread",  200},
                                            {"beer",   50},
                                            {"butter", 10},
                                            {"okey",   5}}));

    list<pair<string, int> > l1{{"bread",  2},
                                {"Coke",   5},
                                {"butter", 20}};
    s.sell(l1);
    assert (l1.size() == 2);
    assert ((l1 == list<pair<string, int> >{{"Coke",   5},
                                            {"butter", 10}}));

    list<pair<string, int> > l2 = s.expired(CDate(2016, 4, 30));

    assert (l2.size() == 1);
    assert ((l2 == list<pair<string, int> >{{"bread", 98}}));

    list<pair<string, int> > l3 = s.expired(CDate(2016, 5, 20));
    assert (l3.size() == 1);
    assert ((l3 == list<pair<string, int> >{{"bread", 198}}));

    list<pair<string, int> > l4{{"bread", 105}};
    s.sell(l4);
    assert (l4.size() == 0);
    assert ((l4 == list<pair<string, int> >{}));

    list<pair<string, int> > l5 = s.expired(CDate(2017, 1, 1));
    assert (l5.size() == 3);
    assert ((l5 == list<pair<string, int> >{{"bread", 93},
                                            {"beer",  50},
                                            {"okey",  5}}));

    s.store("Coke", CDate(2016, 12, 31), 10);

    list<pair<string, int> > l6{{"Cake",  1},
                                {"Coke",  1},
                                {"cake",  1},
                                {"coke",  1},
                                {"cuke",  1},
                                {"Cokes", 1}};
    //CSupermarket::printWarehouse(s);
    s.sell(l6);
    assert (l6.size() == 3);
    assert ((l6 == list<pair<string, int> >{{"cake",  1},
                                            {"cuke",  1},
                                            {"Cokes", 1}}));

    list<pair<string, int> > l7 = s.expired(CDate(2017, 1, 1));
    assert (l7.size() == 4);
    assert ((l7 == list<pair<string, int> >{{"bread", 93},
                                            {"beer",  50},
                                            {"Coke",  7},
                                            {"okey",  5}}));

    s.store("cake", CDate(2016, 11, 1), 5);

    list<pair<string, int> > l8{{"Cake", 1},
                                {"Coke", 1},
                                {"cake", 1},
                                {"coke", 1},
                                {"cuke", 1}};
    s.sell(l8);
    assert (l8.size() == 2);
    assert ((l8 == list<pair<string, int> >{{"Cake", 1},
                                            {"coke", 1}}));

    list<pair<string, int> > l9 = s.expired(CDate(2017, 1, 1));
    assert (l9.size() == 5);
    assert ((l9 == list<pair<string, int> >{{"bread", 93},
                                            {"beer",  50},
                                            {"Coke",  6},
                                            {"okey",  5},
                                            {"cake",  3}}));

    list<pair<string, int> > l10{{"cake", 15},
                                 {"Cake", 2}};
    s.sell(l10);
    assert (l10.size() == 2);
    assert ((l10 == list<pair<string, int> >{{"cake", 12},
                                             {"Cake", 2}}));

    list<pair<string, int> > l11 = s.expired(CDate(2017, 1, 1));
    assert (l11.size() == 4);
    assert ((l11 == list<pair<string, int> >{{"bread", 93},
                                             {"beer",  50},
                                             {"Coke",  6},
                                             {"okey",  5}}));

    list<pair<string, int> > l12{{"Cake", 4}};
    s.sell(l12);
    assert (l12.size() == 0);
    assert ((l12 == list<pair<string, int> >{}));

    list<pair<string, int> > l13 = s.expired(CDate(2017, 1, 1));
    assert (l13.size() == 4);
    assert ((l13 == list<pair<string, int> >{{"bread", 93},
                                             {"beer",  50},
                                             {"okey",  5},
                                             {"Coke",  2}}));

    list<pair<string, int> > l14{{"Beer", 20},
                                 {"Coke", 1},
                                 {"bear", 25},
                                 {"beer", 10}};
    s.sell(l14);
    assert (l14.size() == 1);
    assert ((l14 == list<pair<string, int> >{{"beer", 5}}));

    s.store("ccccb", CDate(2019, 3, 11), 100)
            .store("ccccd", CDate(2019, 6, 9), 100)
            .store("dcccc", CDate(2019, 2, 14), 100);

    list<pair<string, int> > l15{{"ccccc", 10}};
    s.sell(l15);
    assert (l15.size() == 1);
    assert ((l15 == list<pair<string, int> >{{"ccccc", 10}}));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
