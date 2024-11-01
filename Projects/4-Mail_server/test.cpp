#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
#endif /* __PROGTEST__ */

//----------------------------------------------
class String {
public:
    String() = default;

    String(const String &src);

    ~String();

    String &operator=(const char *src);

    String &operator=(const String &src);

    bool operator==(const String &src) const;

    bool operator!=(const String &src) const;

    char *str;
};

String::String(const String &src) {
    size_t len = strlen(src.str);
    str = new char[len + 1];
    for (size_t i = 0; i < len; ++i) {
        str[i] = src.str[i];
    }
    str[len] = '\0';
}

String::~String() {
    delete[] str;
}

String &String::operator=(const char *src) {
    if (src == nullptr) {
        return *this;
    }

    size_t len = strlen(src);
    str = new char[len + 1];
    for (size_t i = 0; i < len; ++i) {
        str[i] = src[i];
    }
    str[len] = '\0';
    return *this;
}

String &String::operator=(const String &src) {
    if (this == &src) {
        return *this;
    }

    if (src.str == nullptr) {
        if (str == nullptr) {
            return *this;
        }
        delete[] str;
        str = nullptr;
    }

    size_t len = strlen(src.str);
    str = new char[len + 1];
    for (size_t i = 0; i < len; ++i) {
        str[i] = src.str[i];
    }
    str[len] = '\0';
    return *this;
}

bool String::operator==(const String &src) const {
    size_t len1 = strlen(str);
    size_t len2 = strlen(src.str);
    if (len1 != len2)
        return false;
    for (size_t i = 0; i < len1; ++i) {
        if (tolower(str[i]) != tolower(src.str[i]))
            return false;
    }
    return true;
}

bool String::operator!=(const String &src) const {
    if (*this == src)
        return false;
    return true;
}

//----------------------------------------------
class CMail {
public:
    CMail() = default;

    CMail(const char *from, const char *to, const char *body);

    bool operator==(const CMail &x) const;

    friend ostream &operator<<(ostream &os, const CMail &m) {
        os << "From: " << m.from_.str << ", To: " << m.to_.str << ", Body: " << m.body_.str;
        return os;
    }

    String from_;
    String to_;
    String body_;
};

CMail::CMail(const char *from, const char *to, const char *body) {
    from_ = from;
    to_ = to;
    body_ = body;
}

bool CMail::operator==(const CMail &x) const {
    if (from_ != x.from_
        || to_ != x.to_
        || body_ != x.body_) {
        return false;
    }
    return true;
}

//----------------------------------------------
class CMailIterator {
public:
    explicit operator bool() const;

    bool operator!() const;

    const CMail &operator*() const;

    CMailIterator &operator++();

    ~CMailIterator();

    CMail *data_;
    int itr_;
    int pos_;
};

CMailIterator::operator bool() const {
    if (itr_ == -1 || itr_ == pos_)
        return false;
    return true;
}

bool CMailIterator::operator!() const {
    if (itr_ == -1 || itr_ == pos_)
        return true;
    return false;
}

const CMail &CMailIterator::operator*() const {
    return data_[itr_];
}

CMailIterator &CMailIterator::operator++() {
    itr_++;
    return *this;
}

CMailIterator::~CMailIterator() {
    delete[] data_;
}

//----------------------------------------------
class CMailServer {
public:
    CMailServer();

    CMailServer(const CMailServer &src);

    void DeleteCMailServer();

    CMailServer &operator=(const CMailServer &src);

    ~CMailServer();

    void ResizeMailServer();

    int SearchMail(const char *email, int start, int end, bool isFrom) const;

    void sendMail(const CMail &m);

    CMailIterator outbox(const char *email) const;

    CMailIterator inbox(const char *email) const;

private:
    CMail *data_from_;
    int pos_from_;
    int size_from_;
    CMail *data_to_;
    int pos_to_;
    int size_to_;
};

CMailServer::CMailServer() {
    data_from_ = nullptr;
    pos_from_ = 0;
    size_from_ = 0;
    data_to_ = nullptr;
    pos_to_ = 0;
    size_to_ = 0;
}

CMailServer::CMailServer(const CMailServer &src) {
    size_from_ = src.size_from_;
    pos_from_ = src.pos_from_;
    size_to_ = src.size_to_;
    pos_to_ = src.pos_to_;

    if (size_from_ > 0) {
        data_from_ = new CMail[src.size_from_];
    } else {
        data_from_ = nullptr;
    }

    if (size_to_ > 0) {
        data_to_ = new CMail[src.size_to_];
    } else {
        data_to_ = nullptr;
    }

    for (int i = 0; i < src.pos_from_; ++i) {
        data_from_[i].from_ = src.data_from_[i].from_;
        data_from_[i].to_ = src.data_from_[i].to_;
        data_from_[i].body_ = src.data_from_[i].body_;
    }

    for (int i = 0; i < src.pos_to_; ++i) {
        data_to_[i].from_ = src.data_to_[i].from_;
        data_to_[i].to_ = src.data_to_[i].to_;
        data_to_[i].body_ = src.data_to_[i].body_;
    }
}

void CMailServer::DeleteCMailServer() {
    delete[]data_from_;
    delete[]data_to_;
}

CMailServer &CMailServer::operator=(const CMailServer &src) {
    if (this == &src)
        return *this;

    if (data_from_ != nullptr || data_to_ != nullptr)
        DeleteCMailServer();

    pos_from_ = src.pos_from_;
    data_from_ = src.data_from_;
    size_from_ = src.size_from_;

    pos_to_ = src.pos_to_;
    data_to_ = src.data_to_;
    size_to_ = src.size_to_;

    if (src.size_from_ != 0) {
        data_from_ = new CMail[src.size_from_];
    } else {
        data_from_ = nullptr;
    }

    if (src.size_to_ != 0) {
        data_to_ = new CMail[src.size_to_];
    } else {
        data_to_ = nullptr;
    }

    for (int i = 0; i < src.pos_from_; ++i) {
        data_from_[i].from_ = src.data_from_[i].from_;
        data_from_[i].to_ = src.data_from_[i].to_;
        data_from_[i].body_ = src.data_from_[i].body_;
    }

    for (int i = 0; i < src.pos_to_; ++i) {
        data_to_[i].from_ = src.data_to_[i].from_;
        data_to_[i].to_ = src.data_to_[i].to_;
        data_to_[i].body_ = src.data_to_[i].body_;
    }

    return *this;
}

CMailServer::~CMailServer() {
    DeleteCMailServer();
}

void CMailServer::ResizeMailServer() {
    size_from_ *= 2;
    CMail *tmp = new CMail[size_from_];
    for (int i = 0; i < pos_from_; ++i) {
        tmp[i].from_ = data_from_[i].from_;
        tmp[i].to_ = data_from_[i].to_;
        tmp[i].body_ = data_from_[i].body_;
    }
    delete[] data_from_;
    data_from_ = tmp;

    size_to_ *= 2;
    CMail *tmp2 = new CMail[size_to_];
    for (int i = 0; i < pos_to_; ++i) {
        tmp2[i].from_ = data_to_[i].from_;
        tmp2[i].to_ = data_to_[i].to_;
        tmp2[i].body_ = data_to_[i].body_;
    }
    delete[] data_to_;
    data_to_ = tmp2;
}

int CMailServer::SearchMail(const char *email, int start, int end, bool isFrom) const {
    while (start <= end) {
        int middle = start + (end - start) / 2;
        if (isFrom) {
            if (middle < 0 || middle >= pos_from_) {
                return -1;
            }
            if (strcasecmp(data_from_[middle].from_.str, email) == 0) {
                return middle;
            } else if (strcasecmp(data_from_[middle].from_.str, email) < 0) {
                start = middle + 1;
            } else {
                end = middle - 1;
            }
        } else {
            if (middle < 0 || middle >= pos_to_) {
                return -1;
            }
            if (strcasecmp(data_to_[middle].to_.str, email) == 0) {
                return middle;
            } else if (strcasecmp(data_to_[middle].to_.str, email) < 0) {
                start = middle + 1;
            } else {
                end = middle - 1;
            }
        }
    }
    return -1;
}

void CMailServer::sendMail(const CMail &m) {
    if (size_from_ == 0) {
        data_from_ = new CMail[1];
        size_from_ = 1;
        data_from_[0].from_ = m.from_;
        data_from_[0].to_ = m.to_;
        data_from_[0].body_ = m.body_;
        pos_from_ = 1;
    }

    if (size_to_ == 0) {
        data_to_ = new CMail[1];
        size_to_ = 1;
        data_to_[0].from_ = m.from_;
        data_to_[0].to_ = m.to_;
        data_to_[0].body_ = m.body_;
        pos_to_ = 1;
        return;
    }

    if (pos_from_ == size_from_ || pos_to_ == size_to_)
        ResizeMailServer();

    bool flag1 = true;
    for (int i = 0; i < pos_from_; ++i) {
        if (strcasecmp(data_from_[i].from_.str, m.from_.str) > 0) {
            CMail *newData = new CMail[size_from_];
            for (int j = 0; j < i; j++) {
                newData[j].from_ = data_from_[j].from_;
                newData[j].to_ = data_from_[j].to_;
                newData[j].body_ = data_from_[j].body_;
            }
            newData[i].from_ = m.from_;
            newData[i].to_ = m.to_;
            newData[i].body_ = m.body_;
            for (int x = i; x < pos_from_; x++) {
                newData[x + 1] = data_from_[x];
            }
            delete[] data_from_;
            data_from_ = newData;
            flag1 = false;
            break;
        }
    }
    if (flag1) {
        data_from_[pos_from_].from_ = m.from_;
        data_from_[pos_from_].to_ = m.to_;
        data_from_[pos_from_].body_ = m.body_;
    }
    pos_from_++;
//--------------------------------------------
    bool flag2 = true;
    for (int i = 0; i < pos_to_; ++i) {
        if (strcasecmp(data_to_[i].to_.str, m.to_.str) > 0) {
            CMail *newData = new CMail[size_to_];
            for (int j = 0; j < i; j++) {
                newData[j].from_ = data_to_[j].from_;
                newData[j].to_ = data_to_[j].to_;
                newData[j].body_ = data_to_[j].body_;
            }
            newData[i].from_ = m.from_;
            newData[i].to_ = m.to_;
            newData[i].body_ = m.body_;
            for (int x = i; x < pos_to_; x++) {
                newData[x + 1] = data_to_[x];
            }
            delete[] data_to_;
            data_to_ = newData;
            flag2 = false;
            break;
        }
    }
    if (flag2) {
        data_to_[pos_to_].from_ = m.from_;
        data_to_[pos_to_].to_ = m.to_;
        data_to_[pos_to_].body_ = m.body_;
    }
    pos_to_++;
}

CMailIterator CMailServer::outbox(const char *email) const {
    CMailIterator tmp;
    tmp.data_ = new CMail[pos_from_];
    tmp.pos_ = 0;
    tmp.itr_ = 0;

    int found = SearchMail(email, 0, pos_from_, true);
    if (found == -1) {
        tmp.itr_ = -1;
        return tmp;
    }
    bool flag = false;
    while (true) {
        if ((found - 1) >= 0 && strcasecmp(data_from_[found - 1].from_.str, email) == 0) {
            found--;
            flag = true;
        } else {
            if (flag) {
                tmp.data_[tmp.pos_].from_ = data_from_[found].from_;
                tmp.data_[tmp.pos_].to_ = data_from_[found].to_;
                tmp.data_[tmp.pos_].body_ = data_from_[found].body_;
                tmp.pos_++;
                found++;
            }
            break;
        }
    }
    tmp.data_[tmp.pos_].from_ = data_from_[found].from_;
    tmp.data_[tmp.pos_].to_ = data_from_[found].to_;
    tmp.data_[tmp.pos_].body_ = data_from_[found].body_;
    tmp.pos_++;
    while (true) {
        found++;
        if (found < pos_from_ &&
            strcasecmp(data_from_[found].from_.str, email) == 0) {
            tmp.data_[tmp.pos_].from_ = data_from_[found].from_;
            tmp.data_[tmp.pos_].to_ = data_from_[found].to_;
            tmp.data_[tmp.pos_].body_ = data_from_[found].body_;
            tmp.pos_++;
        } else {
            break;
        }
    }

    return tmp;
}

CMailIterator CMailServer::inbox(const char *email) const {
    CMailIterator tmp;
    tmp.data_ = new CMail[pos_to_];
    tmp.pos_ = 0;
    tmp.itr_ = 0;

    int found = SearchMail(email, 0, pos_to_, false);
    if (found == -1) {
        tmp.itr_ = -1;
        return tmp;
    }
    bool flag = false;
    while (true) {
        if ((found - 1) >= 0 && strcasecmp(data_to_[found - 1].to_.str, email) == 0) {
            found--;
            flag = true;
        } else {
            if (flag) {
                tmp.data_[tmp.pos_].from_ = data_to_[found].from_;
                tmp.data_[tmp.pos_].to_ = data_to_[found].to_;
                tmp.data_[tmp.pos_].body_ = data_to_[found].body_;
                tmp.pos_++;
                found++;
            }
            break;
        }
    }
    tmp.data_[tmp.pos_].from_ = data_to_[found].from_;
    tmp.data_[tmp.pos_].to_ = data_to_[found].to_;
    tmp.data_[tmp.pos_].body_ = data_to_[found].body_;
    tmp.pos_++;
    while (true) {
        found++;
        if (found < pos_to_ &&
            strcasecmp(data_to_[found].to_.str, email) == 0) {
            tmp.data_[tmp.pos_].from_ = data_to_[found].from_;
            tmp.data_[tmp.pos_].to_ = data_to_[found].to_;
            tmp.data_[tmp.pos_].body_ = data_to_[found].body_;
            tmp.pos_++;
        } else {
            break;
        }
    }

    return tmp;
}
//----------------------------------------------
#ifndef __PROGTEST__

bool matchOutput(const CMail &m,
                 const char *str) {
    ostringstream oss;
    oss << m;
    return oss.str() == str;
}

int main() {
    char from[100], to[100], body[1024];

    assert (CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));

    CMailServer s0;
    s0.sendMail(CMail("john", "peter", "some important mail"));

    strncpy(from, "john", sizeof(from));
    strncpy(to, "thomas", sizeof(to));
    strncpy(body, "another important mail", sizeof(body));

    s0.sendMail(CMail(from, to, body));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "alice", sizeof(to));
    strncpy(body, "deadline notice", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    s0.sendMail(CMail("alice", "john", "deadline confirmation"));
    s0.sendMail(CMail("peter", "alice", "PR bullshit"));

    CMailIterator i0 = s0.inbox("alice");
    assert (i0 && *i0 == CMail("john", "alice", "deadline notice"));

    assert (matchOutput(*i0, "From: john, To: alice, Body: deadline notice"));
    assert (++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i0, "From: peter, To: alice, Body: PR bullshit"));
    assert (!++i0);

    CMailIterator i1 = s0.inbox("john");
    assert (i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
    assert (matchOutput(*i1, "From: alice, To: john, Body: deadline confirmation"));
    assert (!++i1);

    CMailIterator i2 = s0.outbox("john");
    assert (i2 && *i2 == CMail("john", "peter", "some important mail"));
    assert (matchOutput(*i2, "From: john, To: peter, Body: some important mail"));
    assert (++i2 && *i2 == CMail("john", "thomas", "another important mail"));
    assert (matchOutput(*i2, "From: john, To: thomas, Body: another important mail"));
    assert (++i2 && *i2 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i2, "From: john, To: alice, Body: deadline notice"));
    assert (!++i2);

    CMailIterator i3 = s0.outbox("thomas");
    assert (!i3);

    CMailIterator i4 = s0.outbox("steve");
    assert (!i4);

    CMailIterator i5 = s0.outbox("thomas");
    s0.sendMail(CMail("thomas", "boss", "daily report"));
    assert (!i5);

    CMailIterator i6 = s0.outbox("thomas");
    assert (i6 && *i6 == CMail("thomas", "boss", "daily report"));
    assert (matchOutput(*i6, "From: thomas, To: boss, Body: daily report"));
    assert (!++i6);

    CMailIterator i7 = s0.inbox("alice");
    s0.sendMail(CMail("thomas", "alice", "meeting details"));
    assert(i7 && *i7 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i7, "From: john, To: alice, Body: deadline notice"));
    assert(++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
    assert(matchOutput(*i7, "From: peter, To: alice, Body: PR bullshit"));
    assert(!++i7);

    CMailIterator i8 = s0.inbox("alice");
    assert (i8 && *i8 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i8, "From: john, To: alice, Body: deadline notice"));
    assert (++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i8, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i8, "From: thomas, To: alice, Body: meeting details"));
    assert (!++i8);

    CMailServer s1(s0);
    s0.sendMail(CMail("joe", "alice", "delivery details"));
    s1.sendMail(CMail("sam", "alice", "order confirmation"));
    //cout << "--------------\n";
    CMailIterator i9 = s0.inbox("alice");
    assert (i9 && *i9 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i9, "From: john, To: alice, Body: deadline notice"));
    assert (++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i9, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i9, "From: thomas, To: alice, Body: meeting details"));
    assert (++i9 && *i9 == CMail("joe", "alice", "delivery details"));


    assert (matchOutput(*i9, "From: joe, To: alice, Body: delivery details"));
    assert (!++i9);

    CMailIterator i10 = s1.inbox("alice");
    assert (i10 && *i10 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i10, "From: john, To: alice, Body: deadline notice"));
    assert (++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i10, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i10, "From: thomas, To: alice, Body: meeting details"));
    assert (++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
    assert (matchOutput(*i10, "From: sam, To: alice, Body: order confirmation"));
    assert (!++i10);

    CMailServer s2;
    s2.sendMail(CMail("alice", "alice", "mailbox test"));
    CMailIterator i11 = s2.inbox("alice");
    assert (i11 && *i11 == CMail("alice", "alice", "mailbox test"));
    assert (matchOutput(*i11, "From: alice, To: alice, Body: mailbox test"));
    assert (!++i11);

    s2 = s0;
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s2.sendMail(CMail("paul", "alice", "invalid invoice"));
    CMailIterator i12 = s0.inbox("alice");
    assert (i12 && *i12 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i12, "From: john, To: alice, Body: deadline notice"));
    assert (++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i12, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i12, "From: thomas, To: alice, Body: meeting details"));
    assert (++i12 && *i12 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i12, "From: joe, To: alice, Body: delivery details"));
    assert (++i12 && *i12 == CMail("steve", "alice", "newsletter"));
    assert (matchOutput(*i12, "From: steve, To: alice, Body: newsletter"));
    assert (!++i12);

    CMailIterator i13 = s2.inbox("alice");
    assert (i13 && *i13 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i13, "From: john, To: alice, Body: deadline notice"));
    assert (++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i13, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i13, "From: thomas, To: alice, Body: meeting details"));
    assert (++i13 && *i13 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i13, "From: joe, To: alice, Body: delivery details"));
    assert (++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
    assert (matchOutput(*i13, "From: paul, To: alice, Body: invalid invoice"));
    assert (!++i13);


    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */