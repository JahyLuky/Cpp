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

    Company() = default;

    Company(const string &seller, const string &buyer)
            : seller_(seller), buyer_(buyer),
              seller_format_(removeExtraSpaces(seller)),
              buyer_format_(removeExtraSpaces(buyer)) {}
};

class CInvoice {
public:
    CInvoice(const CDate &date, const string &seller,
             const string &buyer, unsigned int amount, double vat)
            : date_(date), comp_(seller, buyer),
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

private:
    CDate date_;
    Company comp_;
    unsigned int amount_;
    double vat_;
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
        return *this;
    }

private:
    // todo
};

// https://courses.fit.cvut.cz/BI-PA2/media/lectures/l06-stl-cz.pdf
struct CompareInvoices {
    bool operator()(const string &a, const string &b) const {
        return strcasecmp(a.c_str(), b.c_str()) < 0;
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
        string tmp = removeExtraSpaces(name);
        auto res = comp_.insert(tmp);
        if (!res.second) {
            return false;
        }
        return true;
    }

    bool checkRegistr(const CInvoice &x) const {
        if (strcasecmp(x.seller_formated().c_str(), x.buyer_formated().c_str()) == 0) {
            return false;
        }

        auto itr1 = lower_bound(comp_.begin(),
                                comp_.end(),
                                x.seller_formated(), CompareInvoices());

        if (itr1 == comp_.end() ||
            strcasecmp((*itr1).c_str(), x.seller_formated().c_str()) != 0) {
            return false;
        }

        auto itr2 = lower_bound(comp_.begin(),
                                comp_.end(),
                                x.buyer_formated(), CompareInvoices());

        if (itr2 == comp_.end() ||
            strcasecmp((*itr2).c_str(), x.buyer_formated().c_str()) != 0) {
            return false;
        }

        return true;
    }

    bool addIssued(const CInvoice &x) {
        if (!checkRegistr(x))
            return false;

        auto res = sell_registry_.insert(x);
        if (!res.second) {
            return false;
        }
        return true;
    }

    bool addAccepted(const CInvoice &x) {
        if (!checkRegistr(x))
            return false;

        auto res = buy_registry_.insert(x);
        if (!res.second) {
            return false;
        }
        return true;
    }

    bool delIssued(const CInvoice &x) {
        if (!checkRegistr(x))
            return false;

        auto itr = sell_registry_.find(x);
        if (itr != sell_registry_.end())
            cout << "nasel jsem\n";
        else
            cout << "smula\n";

        if (sell_registry_.erase(x) == 0) {
            return false;
        }
        return true;
    }

    bool delAccepted(const CInvoice &x) {
        if (!checkRegistr(x))
            return false;

        if (buy_registry_.erase(x) == 0) {
            return false;
        }
        return true;
    }

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const {
        list<CInvoice> sorted;
        string company_format = removeExtraSpaces(company);

        auto itr_sell = lower_bound(sell_registry_.begin(),
                                    sell_registry_.end(),
                                    company_format, CompareInvoices());

        auto itr_buy = lower_bound(buy_registry_.begin(),
                                   buy_registry_.end(),
                                   company_format, CompareInvoices());

        if (itr_sell == sell_registry_.end() || (*itr_sell).seller_formated() != company_format)
            cout << "sell end\n";

        if (itr_buy == buy_registry_.end() || (*itr_buy).buyer_formated() != company_format)
            cout << "buy end\n";


        return sorted;
    }

private:
    set<string, CompareInvoices> comp_;
    set<CInvoice, CompareInvoices> sell_registry_;
    set<CInvoice, CompareInvoices> buy_registry_;
};

#ifndef __PROGTEST__

bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
    if (a.size() != b.size())
        return false;

    for (const auto &tmp1: a) {
        for (const auto &tmp2: b) {
            cout << tmp1.seller_formated() << " == " << tmp2.seller_formated() << endl;
            cout << tmp1.buyer_formated() << " == " << tmp2.buyer_formated() << endl;
            cout << tmp1.amount() << " == " << tmp2.amount() << endl;
            cout << tmp1.vat() << " == " << tmp2.vat() << endl;
            cout << "-----------------" << endl;
            if (tmp1 != tmp2) {
                return false;
            }
        }
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
/*
    assert(equalLists(r.unmatched("First Company",
                                  CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, false).addKey(
                                          CSortOpt::BY_DATE, false)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
    */
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