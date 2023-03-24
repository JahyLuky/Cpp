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
#include <functional>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

// class represents Employee records
class CPersonalAgenda {
private:
    // struct that holds information about employees
    struct Person {
        string name_;
        string sur_;
        string email_;
        unsigned int salary_;

        Person(const string &name, const string &sur,
               const string &email, unsigned int salary) :
                name_(name), sur_(sur), email_(email), salary_(salary) {}

        bool operator!=(const Person &a) const {
            int name_cmp = strcmp(a.name_.c_str(), name_.c_str());
            int sur_cmp = strcmp(a.sur_.c_str(), sur_.c_str());
            int email_cmp = strcmp(a.email_.c_str(), email_.c_str());

            if (sur_cmp == 0) {
                if (name_cmp == 0)
                    return false;
            }
            if (email_cmp == 0)
                return false;
            return true;
        }
    };

    // compares 2 Persons by surnames, by names if surnames are equal
    struct compare_name_sur {
        bool operator()(const Person &a, const Person &b) const {
            int name_cmp = strcmp(a.name_.c_str(), b.name_.c_str());
            int sur_cmp = strcmp(a.sur_.c_str(), b.sur_.c_str());

            if (sur_cmp == 0) {
                if (name_cmp == 0)
                    return false;
                else
                    return name_cmp < 0;
            }
            return sur_cmp < 0;
        }
    };

    // compares 2 Persons by emails
    struct compare_email {
        bool operator()(const Person &a, const Person &b) const {
            return strcmp(a.email_.c_str(), b.email_.c_str()) < 0;
        }
    };

    // store database of employees sorted by surnames (by names if surnames are equal)
    vector<Person> name_sur_sort_;
    // store database of employees sorted by emails
    vector<Person> email_sort_;

public:
    CPersonalAgenda(void) = default;

    ~CPersonalAgenda(void) = default;

    static bool findPerson_name_sur(const vector<Person>::iterator &itr_ns,
                                    const vector<Person> &name_sur_sort,
                                    const Person &emp) {
        if (itr_ns == name_sur_sort.end() || (*itr_ns) != emp)
            return false;
        return true;
    }

    static bool findPerson_email(const vector<Person>::iterator &itr_e,
                                 const vector<Person> &email_sort,
                                 const Person &emp) {
        if (itr_e == email_sort.end() || (*itr_e) != emp)
            return false;
        return true;
    }

    bool add(const string &name, const string &surname, const string &email, unsigned int salary) {
        Person emp(name, surname, email, salary);
        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  emp, compare_name_sur());
        // name and surname already exists
        if (findPerson_name_sur(itr_ns, name_sur_sort_, emp))
            return false;

        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 emp, compare_email());

        // email already exists
        if (findPerson_email(itr_e, email_sort_, emp))
            return false;

        name_sur_sort_.insert(itr_ns, emp);
        email_sort_.insert(itr_e, emp);

        return true;
    }

    bool del(const string &name, const string &surname) {
        Person emp(name, surname, "", 0);
        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  emp, compare_name_sur());

        // name and surname not found
        if (!findPerson_name_sur(itr_ns, name_sur_sort_, emp))
            return false;

        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 (*itr_ns), compare_email());

        name_sur_sort_.erase(itr_ns);
        email_sort_.erase(itr_e);

        return true;
    }

    bool del(const string &email) {
        Person emp("", "", email, 0);
        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 emp, compare_name_sur());

        // email not found
        if (!findPerson_name_sur(itr_e, email_sort_, emp))
            return false;

        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  (*itr_e), compare_email());

        name_sur_sort_.erase(itr_ns);
        email_sort_.erase(itr_e);

        return true;
    }

    bool changeName(const string &email, const string &newName, const string &newSurname) {
        Person emp("", "", email, 0);
        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 emp, compare_email());

        // email not found
        if (!findPerson_email(itr_e, email_sort_, emp))
            return false;

        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  (*itr_e), compare_name_sur());

        unsigned int newSalary = (*itr_ns).salary_;
        string newEmail = (*itr_ns).email_;

        // check if person with 'newName' and 'newSurname' already exists
        Person tmp(newName, newSurname, newEmail, newSalary);
        auto itr_tmp = lower_bound(name_sur_sort_.begin(),
                                   name_sur_sort_.end(),
                                   tmp, compare_name_sur());

        // newName and newSurname already exists
        if (findPerson_name_sur(itr_tmp, name_sur_sort_, tmp))
            return false;

        name_sur_sort_.erase(itr_ns);
        email_sort_.erase(itr_e);

        auto itr_tmp1 = lower_bound(name_sur_sort_.begin(),
                                   name_sur_sort_.end(),
                                   tmp, compare_name_sur());

        auto itr_tmp2 = lower_bound(email_sort_.begin(),
                                    email_sort_.end(),
                                    tmp, compare_email());

        name_sur_sort_.insert(itr_tmp1, tmp);
        email_sort_.insert(itr_tmp2, tmp);

        return true;
    }

    bool changeEmail(const string &name, const string &surname, const string &newEmail) {
        Person emp(name, surname, "", 0);
        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  emp, compare_name_sur());

        // person not found
        if (!findPerson_name_sur(itr_ns, name_sur_sort_, emp))
            return false;

        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 (*itr_ns), compare_email());

        unsigned int newSalary = (*itr_e).salary_;
        string newName = (*itr_e).name_;
        string newSurname = (*itr_e).sur_;

        // check if person with 'newEmail' already exists
        Person tmp(newName, newSurname, newEmail, newSalary);
        auto itr_tmp = lower_bound(email_sort_.begin(),
                                   email_sort_.end(),
                                   tmp, compare_email());


        // newEmail already exists
        if (findPerson_email(itr_tmp, email_sort_, tmp))
            return false;


        name_sur_sort_.erase(itr_ns);
        email_sort_.erase(itr_e);

        auto itr_tmp1 = lower_bound(email_sort_.begin(),
                                   email_sort_.end(),
                                   tmp, compare_email());

        auto itr_tmp2 = lower_bound(name_sur_sort_.begin(),
                                    name_sur_sort_.end(),
                                    tmp, compare_name_sur());

        email_sort_.insert(itr_tmp1, tmp);
        name_sur_sort_.insert(itr_tmp2, tmp);

        return true;
    }

    bool setSalary(const string &name, const string &surname, unsigned int salary) {
        Person emp(name, surname, "", 0);
        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  emp, compare_name_sur());

        // name and surname not found
        if (!findPerson_name_sur(itr_ns, name_sur_sort_, emp))
            return false;

        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 (*itr_ns), compare_email());
        (*itr_ns).salary_ = salary;
        (*itr_e).salary_ = salary;

        return true;
    }

    bool setSalary(const string &email, unsigned int salary) {
        Person emp("", "", email, 0);
        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 emp, compare_email());

        // email not found
        if (!findPerson_name_sur(itr_e, email_sort_, emp))
            return false;

        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  (*itr_e), compare_name_sur());
        (*itr_e).salary_ = salary;
        (*itr_ns).salary_ = salary;

        return true;
    }

    unsigned int getSalary(const string &name, const string &surname) const {
        Person emp(name, surname, "", 0);
        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  emp, compare_name_sur());

        // name and surname not found
        if (itr_ns == name_sur_sort_.end() || (*itr_ns) != emp)
            return false;

        return (*itr_ns).salary_;
    }

    unsigned int getSalary(const string &email) const {
        Person emp("", "", email, 0);
        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 emp, compare_email());

        // email not found
        if (itr_e == email_sort_.end() || (*itr_e) != emp)
            return false;

        return (*itr_e).salary_;
        return true;
    }

    bool getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
        Person emp(name, surname, "", 0);
        auto itr_ns = lower_bound(name_sur_sort_.begin(),
                                  name_sur_sort_.end(),
                                  emp, compare_name_sur());

        // name and surname not found
        if (itr_ns == name_sur_sort_.end() || (*itr_ns) != emp)
            return false;

        int min = 0, max = 0;
        for (const auto &item: name_sur_sort_) {
            if (item.salary_ < (*itr_ns).salary_)
                min++;
            if (item.salary_ == (*itr_ns).salary_)
                max++;
        }
        rankMin = min;
        rankMax = max + min - 1;

        return true;
    }

    bool getRank(const string &email, int &rankMin, int &rankMax) const {
        Person emp("", "", email, 0);
        auto itr_e = lower_bound(email_sort_.begin(),
                                 email_sort_.end(),
                                 emp, compare_email());

        // email not found
        if (itr_e == email_sort_.end() || (*itr_e) != emp)
            return false;

        int min = 0, max = 0;
        for (const auto &item: email_sort_) {
            if (item.salary_ < (*itr_e).salary_)
                min++;
            if (item.salary_ == (*itr_e).salary_)
                max++;
        }
        rankMin = min;
        rankMax = max + min - 1;

        return true;
    }

    bool getFirst(string &outName, string &outSurname) const {
        if (name_sur_sort_.empty())
            return false;

        outName = name_sur_sort_[0].name_;
        outSurname = name_sur_sort_[0].sur_;

        return true;
    }

    bool getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
        Person emp(name, surname, "", 0);
        auto itr = lower_bound(name_sur_sort_.begin(),
                               name_sur_sort_.end(),
                               emp, compare_name_sur());

        // name and surname not found
        if (itr == name_sur_sort_.end() || (++itr) == name_sur_sort_.end())
            return false;

        outName = (*itr).name_;
        outSurname = (*itr).sur_;

        return true;
    }
};

#ifndef __PROGTEST__

int main(void) {
    string outName, outSurname;
    int lo, hi;

    // custom tests
    CPersonalAgenda b0;
    assert (b0.add("John", "Smith", "john", 3));

    assert (!b0.getNext("John", "Smith", outName, outSurname));

    assert (b0.add("aA", "a", "dD", 3));
    assert (b0.add("Aa", "a", "Dd", 3));
    assert (b0.add("a", "a", "a", 3));
    assert (b0.add("b", "A", "c", 2));

    assert (!b0.add("John", "Smith", "a", 30001));
    assert (!b0.add("b", "a", "john", 30001));
    assert (!b0.add("John", "Smith", "a", 30001));
    assert (!b0.add("b", "a", "john", 30001));
    assert (!b0.add("a", "b", "john", 30001));

    CPersonalAgenda b3;
    // diff sur
    assert (b3.add("a", "c", "john1", 3));
    assert (b3.add("a", "a", "john2", 3));
    assert (b3.add("a", "b", "john3", 3));
    assert (b3.getFirst(outName, outSurname)
            && outName == "a"
            && outSurname == "a");
    assert (b3.getNext("a", "a", outName, outSurname)
            && outName == "a"
            && outSurname == "b");
    assert (b3.getNext("a", "b", outName, outSurname)
            && outName == "a"
            && outSurname == "c");

    CPersonalAgenda b4;
    // same surr, diff name
    assert (b4.add("b", "a", "john3", 3));
    assert (b4.add("c", "a", "john4", 3));
    assert (b4.add("a", "a", "john5", 3));
    assert (b4.getFirst(outName, outSurname)
            && outName == "a"
            && outSurname == "a");
    assert (b4.getNext("a", "a", outName, outSurname)
            && outName == "b"
            && outSurname == "a");
    assert (b4.getNext("b", "a", outName, outSurname)
            && outName == "c"
            && outSurname == "a");

    CPersonalAgenda b1;
    assert (b1.add("John", "Smith", "john", 30000));
    assert (b1.add("John", "Miller", "johnm", 35000));
    assert (b1.add("Peter", "Smith", "peter", 23000));
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");

    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");

    assert (b1.getNext("John", "Smith", outName, outSurname)
            && outName == "Peter"
            && outSurname == "Smith");
    assert (!b1.getNext("Peter", "Smith", outName, outSurname));

    assert (b1.setSalary("john", 32000));
    assert (b1.getSalary("john") == 32000);
    assert (b1.getSalary("John", "Smith") == 32000);

    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("johnm", lo, hi)
            && lo == 2
            && hi == 2);
    assert (b1.setSalary("John", "Smith", 35000));
    assert (b1.getSalary("John", "Smith") == 35000);
    assert (b1.getSalary("john") == 35000);
    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("johnm", lo, hi)
            && lo == 1
            && hi == 2);

    assert (b1.changeName("peter", "James", "Bond"));
    assert (b1.getSalary("peter") == 23000);
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("Peter", "Smith") == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "James"
            && outSurname == "Bond");
    assert (b1.getNext("James", "Bond", outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));

    assert (b1.changeEmail("James", "Bond", "james"));
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("james") == 23000);
    assert (b1.getSalary("peter") == 0);

    assert (b1.del("james"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 1);
    assert (b1.del("John", "Miller"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));
    assert (b1.del("john"));
    assert (!b1.getFirst(outName, outSurname));
    assert (b1.add("John", "Smith", "john", 31000));
    assert (b1.add("john", "Smith", "joHn", 31000));
    assert (b1.add("John", "smith", "jOhn", 31000));


    CPersonalAgenda b2;
    assert (!b2.getFirst(outName, outSurname));
    assert (b2.add("James", "Bond", "james", 70000));
    assert (b2.add("James", "Smith", "james2", 30000));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (!b2.add("James", "Bond", "james3", 60000));
    assert (!b2.add("Peter", "Bond", "peter", 50000));
    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.setSalary("Joe", "Black", 90000));
    assert (!b2.setSalary("joe", 90000));
    assert (b2.getSalary("Joe", "Black") == 0);
    assert (b2.getSalary("joe") == 0);
    assert (!b2.getRank("Joe", "Black", lo, hi));
    assert (!b2.getRank("joe", lo, hi));
    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.del("Joe", "Black"));
    assert (!b2.del("joe"));
    assert (!b2.changeName("james2", "James", "Bond"));
    assert (!b2.changeEmail("Peter", "Smith", "james"));
    assert (!b2.changeName("peter", "Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter"));
    assert (b2.del("Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter2"));
    assert (!b2.setSalary("Peter", "Smith", 35000));
    assert (b2.getSalary("Peter", "Smith") == 0);
    assert (!b2.getRank("Peter", "Smith", lo, hi));
    assert (!b2.changeName("peter", "Peter", "Falcon"));
    assert (!b2.setSalary("peter", 37000));
    assert (b2.getSalary("peter") == 0);
    assert (!b2.getRank("peter", lo, hi));
    assert (!b2.del("Peter", "Smith"));
    assert (!b2.del("peter"));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (b2.getSalary("peter") == 40000);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */