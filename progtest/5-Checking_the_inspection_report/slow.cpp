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

    // iterate over chars in string
    for (auto itr: str) {
        if (itr != ' ') {
            result.push_back(itr);
            last_space = false;
        } else if (!last_space) {
            result.push_back(' ');
            last_space = true;
        }
    }

    // pop ending space
    if (result.back() == ' ')
        result.pop_back();

    return result;
}

// stores company information
struct Company {
    string seller_;
    string buyer_;
    string seller_format_;
    string buyer_format_;
    size_t order_;

    Company() = default;

    Company(const string &seller, const string &buyer, int order)
            : seller_(seller), buyer_(buyer),
              seller_format_(removeExtraSpaces(seller)),
              buyer_format_(removeExtraSpaces(buyer)),
              order_(order) {}
};

class CInvoice {
public:
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
        return comp_.order_;
    }

    bool operator==(const CInvoice &a) const {
        if (a.amount() == amount() && a.vat() == vat() &&
            a.date().compare(date()) == 0 &&
            strcasecmp(a.seller_formated().c_str(), seller_formated().c_str()) == 0 &&
            strcasecmp(a.buyer_formated().c_str(), buyer_formated().c_str()) == 0) {
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

    // save criterion order for sort
    CSortOpt &addKey(int key, bool ascending = true) {
        sort_level_.emplace_back(key, ascending);
        return *this;
    }

    // sorting by criterions
    // key.first -> sorting cases
    // key.second - > true = asc, false = desc
    bool operator()(const CInvoice &left, const CInvoice &right) const {
        for (auto &key: sort_level_) {
            switch (key.first) {
                case BY_DATE: // 0
                    if (left.date().compare(right.date()) != 0) {
                        if (key.second) {
                            return left.date().compare(right.date()) < 0;
                        } else {
                            return left.date().compare(right.date()) > 0;
                        }
                    }
                    break;
                case BY_BUYER: // 1
                    if (left.buyer() != right.buyer()) {
                        if (key.second) {
                            return strcasecmp(left.buyer().c_str(), right.buyer().c_str()) < 0;
                        } else {
                            return strcasecmp(left.buyer().c_str(), right.buyer().c_str()) > 0;
                        }
                    }
                    break;
                case BY_SELLER: // 2
                    if (strcasecmp(left.seller().c_str(), right.seller().c_str()) != 0) {
                        if (key.second) {
                            return strcasecmp(left.seller().c_str(), right.seller().c_str()) < 0;
                        } else {
                            return strcasecmp(left.seller().c_str(), right.seller().c_str()) > 0;
                        }
                    }
                    break;
                case BY_AMOUNT: // 3
                    if (left.amount() != right.amount()) {
                        if (key.second) {
                            return left.amount() < right.amount();
                        } else {
                            return left.amount() > right.amount();
                        }
                    }
                    break;
                case BY_VAT: // 4
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
        // all criterion passed
        return left.order() < right.order();
    }

private:
    vector<pair<int, bool>> sort_level_;
};

// https://courses.fit.cvut.cz/BI-PA2/media/lectures/l06-stl-cz.pdf
struct CompareInvoices {
    // cmp for registerCompany
    bool operator()(const string &a, const string &b) const {
        string tmp1 = removeExtraSpaces(a);
        string tmp2 = removeExtraSpaces(b);
        return strcasecmp(tmp1.c_str(), tmp2.c_str()) < 0;
    }

    // cmp for CInvoices
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
};

class CVATRegister {
public:
    CVATRegister() = default;

    bool registerCompany(const string &name) {
        auto res = comp_.insert(name);

        // name already exists
        if (!res.second)
            return false;

        return true;
    }

    bool checkRegister(CInvoice &x) {
        // same seller and buyer name
        if (strcasecmp(x.seller_formated().c_str(), x.buyer_formated().c_str()) == 0) {
            return false;
        }

        // find "x" in sell
        auto sell_itr = comp_.find(x.seller_formated());
        if (sell_itr == comp_.end())
            return false;

        // find "x" in buy
        auto buy_itr = comp_.find(x.buyer_formated());
        if (buy_itr == comp_.end())
            return false;

        // save original name and registers order
        x.comp_.seller_ = *sell_itr;
        x.comp_.buyer_ = *buy_itr;
        x.comp_.order_ = register_order_;
        register_order_++;

        return true;
    }

    bool addIssued(const CInvoice &x) {
        CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()),
                     x.comp_.seller_, x.comp_.buyer_, x.amount_, x.vat_);

        if (!checkRegister(tmp))
            return false;

        auto res = sell_register_.insert(tmp);

        if (!res.second)
            return false;

        return true;
    }

    bool addAccepted(const CInvoice &x) {
        CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()),
                     x.comp_.seller_, x.comp_.buyer_, x.amount_, x.vat_);

        if (!checkRegister(tmp))
            return false;

        auto res = buy_register_.insert(tmp);

        if (!res.second)
            return false;

        return true;
    }

    bool delIssued(const CInvoice &x) {
        CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()),
                     x.comp_.seller_, x.comp_.buyer_, x.amount_, x.vat_);

        if (!checkRegister(tmp))
            return false;

        if (sell_register_.erase(tmp) == 0)
            return false;

        return true;
    }

    bool delAccepted(const CInvoice &x) {
        CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()),
                     x.comp_.seller_, x.comp_.buyer_, x.amount_, x.vat_);

        if (!checkRegister(tmp))
            return false;

        if (buy_register_.erase(tmp) == 0) {
            return false;
        }
        return true;
    }

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const {
        list<CInvoice> result;
        string company_format = removeExtraSpaces(company);

        // find all "company" matches in sell (that are not in buy)
        for (const auto &sell: sell_register_) {
            if (strcasecmp(company_format.c_str(), sell.seller_formated().c_str()) == 0 ||
                strcasecmp(company_format.c_str(), sell.buyer_formated().c_str()) == 0) {
                auto itr = buy_register_.find(sell);
                if (itr == buy_register_.end()) {
                    result.emplace_back(sell);
                }
            }
        }

        // find all "company" matches in buy (that are not in sell)
        for (const auto &buy: buy_register_) {
            if (strcasecmp(company_format.c_str(), buy.seller_formated().c_str()) == 0 ||
                strcasecmp(company_format.c_str(), buy.buyer_formated().c_str()) == 0) {
                auto itr = sell_register_.find(buy);
                if (itr == sell_register_.end()) {
                    result.emplace_back(buy);
                }
            }
        }

        result.sort(sortBy);

        return result;
    }

private:
    set<string, CompareInvoices> comp_;
    set<CInvoice, CompareInvoices> sell_register_;
    set<CInvoice, CompareInvoices> buy_register_;
    size_t register_order_;
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
    // testing 1 space and no space -> should be same name
    CVATRegister r2;
    assert(r2.registerCompany(""));
    assert(!r2.registerCompany(" "));
    assert(!r2.registerCompany("    "));
    assert(!r2.registerCompany(""));

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

    assert(r.addAccepted(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company ", 200, 30)));
    assert(r.addAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert(r.addAccepted(CInvoice(CDate(2000, 1, 1), "Second company ", "First Company", 300, 32)));

    cout << "\n\n\n";

    assert(equalLists(r.unmatched("First Company",
                                  CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                          CSortOpt::BY_DATE, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));

    assert(!r.delIssued(CInvoice(CDate(2001, 1, 1), "First Company", "Second Company ", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "A First Company", "Second Company ", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Hand", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 1200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 200, 130)));
    assert(r.delIssued(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company", 200, 30)));

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

    assert(r.delAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));

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

    assert(r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));

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

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */