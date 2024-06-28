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

class CInvoice {
public:
    CInvoice();

    CInvoice(const CDate &date, const string &seller,
             const string &buyer, unsigned int amount, double vat)
            : date_(date), seller_(seller), buyer_(buyer),
              amount_(amount), vat_(vat) {
        seller_format_ = removeExtraSpaces(seller);
        buyer_format_ = removeExtraSpaces(buyer);
    }

    CDate date() const {
        return date_;
    }

    string buyer() const {
        return buyer_;
    }

    string buyer_formated() const {
        return buyer_format_;
    }

    string seller() const {
        return seller_;
    }

    string seller_formated() const {
        return seller_format_;
    }

    unsigned int amount() const {
        return amount_;
    }

    double vat() const {
        return vat_;
    }

    int order() const {
        return order_;
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
    string seller_;
    string seller_format_;
    string buyer_;
    string buyer_format_;
    unsigned int amount_;
    double vat_;
    int order_;
};

// https://courses.fit.cvut.cz/BI-PA2/media/lectures/l06-stl-cz.pdf
struct CompareInvoices {
    // compare for registerCompany
    bool operator()(const string &a, const string &b) const {
        string tmp1 = removeExtraSpaces(a);
        string tmp2 = removeExtraSpaces(b);
        return strcasecmp(tmp1.c_str(), tmp2.c_str()) < 0;
    }

    // compare for CInvoices
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

// stores company information
struct Company {
    set<CInvoice, CompareInvoices> issued;
    set<CInvoice, CompareInvoices> accepted;

    Company() = default;
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

// custom string hash
struct StringHash {
    size_t operator()(const string &s) const {
        string a_format = removeExtraSpaces(s);
        // used for same ASCII values (a != A)
        // https://en.cppreference.com/w/cpp/algorithm/transform
        transform(a_format.begin(), a_format.end(), a_format.begin(), ::tolower);
        size_t hash = 0;
        for (char c: a_format) {
            hash = hash * 31 + c;
        }
        return hash;
    }
};

// compare function for map
struct StringHashCompare {
    bool operator()(const string &a, const string &b) const {
        string a_format = removeExtraSpaces(a);
        string b_format = removeExtraSpaces(b);
        // used for same ASCII values (a != A)
        // https://en.cppreference.com/w/cpp/algorithm/transform
        transform(a_format.begin(), a_format.end(), a_format.begin(), ::tolower);
        transform(b_format.begin(), b_format.end(), b_format.begin(), ::tolower);
        return strcmp(a_format.c_str(), b_format.c_str()) == 0;
    }
};

class CVATRegister {
public:
    CVATRegister() = default;

    bool registerCompany(const string &name) {
        auto itr = register_.find(name);
        if (itr != register_.end()) {
            return false;
        } else {
            Company tmp;
            register_.insert({name, tmp});
        }
        return true;
    }

    bool addIssued(const CInvoice &x) {
        // same selling name as buying name
        if (strcasecmp(x.seller_format_.c_str(), x.buyer_format_.c_str()) == 0) {
            return false;
        }

        // looking for SELLERs invoices
        auto itr = register_.find(x.seller());

        // seller name is not found in register
        if (itr != register_.end()) {
            Company &company = itr->second;

            CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()),
                         x.seller_, x.buyer_, x.amount_, x.vat_);

            tmp.seller_ = itr->first;
            tmp.seller_format_ = removeExtraSpaces(tmp.seller());
            auto find_buy = register_.find(tmp.buyer());
            tmp.buyer_ = find_buy->first;
            tmp.buyer_format_ = removeExtraSpaces(tmp.buyer());
            tmp.order_ = register_order_;
            register_order_++;

            // insert.second = true -> tmp already exists
            auto itr_sell = company.issued.insert(tmp);
            if (!itr_sell.second) {
                return false;
            }
        } else {
            return false;
        }

        // looking for BUYERs invoices
        auto itr1 = register_.find(x.buyer());

        // buyer name is not found in register
        if (itr1 != register_.end()) {
            Company &company = itr1->second;

            CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()),
                         x.seller_, x.buyer_, x.amount_, x.vat_);

            tmp.buyer_ = itr1->first;
            tmp.buyer_format_ = removeExtraSpaces(tmp.buyer());
            auto find_buy = register_.find(tmp.seller());
            tmp.seller_ = find_buy->first;
            tmp.seller_format_ = removeExtraSpaces(tmp.seller());
            tmp.order_ = register_order_;
            register_order_++;

            auto itr_sell = company.issued.insert(tmp);
            if (!itr_sell.second) {
                return false;
            }
        } else {
            return false;
        }

        return true;
    }

    bool addAccepted(const CInvoice &x) {
        // same selling name as buying name
        if (strcasecmp(x.seller_format_.c_str(), x.buyer_format_.c_str()) == 0) {
            return false;
        }

        // looking for SELLERs invoices
        auto itr = register_.find(x.seller());

        // seller name is not found in register
        if (itr != register_.end()) {
            Company &company = itr->second;

            CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()),
                         x.seller_, x.buyer_, x.amount_, x.vat_);

            tmp.seller_ = itr->first;
            tmp.seller_format_ = removeExtraSpaces(tmp.seller());
            auto find_buy = register_.find(tmp.buyer());
            tmp.buyer_ = find_buy->first;
            tmp.buyer_format_ = removeExtraSpaces(tmp.buyer());
            tmp.order_ = register_order_;
            register_order_++;

            auto itr_sell = company.accepted.insert(tmp);
            if (!itr_sell.second) {
                return false;
            }
        } else {
            return false;
        }

        // looking for BUYERs invoices
        auto itr1 = register_.find(x.buyer());

        // buyer name is not found in register
        if (itr1 != register_.end()) {
            Company &company = itr1->second;

            CInvoice tmp(CDate(x.date_.year(), x.date_.month(), x.date_.day()),
                         x.seller_, x.buyer_, x.amount_, x.vat_);

            tmp.buyer_ = itr1->first;
            tmp.buyer_format_ = removeExtraSpaces(tmp.buyer());
            auto find_buy = register_.find(tmp.seller());
            tmp.seller_ = find_buy->first;
            tmp.seller_format_ = removeExtraSpaces(tmp.seller());
            tmp.order_ = register_order_;
            register_order_++;

            auto itr_sell = company.accepted.insert(tmp);
            if (!itr_sell.second) {
                return false;
            }
        } else {
            return false;
        }

        return true;
    }

    bool delIssued(const CInvoice &x) {
        // same selling name as buying name
        if (strcasecmp(x.seller_format_.c_str(), x.buyer_format_.c_str()) == 0) {
            return false;
        }

        // looking for SELLERs invoices
        auto itr = register_.find(x.seller());

        // seller name is not found in register
        if (itr != register_.end()) {
            Company &company = itr->second;
            if (company.issued.erase(x) == 0) {
                return false;
            }
        } else {
            return false;
        }

        // looking for BUYERs invoices
        auto itr1 = register_.find(x.buyer());

        // buyer name is not found in register
        if (itr1 != register_.end()) {
            Company &company = itr1->second;
            if (company.issued.erase(x) == 0) {
                return false;
            }
        } else {
            return false;
        }
        return true;
    }

    bool delAccepted(const CInvoice &x) {
        // same selling name as buying name
        if (strcasecmp(x.seller_format_.c_str(), x.buyer_format_.c_str()) == 0) {
            return false;
        }

        // looking for SELLERs invoices
        auto itr = register_.find(x.buyer());

        // seller name is not found in register
        if (itr != register_.end()) {
            Company &company = itr->second;
            if (company.accepted.erase(x) == 0) {
                return false;
            }
        } else {
            return false;
        }

        // looking for BUYERs invoices
        auto itr1 = register_.find(x.seller());

        // buyer name is not found in register
        if (itr1 != register_.end()) {
            Company &company = itr1->second;
            if (company.accepted.erase(x) == 0) {
                return false;
            }
        } else {
            return false;
        }
        return true;
    }

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const {
        list<CInvoice> result;
        auto itr_sort = register_.find(company);

        // company not found in register
        if (itr_sort == register_.end()) {
            return result;
        }

        const Company &company_set = itr_sort->second;

        // looking for unique issued invoices
        for (const auto &sell: company_set.issued) {
            if (company_set.accepted.find(sell) == company_set.accepted.end()) {
                result.emplace_back(sell);
            }
        }

        // looking for unique accepted invoices
        for (const auto &buy: company_set.accepted) {
            if (company_set.issued.find(buy) == company_set.issued.end()) {
                result.emplace_back(buy);
            }
        }

        result.sort(sortBy);

        return result;
    }

private:
    // name -> hash, Company -> set<issued>, set<accepted>
    unordered_map<string, Company, StringHash, StringHashCompare> register_;
    int register_order_;
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