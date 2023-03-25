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

private:
    /**
    * Struct that holds information about companies..
    */
    struct Company {
        string m_name;
        string m_address;
        string m_id;
        unsigned int m_invoice;

        Company(const string &mName, const string &mAddress, const string &mId, unsigned int mInvoice) :
        m_name(mName),m_address(mAddress),m_id(mId),m_invoice(mInvoice) {}

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

    struct compare_name {
        bool operator()(const Company &a, const Company &b) const {
            int name = strcasecmp(a.m_name.c_str(), b.m_name.c_str());
            int addr = strcasecmp(a.m_address.c_str(), b.m_address.c_str());

            if (name == 0) {
                if (addr == 0)
                    return false;
                else
                    return addr < 0;
            }
            return name < 0;
        }
    };

    struct compare_id {
        bool operator()(const Company &a, const Company &b) const {
            return strcmp(a.m_id.c_str(), b.m_id.c_str()) < 0;
        }
    };

    // stores sorted database by id
    vector<Company> m_id_sort;
    // stores sorted database by name, if same, sort by address
    vector<Company> m_name_sort;
    // stores invoices
    vector<unsigned int> m_invoice;

public:
    // Constructor
    CVATRegister();

    // Destructor
    ~CVATRegister();

    /**
    * Creates new comapny and adds it to database.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    * @param[in] string_taxID ID of company.
    *
    * @return true success, false otherwise.
    */
    bool newCompany(const string &name, const string &addr, const string &taxID);

    /**
    * Deletes comapny from database.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return true if success, false otherwise.
    */
    bool cancelCompany(const string &name, const string &addr);

    /**
    * Deletes comapny from database.
    *
    * @param[in] string_taxID ID of comapny.
    *
    * @return true if success, false otherwise.
    */
    bool cancelCompany(const string &taxID);

    /**
    * Checks comapny's invoice.
    *
    * @param[in] string_taxID ID of comapny.
    *
    * @return true if success, false otherwise.
    */
    bool invoice(const string &taxID, unsigned int amount);

    /**
    * Checks comapny's invoice.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return true if success, false otherwise.
    */
    bool invoice(const string &name, const string &addr, unsigned int amount);

    /**
    * Finds sum of comapny's invoices.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return true if success, false otherwise.
    */
    bool audit(const string &name, const string &addr, unsigned int &sumIncome) const;

    /**
    * Finds sum of comapny's invoices.
    *
    * @param[in] string_taxID ID of comapny.
    *
    * @return true if success, false otherwise.
    */
    bool audit(const string &taxID, unsigned int &sumIncome) const;

    /**
    * Finds first comapny in database.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return true if success, false otherwise.
    */
    bool firstCompany(string &name, string &addr) const;

    /**
    * Finds next company that follows the parameters in database.
    *
    * @param[in] string_name Name of comapny.
    * @param[in] string_addr Address of company.
    *
    * @return true if success, false otherwise.
    */
    bool nextCompany(string &name, string &addr) const;

    /**
    * Finds median of comapny's invoices.
    *
    * @return median
    */
    unsigned int medianInvoice() const;

    /**
    * Searches if company is in database by name.
    *
    * @return true if company is found
    */
    static bool findCompany_name(const vector<Company>::iterator &pos_Name,
                                 const Company &toFind,
                                 const vector<Company> &name_sort);

    /**
    * Searches if company is in database by ID.
    *
    * @return true if company is found, false otherwise
    */
    static bool findCompany_ID(const vector<Company>::iterator &pos_ID, const Company &toFind,
                               const vector<Company> &id_sort);
};


CVATRegister::CVATRegister() = default;

CVATRegister::~CVATRegister() = default;

bool CVATRegister::findCompany_name(const vector<Company>::iterator &pos_Name,
                                    const Company &toFind,
                                    const vector<Company> &name_sort) {
    if (pos_Name == name_sort.end() || *pos_Name != toFind)
        return false;
    return true;
}

bool CVATRegister::findCompany_ID(const vector<Company>::iterator &pos_ID, const Company &toFind,
                                  const vector<Company> &id_sort) {
    if (pos_ID == id_sort.end() || *pos_ID != toFind)
        return false;
    return true;
}

bool CVATRegister::cancelCompany(const string &name, const string &addr) {
    Company toFind(name,addr,"",0);
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), toFind, compare_name());

    if (!findCompany_name(posName, toFind, m_name_sort))
        return false;

    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), (*posName), compare_id());
    m_name_sort.erase(posName);
    m_id_sort.erase(posID);

    return true;
}

bool CVATRegister::cancelCompany(const string &taxID) {
    Company toFind("","",taxID,0);
    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), toFind, compare_id());

    if (!findCompany_ID(posID, toFind, m_id_sort))
        return false;

    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), (*posID), compare_name());
    m_name_sort.erase(posName);
    m_id_sort.erase(posID);

    return true;
}

bool CVATRegister::newCompany(const string &name, const string &addr, const string &taxID) {
    Company toFind(name,addr,taxID,0);
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), toFind, compare_name());

    if (!findCompany_name(posName, toFind, m_name_sort)) {
        auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), toFind, compare_id());

        if (!findCompany_ID(posID, toFind, m_id_sort)) {
            m_name_sort.insert(posName, toFind);
            m_id_sort.insert(posID, toFind);
            return true;
        }
    }

    return false;
}

bool CVATRegister::invoice(const string &name, const string &addr, unsigned int amount) {
    Company toFind(name,addr,"",0);
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), toFind, compare_name());

    if (!findCompany_name(posName, toFind, m_name_sort))
        return false;

    m_invoice.push_back(amount);
    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), (*posName), compare_id());
    (*posName).m_invoice += amount;
    (*posID).m_invoice += amount;

    return true;
}

bool CVATRegister::invoice(const string &taxID, unsigned int amount) {
    Company toFind("","",taxID,0);
    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), toFind, compare_id());

    if (!findCompany_ID(posID, toFind, m_id_sort))
        return false;

    m_invoice.push_back(amount);
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), (*posID), compare_name());
    (*posName).m_invoice += amount;
    (*posID).m_invoice += amount;

    return true;
}

bool CVATRegister::audit(const string &name, const string &addr, unsigned int &sumIncome) const {
    Company toFind(name,addr,"",0);
    auto posName = lower_bound(m_name_sort.begin(), m_name_sort.end(), toFind, compare_name());

    if (posName == m_name_sort.end() || *posName != toFind)
        return false;

    sumIncome = (*posName).m_invoice;

    return true;
}

bool CVATRegister::audit(const string &taxID, unsigned int &sumIncome) const {
    Company toFind("","",taxID,0);
    auto posID = lower_bound(m_id_sort.begin(), m_id_sort.end(), toFind, compare_id());

    if (posID == m_id_sort.end() || *posID != toFind)
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
    Company tmp(name,addr,"",0);
    auto pos = lower_bound(m_name_sort.begin(), m_name_sort.end(), tmp, compare_name());

    if (pos == m_name_sort.end() || (++pos) == m_name_sort.end())
        return false;

    name = (*pos).m_name;
    addr = (*pos).m_address;

    return true;
}

unsigned int CVATRegister::medianInvoice() const {
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

int main() {
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

    CVATRegister test;
    assert (test.newCompany("AHOJ", "cau", "xyz"));
    assert (!test.newCompany("ahoj", "CAU", "xyzQ"));
    assert (test.medianInvoice() == 0);
    assert (test.invoice("xyz", 666));
    assert (test.invoice("xyz", 666));
    assert (test.invoice("xyz", 1));
    assert (test.invoice("xyz", 1));
    assert (test.medianInvoice() == 666);
    assert (test.invoice("xyz", 1));
    assert (test.medianInvoice() == 1);
    assert (test.cancelCompany("aHoJ", "Cau"));
    assert (!test.cancelCompany("nic", "neni"));

    return 0;
}

#endif /* __PROGTEST__ */
