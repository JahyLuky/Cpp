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
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>

using namespace std;

class CDate {
public:
    //---------------------------------------------------------------------------------------------
    CDate(int y, int m, int d)
            : m_Year(y), m_Month(m), m_Day(d) {}

    //---------------------------------------------------------------------------------------------
    int compare(const CDate &x) const {
        if (m_Year != x.m_Year)
            return m_Year - x.m_Year;
        if (m_Month != x.m_Month)
            return m_Month - x.m_Month;
        return m_Day - x.m_Day;
    }

    //---------------------------------------------------------------------------------------------
    int year() const {
        return m_Year;
    }

    //---------------------------------------------------------------------------------------------
    int month() const {
        return m_Month;
    }

    //---------------------------------------------------------------------------------------------
    int day() const {
        return m_Day;
    }

    //---------------------------------------------------------------------------------------------
    friend ostream &operator<<(ostream &os,
                               const CDate &x) {
        char oldFill = os.fill();
        return os << setfill('0') << setw(4) << x.m_Year << "-"
                  << setw(2) << static_cast<int>(x.m_Month) << "-"
                  << setw(2) << static_cast<int>(x.m_Day)
                  << setfill(oldFill);
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};

#endif /* __PROGTEST__ */

string removeExtraSpaces(const string &str) {
    string result;
    bool last_space = true;
    for (auto itr: str) {
        if (itr != ' ') {
            result.push_back(itr);
            last_space = false;
        } else if (!last_space) {
            result.push_back(' ');
            last_space = true;
        }
    }
    if (result.back() == ' ')
        result.pop_back();

    return result;
}

struct Company {
    string seller_;
    string buyer_;
    string seller_format_;
    string buyer_format_;
    int register_order_;

    Company() = default;

    Company(const string &seller, const string &buyer, int order)
            : seller_(seller), buyer_(buyer),
              seller_format_(removeExtraSpaces(seller)),
              buyer_format_(removeExtraSpaces(buyer)), register_order_(order) {}
};

class CInvoice {
public:
    CInvoice();

    CInvoice(const CDate &date, const string &seller,
             const string &buyer, unsigned int amount, double vat)
            : date_(date), comp_(seller, buyer, 0),
              amount_(amount), vat_(vat) {}

    CDate date() const {
        return date_;
    }

    string buyer() const {
        return comp_.buyer_;
    }

    string buyer_formated() const {
        return comp_.buyer_format_;
    }

    string seller() const {
        return comp_.seller_;
    }

    string seller_formated() const {
        return comp_.seller_format_;
    }

    unsigned int amount() const {
        return amount_;
    }

    double vat() const {
        return vat_;
    }

    int order() const {
        return comp_.register_order_;
    }

    bool operator==(const CInvoice &a) const {
        if (a.amount() == amount() && a.vat() == vat()
            && a.date().compare(date()) == 0
            && strcasecmp(a.seller_formated().c_str(), seller_formated().c_str()) == 0
            && strcasecmp(a.buyer_formated().c_str(), buyer_formated().c_str()) == 0) {
            return true;
        }
        return false;
    }

    bool operator!=(const CInvoice &a) const {
        if (*this == a)
            return false;
        return true;
    }

    CDate date_;
    Company comp_;
    unsigned int amount_;
    double vat_;
private:
};

class CSortOpt {
public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    CSortOpt() = default;

    CSortOpt &addKey(int key, bool ascending = true) {
        sort_level_.emplace_back(key, ascending);
        return *this;
    }

    bool operator()(const CInvoice &left, const CInvoice &right) const {
        for (auto &key: sort_level_) {
            switch (key.first) {
                case BY_DATE:
                    if (left.date().compare(right.date()) != 0) {
                        if (key.second) {
                            return left.date().compare(right.date()) < 0;
                        } else {
                            return left.date().compare(right.date()) > 0;
                        }
                    }
                    break;
                case BY_BUYER:
                    if (left.buyer_formated() != right.buyer_formated()) {
                        if (key.second) {
                            return strcasecmp(left.buyer().c_str(), right.buyer().c_str()) < 0;
                        } else {
                            return strcasecmp(left.buyer().c_str(), right.buyer().c_str()) > 0;
                        }
                    }
                    break;
                case BY_SELLER:
                    if (strcasecmp(left.seller().c_str(), right.seller().c_str()) != 0) {
                        if (key.second) {
                            return strcasecmp(left.seller().c_str(), right.seller().c_str()) < 0;
                        } else {
                            return strcasecmp(left.seller().c_str(), right.seller().c_str()) > 0;
                        }
                    }
                    break;
                case BY_AMOUNT:
                    if (left.amount() != right.amount()) {
                        if (left.vat() != right.vat()) {
                            return left.amount() < right.amount();
                        } else {
                            return left.amount() > right.amount();
                        }
                    }
                    break;
                case BY_VAT:
                    if (left.vat() != right.vat()) {
                        if (key.second) {
                            return left.vat() < right.vat();
                        } else {
                            return left.vat() > right.vat();
                        }
                    }
                    break;
            }
        }
        return true;
    }

private:
    vector<pair<int, bool>> sort_level_;
};

// https://courses.fit.cvut.cz/BI-PA2/media/lectures/l06-stl-cz.pdf
struct CompareInvoices {
    bool operator()(const string &a, const string &b) const {
        string tmp1 = removeExtraSpaces(a);
        string tmp2 = removeExtraSpaces(b);
        return strcasecmp(tmp1.c_str(), tmp2.c_str()) < 0;
    }

    bool operator()(const CInvoice &a, const CInvoice &b) const {
        if (a.date().compare(b.date()) != 0) {
            return a.date().compare(b.date()) < 0;
        }

        if (strcasecmp(a.seller_formated().c_str(), b.seller_formated().c_str()) != 0) {
            return strcasecmp(a.seller_formated().c_str(), b.seller_formated().c_str()) < 0;
        }

        if (strcasecmp(a.buyer_formated().c_str(), b.buyer_formated().c_str()) != 0) {
            return strcasecmp(a.buyer_formated().c_str(), b.buyer_formated().c_str()) < 0;
        }

        if (a.amount() != b.amount()) {
            return a.amount() < b.amount();
        }

        return a.vat() < b.vat();
    }

    bool operator()(const CInvoice &b, const string &a) const {
        return strcasecmp(a.c_str(), b.seller_formated().c_str()) < 0;
    }

};

class CVATRegister {
public:
    CVATRegister() = default;

    bool registerCompany(const string &name) {
        auto res = comp_.insert(name);

        if (!res.second)
            return false;

        return true;
    }

    bool checkRegister1(CInvoice &x) {
        if (strcasecmp(x.seller_formated().c_str(), x.buyer_formated().c_str()) == 0) {
            return false;
        }

        auto sell_itr = comp_.find(x.seller_formated());
        if (sell_itr == comp_.end())
            return false;

        auto buy_itr = comp_.find(x.buyer_formated());
        if (buy_itr == comp_.end())
            return false;

        x.comp_.seller_ = *sell_itr;
        x.comp_.buyer_ = *buy_itr;

        return true;
    }

    bool checkRegister(const CInvoice &x) const {
        if (strcasecmp(x.seller_formated().c_str(), x.buyer_formated().c_str()) == 0) {
            return false;
        }

        auto sell_itr = comp_.find(x.seller_formated());
        if (sell_itr == comp_.end())
            return false;

        if (comp_.find(x.buyer_formated()) == comp_.end())
            return false;

        return true;
    }

    bool addIssued(const CInvoice &x) {
        CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()), x.comp_.seller_, x.comp_.buyer_, x.amount_,
                     x.vat_);
        if (!checkRegister1(tmp))
            return false;

        auto res = sell_register_.insert(tmp);

        if (!res.second)
            return false;

        return true;
    }

    bool addAccepted(const CInvoice &x) {
        if (!checkRegister(x))
            return false;

        auto res = buy_register_.insert(x);

        if (!res.second)
            return false;

        return true;
    }

    bool delIssued(const CInvoice &x) {
        if (!checkRegister(x))
            return false;

        if (sell_register_.erase(x) == 0)
            return false;

        return true;
    }

    bool delAccepted(const CInvoice &x) {
        if (!checkRegister(x))
            return false;

        if (buy_register_.erase(x) == 0) {
            return false;
        }
        return true;
    }

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const {
        list<CInvoice> result;
        string company_format = removeExtraSpaces(company);

        bool isFoundSell = false;
        bool isFoundBuy = false;
        for (const auto &sell: sell_register_) {
            if (strcasecmp(sell.seller_formated().c_str(), company_format.c_str()) == 0
                || strcasecmp(sell.buyer_formated().c_str(), company_format.c_str()) == 0) {
                isFoundSell = true;
            }
            for (const auto &buy: buy_register_) {
                if (strcasecmp(buy.buyer_formated().c_str(), company_format.c_str()) == 0
                    || strcasecmp(buy.buyer_formated().c_str(), company_format.c_str()) == 0) {
                    isFoundBuy = true;
                    break;
                }
            }
            if (isFoundSell && !isFoundBuy) {
                result.emplace_back(sell);
            }
            isFoundSell = false;
            isFoundBuy = false;
        }

        result.sort(sortBy);

        return result;
    }

private:
    set<string, CompareInvoices> comp_;
    set<CInvoice, CompareInvoices> sell_register_;
    set<CInvoice, CompareInvoices> buy_register_;
};

#ifndef __PROGTEST__

bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
    if (a.size() != b.size()) {
        return false;
    }

    auto tmp1 = a.begin();
    auto tmp2 = b.begin();
    for (size_t i = 0; i < a.size(); ++i) {
        cout << tmp1->date() << " == " << tmp2->date() << endl;
        cout << tmp1->seller() << " == " << tmp2->seller() << endl;
        cout << tmp1->buyer() << " == " << tmp2->buyer() << endl;
        cout << tmp1->amount() << " == " << tmp2->amount() << endl;
        cout << tmp1->vat() << " == " << tmp2->vat() << endl;
        cout << "-----------------" << endl;
        if (*tmp1 != *tmp2) {
            return false;
        }
        tmp1++;
        tmp2++;
    }
    return true;
}

int main() {
    CVATRegister r;
    assert(r.registerCompany("first Company"));
    assert(r.registerCompany("Second     Company"));
    assert(r.registerCompany("ThirdCompany, Ltd."));
    assert(r.registerCompany("Third Company, Ltd."));
    assert(!r.registerCompany("Third Company, Ltd."));
    assert(!r.registerCompany(" Third  Company,  Ltd.  "));

    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 20)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 2), "FirSt Company", "Second Company ", 200, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "Second Company ", "First Company", 300, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34)));
    assert(!r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert(!r.addIssued(CInvoice(CDate(2000, 1, 4), "First Company", "First   Company", 200, 30)));
    assert(!r.addIssued(CInvoice(CDate(2000, 1, 4), "Another Company", "First   Company", 200, 30)));

    assert(equalLists(r.unmatched("First Company",
                                  CSortOpt().addKey(CSortOpt::BY_SELLER, true)
                                          .addKey(CSortOpt::BY_BUYER, false)
                                          .addKey(CSortOpt::BY_DATE, false)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));

    /*
    assert(equalLists(r.unmatched("First Company",
                                  CSortOpt().addKey(CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(
                                          CSortOpt::BY_BUYER, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000)}));
    assert(equalLists(r.unmatched("First Company",
                                  CSortOpt().addKey(CSortOpt::BY_VAT, true).addKey(CSortOpt::BY_AMOUNT, true).addKey(
                                          CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(
                                          CSortOpt::BY_BUYER, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
    assert(equalLists(r.unmatched("First Company", CSortOpt()),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
    assert(equalLists(r.unmatched("second company", CSortOpt().addKey(CSortOpt::BY_DATE, false)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Third Company, Ltd.", "Second     Company", 400,
                                       34.000000)}));
    assert(equalLists(r.unmatched("last company", CSortOpt().addKey(CSortOpt::BY_VAT, true)),
                      list<CInvoice>{}));
    */
    assert(r.addAccepted(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company ", 200, 30)));
    assert(r.addAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert(r.addAccepted(CInvoice(CDate(2000, 1, 1), "Second company ", "First Company", 300, 32)));
    /*
    assert(equalLists(r.unmatched("First Company",
                                  CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                          CSortOpt::BY_DATE, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
    */
    assert(!r.delIssued(CInvoice(CDate(2001, 1, 1), "First Company", "Second Company ", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "A First Company", "Second Company ", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Hand", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 1200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 200, 130)));
    assert(r.delIssued(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company", 200, 30)));
    /*
     assert(equalLists(r.unmatched("First Company",
                                  CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                          CSortOpt::BY_DATE, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
    */
    assert(r.delAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    /*
     assert(equalLists(r.unmatched("First Company",
                                  CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                          CSortOpt::BY_DATE, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
    */
    assert(r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    /*
     assert(equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>{
                               CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                               CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                               CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                               CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                               CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                               CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
     */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */