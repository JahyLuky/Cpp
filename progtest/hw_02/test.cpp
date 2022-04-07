#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

/** Class represents control report database*/
class CVATRegister {
public:
    // Constructor
    CVATRegister(void);

    CVATRegister(const string &name, const string &addr);

    CVATRegister(const string &taxID);

    // Destructor
    ~CVATRegister(void);

    /**
    * Creates new comapny and adds it to database.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    * @param[in] string_taxID ID of company.
    *
    * @return 1 success, 0 otherwise.
    */
    bool newCompany(const string &name, const string &addr, const string &taxID);

    /**
    * Deletes comapny from database.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return 1 if success, 0 otherwise.
    */
    bool cancelCompany(const string &name, const string &addr);

    /**
    * Deletes comapny from database.
    *
    * @param[in] string_taxID ID of comapny.
    *
    * @return 1 if success, 0 otherwise.
    */
    bool cancelCompany(const string &taxID);

    /**
    * Checks comapny's invoice.
    *
    * @param[in] string_taxID ID of comapny.
    *
    * @return 1 if success, 0 otherwise.
    */
    bool invoice(const string &taxID, unsigned int amount);

    /**
    * Checks comapny's invoice.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return 1 if success, 0 otherwise.
    */
    bool invoice(const string &name, const string &addr, unsigned int amount);

    /**
    * Finds sum of comapny's invoices.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return 1 if success, 0 otherwise.
    */
    bool audit(const string &name, const string &addr, unsigned int &sumIncome) const;

    /**
    * Finds sum of comapny's invoices.
    *
    * @param[in] string_taxID ID of comapny.
    *
    * @return 1 if success, 0 otherwise.
    */
    bool audit(const string &taxID, unsigned int &sumIncome) const;

    /**
    * Finds first comapny in database.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return 1 if success, 0 otherwise.
    */
    bool firstCompany(string &name, string &addr) const;

    /**
    * Finds next company that follows the parameters in database.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return 1 if success, 0 otherwise.
    */
    bool nextCompany(string &name, string &addr) const;

    /**
    * Finds median of comapny's invoices.
    *
    * @return median
    */
    unsigned int medianInvoice(void) const;


private:
    struct Company {
        string m_name;
        string m_address;
        string m_id;
        unsigned int m_invoice;

        bool operator!=(const Company &a) const {
            int compName = strcasecmp(a.m_name.c_str(), m_name.c_str());
            int compAddr = strcasecmp(a.m_address.c_str(), m_address.c_str());
            int compID = strcmp(a.m_id.c_str(), m_id.c_str());

            if (compName == 0) {
                if (compAddr == 0) {
                    return false;
                }
            }
            if (compID == 0) {
                return false;
            }
            return true;
        }
    };

    struct compare {
        bool operator()(const Company &a, const Company &b) const {
            int cmp = strcasecmp(a.m_name.c_str(), b.m_name.c_str());
            int addr = strcasecmp(a.m_address.c_str(), b.m_address.c_str());

            if (cmp == 0) {
                if (addr == 0)
                    return false;
                else
                    return addr < 0;
            }
            return cmp < 0;
        }
    };

    struct compare_id {
        bool operator()(const Company &a, const Company &b) const {
            return strcmp(a.m_id.c_str(), b.m_id.c_str()) < 0;
        }
    };

    vector<Company> m_id_sort;
    vector<Company> m_name_sort;
    vector<unsigned int> m_invoice;
};

CVATRegister::CVATRegister(void) {
}

CVATRegister::~CVATRegister(void) {}

bool CVATRegister::cancelCompany(const string &name, const string &addr) {
    Company isThere;
    isThere.m_name = name;
    isThere.m_address = addr;
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), isThere, compare());

    if (posName == m_name_sort.end() || *posName != isThere)
        return false;

    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), (*posName), compare_id());
    m_name_sort.erase(posName);
    m_id_sort.erase(posID);

    return true;
}

bool CVATRegister::cancelCompany(const string &taxID) {
    Company isThere;
    isThere.m_id = taxID;
    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), isThere, compare_id());

    if (posID == m_id_sort.end() || *posID != isThere)
        return false;

    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), (*posID), compare());
    m_name_sort.erase(posName);
    m_id_sort.erase(posID);

    return true;
}

bool CVATRegister::newCompany(const string &name, const string &addr, const string &taxID) {
    // TODO: udelat konstruktor
    Company isThere;
    isThere.m_name = name;
    isThere.m_address = addr;
    isThere.m_id = taxID;
    isThere.m_invoice = 0;
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), isThere, compare());

    if (posName == m_name_sort.end() || *posName != isThere) {
        auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), isThere, compare_id());

        if (posID == m_id_sort.end() || (*posID != isThere)) {
            m_name_sort.insert(posName, isThere);
            m_id_sort.insert(posID, isThere);
            return true;
        }
    }

    return false;
}

bool CVATRegister::invoice(const string &name, const string &addr, unsigned int amount) {
    Company isThere;
    isThere.m_name = name;
    isThere.m_address = addr;
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), isThere, compare());

    if (posName == m_name_sort.end() || *posName != isThere)
        return false;

    m_invoice.push_back(amount);
    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), (*posName), compare_id());
    (*posName).m_invoice += amount;
    (*posID).m_invoice += amount;

    return true;
}

bool CVATRegister::invoice(const string &taxID, unsigned int amount) {
    Company isThere;
    isThere.m_id = taxID;
    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), isThere, compare_id());

    if (posID == m_id_sort.end() || *posID != isThere)
        return false;

    m_invoice.push_back(amount);
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), (*posID), compare());
    (*posName).m_invoice += amount;
    (*posID).m_invoice += amount;

    return true;
}

bool CVATRegister::audit(const string &name, const string &addr, unsigned int &sumIncome) const {
    Company isThere;
    isThere.m_name = name;
    isThere.m_address = addr;
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), isThere, compare());

    if (posName == m_name_sort.end() || *posName != isThere)
        return false;

    sumIncome = (*posName).m_invoice;

    return true;
}

bool CVATRegister::audit(const string &taxID, unsigned int &sumIncome) const {
    Company isThere;
    isThere.m_id = taxID;
    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), isThere, compare_id());

    if (posID == m_id_sort.end() || *posID != isThere)
        return false;

    sumIncome = (*posID).m_invoice;

    return true;
}

bool CVATRegister::firstCompany(string &name, string &addr) const {
    if (m_name_sort.empty())
        return false;

    name = m_name_sort[0].m_name;
    addr = m_name_sort[0].m_address;

    return true;
}

bool CVATRegister::nextCompany(string &name, string &addr) const {
    Company tmp;
    tmp.m_name = name;
    tmp.m_address = addr;
    auto pos = lower_bound(m_name_sort.begin(), m_name_sort.end(), tmp, compare());

    if (pos == m_name_sort.end() || (++pos) == m_name_sort.end())
        return false;

    name = (*pos).m_name;
    addr = (*pos).m_address;

    return true;
}

unsigned int CVATRegister::medianInvoice(void) const {
    if (m_invoice.empty())
        return 0;

    vector<unsigned int> tmp;
    size_t count = m_invoice.size();

    for (size_t i = 0; i < count; i++)
        tmp.push_back(m_invoice[i]);

    sort(tmp.begin(), tmp.end());

    unsigned int median = count / 2;

    return tmp[median];
}

#ifndef __PROGTEST__

int main(void) {
    string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;

    assert (b1.newCompany("ACME", "Thakurova", "666/666"));
    assert (b1.newCompany("ACME", "Kolejni", "666/666/666"));
    assert (b1.newCompany("Dummy", "Thakurova", "123456"));
    assert (b1.invoice("666/666", 2000));
    assert (b1.medianInvoice() == 2000);
    assert (b1.invoice("666/666/666", 3000));
    assert (b1.medianInvoice() == 3000);
    assert (b1.invoice("123456", 4000));
    assert (b1.medianInvoice() == 3000);
    assert (b1.invoice("aCmE", "Kolejni", 5000));
    assert (b1.medianInvoice() == 4000);
    assert (b1.audit("ACME", "Kolejni", sumIncome) && sumIncome == 8000);
    assert (b1.audit("123456", sumIncome) && sumIncome == 4000);
    assert (b1.firstCompany(name, addr) && name == "ACME" && addr == "Kolejni");
    assert (b1.nextCompany(name, addr) && name == "ACME" && addr == "Thakurova");
    assert (b1.nextCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
    assert (!b1.nextCompany(name, addr));
    assert (b1.cancelCompany("ACME", "KoLeJnI"));
    assert (b1.medianInvoice() == 4000);
    assert (b1.cancelCompany("666/666"));
    assert (b1.medianInvoice() == 4000);
    assert (b1.invoice("123456", 100));
    assert (b1.medianInvoice() == 3000);
    assert (b1.invoice("123456", 300));
    assert (b1.medianInvoice() == 3000);
    assert (b1.invoice("123456", 200));
    assert (b1.medianInvoice() == 2000);
    assert (b1.invoice("123456", 230));
    assert (b1.medianInvoice() == 2000);
    assert (b1.invoice("123456", 830));
    assert (b1.medianInvoice() == 830);
    assert (b1.invoice("123456", 1830));
    assert (b1.medianInvoice() == 1830);
    assert (b1.invoice("123456", 2830));
    assert (b1.medianInvoice() == 1830);
    assert (b1.invoice("123456", 2830));
    assert (b1.medianInvoice() == 2000);
    assert (b1.invoice("123456", 3200));
    assert (b1.medianInvoice() == 2000);
    assert (b1.firstCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
    assert (!b1.nextCompany(name, addr));
    assert (b1.cancelCompany("123456"));
    assert (!b1.firstCompany(name, addr));

    CVATRegister b2;
    assert (b2.newCompany("ACME", "Kolejni", "abcdef"));
    assert (b2.newCompany("Dummy", "Kolejni", "123456"));
    assert (!b2.newCompany("AcMe", "kOlEjNi", "1234"));
    assert (b2.newCompany("Dummy", "Thakurova", "ABCDEF"));
    assert (b2.medianInvoice() == 0);
    assert (b2.invoice("ABCDEF", 1000));
    assert (b2.medianInvoice() == 1000);
    assert (b2.invoice("abcdef", 2000));
    assert (b2.medianInvoice() == 2000);
    assert (b2.invoice("aCMe", "kOlEjNi", 3000));
    assert (b2.medianInvoice() == 2000);
    assert (!b2.invoice("1234567", 100));
    assert (!b2.invoice("ACE", "Kolejni", 100));
    assert (!b2.invoice("ACME", "Thakurova", 100));
    assert (!b2.audit("1234567", sumIncome));
    assert (!b2.audit("ACE", "Kolejni", sumIncome));
    assert (!b2.audit("ACME", "Thakurova", sumIncome));
    assert (!b2.cancelCompany("1234567"));
    assert (!b2.cancelCompany("ACE", "Kolejni"));
    assert (!b2.cancelCompany("ACME", "Thakurova"));
    assert (b2.cancelCompany("abcdef"));
    assert (b2.medianInvoice() == 2000);
    assert (!b2.cancelCompany("abcdef"));
    assert (b2.newCompany("ACME", "Kolejni", "abcdef"));
    assert (b2.cancelCompany("ACME", "Kolejni"));
    assert (!b2.cancelCompany("ACME", "Kolejni"));


    return 0;
}

#endif /* __PROGTEST__ */
