#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRange {
public:
    CRange() = default;

    // constructor
    CRange(long long lo, long long hi) {
        if (lo > hi) {
            throw logic_error("Invalid exception thrown");
        }
        from_ = lo;
        to_ = hi;
    }


    void merge_intervals(const CRange &other) {
        from_ = min(from_, other.from_);
        to_ = max(to_, other.to_);
    }

    bool overlap_intervals(const CRange &other) {
        return from_ <= other.to_ && to_ >= other.from_;
    }

    long long from_;
    long long to_;
private:
};

class CRangeList {
private:
    vector<CRange> ranges_;

public:
    // constructor
    CRangeList() = default;

    // = range
    CRangeList(const CRange &other) {
        ranges_.clear();
        ranges_.push_back(other);
    }

    void merge(const CRange &a) {
        CRange other = a;
        bool inserted = false;
        auto itr = ranges_.begin();
        for (size_t i = 0; i < ranges_.size();) {
            if (ranges_[i].from_ <= other.from_ && ranges_[i].to_ <= other.to_ &&
                ranges_[i].to_ >= other.from_) {
                other.from_ = ranges_[i].from_;
                ranges_.erase(itr);
            } else if (other.from_ <= ranges_[i].from_ && other.to_ <= ranges_[i].to_ &&
                       other.to_ >= ranges_[i].from_) {
                other.to_ = ranges_[i].to_;
                ranges_.erase(itr);
            } else if (ranges_[i].to_ < other.from_ && (ranges_[i].to_ + 1) != other.from_
            && ranges_[i].to_ != LLONG_MAX) {
                itr++;
                i++;
            } else if (other.to_ < ranges_[i].from_ && (other.to_ + 1) != ranges_[i].from_
            && other.to_ != LLONG_MAX) {
                ranges_.emplace(itr, other);
                inserted = true;
                break;
            } else if (ranges_[i].from_ <= other.from_ && other.to_ <= ranges_[i].to_) {
                inserted = true;
                break;
            } else if (other.from_ <= ranges_[i].from_ && other.to_ >= ranges_[i].to_) {
                ranges_.erase(itr);
            } else if (ranges_[i].to_ == other.from_ ||
                       (ranges_[i].to_ != LLONG_MAX && (ranges_[i].to_ + 1) == other.from_)) {
                other.from_ = ranges_[i].from_;
                ranges_.erase(itr);
            } else if (other.to_ == ranges_[i].from_ ||
                       (other.to_ != LLONG_MAX && (other.to_ + 1) == ranges_[i].from_)) {
                other.to_ = ranges_[i].to_;
                ranges_.erase(itr);
            } else { i++; }
        }

        if (!inserted)
            ranges_.emplace_back(other);
    }

    // includes long long / range
    bool includes(const long long find) const {
        for (const auto &range: ranges_) {
            if (range.from_ <= find && range.to_ >= find)
                return true;
        }
        return false;
    }

    bool includes(const CRange &other) const {
        for (const auto &range: ranges_) {
            if (range.from_ <= other.from_ && range.to_ >= other.to_)
                return true;
        }
        return false;
    }

    // += range / range list
    CRangeList &operator+=(const CRange &other) {
        merge(other);

        return *this;
    }

    // += range list
    CRangeList &operator+=(const CRangeList &other_list) {
        for (const auto &range: other_list.ranges_)
            merge(range);

        return *this;
    }

    // + range
    CRangeList operator+(const CRange &other) const {
        CRangeList result(*this);
        result += other;
        return result;
    }

    // + range list
    CRangeList operator+(const CRangeList &other) const {
        CRangeList result(*this);
        result += other;
        return result;
    }

    // - range
    CRangeList operator-(const CRange &other) const {
        CRangeList result(*this);
        result -= other;
        return result;
    }

    // - range list
    CRangeList operator-(const CRangeList &other) const {
        CRangeList result(*this);
        result -= other;
        return result;
    }

    void unmerge(const CRange &a) {
        CRange other = a;
        auto itr = ranges_.begin();
        for (size_t i = 0; i < ranges_.size();) {
            if (ranges_[i].from_ == other.from_ && ranges_[i].to_ == other.to_) {
                ranges_.erase(itr);
                break;
            } else if ((ranges_[i].from_ <= other.from_ && ranges_[i].to_ <= other.to_ &&
                        ranges_[i].to_ >= other.from_) && other.from_ != LLONG_MIN) {
                long long tmp = other.from_;
                other.from_ = ranges_[i].to_;
                ranges_[i].to_ = tmp - 1;
                itr++;
                i++;
            } else if ((other.from_ <= ranges_[i].from_ && other.to_ <= ranges_[i].to_ &&
                        other.to_ >= ranges_[i].from_) && other.to_ != LLONG_MAX) {
                ranges_[i].from_ = other.to_ + 1;
                break;
            } else if (ranges_[i].to_ < other.from_ && (ranges_[i].to_ + 1) != other.from_
                       && ranges_[i].to_ != LLONG_MAX) {
                itr++;
                i++;
            } else if (other.to_ < ranges_[i].from_ &&
                       (other.to_ != LLONG_MAX && (other.to_ + 1) != ranges_[i].from_)
                       && other.to_ != LLONG_MAX) {
                break;
            } else if ((ranges_[i].from_ <= other.from_ && other.to_ <= ranges_[i].to_)
                       && other.from_ != LLONG_MIN && other.to_ != LLONG_MAX) {
                long long old_to = ranges_[i].to_;
                ranges_[i].to_ = other.from_ - 1;
                CRange tmp(other.to_ + 1, old_to);
                ++itr;
                ranges_.emplace(itr, tmp);
                break;
            } else if (other.from_ <= ranges_[i].from_ && other.to_ >= ranges_[i].to_) {
                other.from_ = ranges_[i].to_;
                ranges_.erase(itr);
            } else if (ranges_[i].to_ == other.from_ ||
                       (ranges_[i].to_ != LLONG_MAX && (ranges_[i].to_ + 1) == other.from_)) {
                if (ranges_[i].to_ == other.from_) {
                    ranges_[i].to_ = other.from_ - 1;
                }
                itr++;
                i++;
            } else if (other.to_ == ranges_[i].from_ ||
                       (other.to_ != LLONG_MAX && (other.to_ + 1) == ranges_[i].from_)) {
                ranges_[i].from_ = other.to_ + 1;
                break;
            } else { i++; }
        }
    }

    // -= range
    CRangeList &operator-=(const CRange &other) {
        unmerge(other);

        return *this;
    }

    // -= range list
    CRangeList &operator-=(const CRangeList &other_list) {
        for (const auto &range: other_list.ranges_)
            unmerge(range);

        return *this;
    }

    // operator ==
    bool operator==(const CRangeList &other_list) const {
        if (ranges_.size() != other_list.ranges_.size())
            return false;

        auto itr = other_list.ranges_.begin();
        for (const auto &range: ranges_) {
            if (range.from_ != (*itr).from_ || range.to_ != (*itr).to_)
                return false;
            else
                itr++;
        }
        return true;
    }

    // operator !=
    bool operator!=(const CRangeList &other_list) const {
        auto itr = other_list.ranges_.begin();
        for (const auto &range: ranges_) {
            if (range.from_ != (*itr).from_ || range.to_ != (*itr).to_)
                return true;
            else
                itr++;
        }
        return false;
    }

    // operator <<
    const friend ostream &operator<<(ostream &out, const CRangeList &a) {
        out << "{";
        for (size_t i = 0; i < a.ranges_.size(); ++i) {
            if (a.ranges_[i].from_ != a.ranges_[i].to_)
                out << "<" << a.ranges_[i].from_ << ".." << a.ranges_[i].to_ << ">";
            else
                out << a.ranges_[i].from_;
            if ((i + 1) != a.ranges_.size())
                out << ",";
        }
        out << "}";
        return out;
    }
};

const CRangeList operator+(const CRange &a, const CRange &b) {
    CRangeList tmp;
    tmp += a;
    tmp += b;
    return tmp;
}

const CRangeList operator-(const CRange &a, const CRange &b) {
    CRangeList tmp;
    tmp = a;
    tmp -= b;
    return tmp;
}

#ifndef __PROGTEST__

string toString(const CRangeList &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main() {
    CRangeList a, b;

    assert (sizeof(CRange) <= 2 * sizeof(long long));
    a = CRange(5, 10);
    a += CRange(25, 100);
    assert (toString(a) == "{<5..10>,<25..100>}");

    a += CRange(-5, 0);
    a += CRange(8, 50);
    assert (toString(a) == "{<-5..0>,<5..100>}");

    a += CRange(101, 105) + CRange(120, 150) + CRange(160, 180) + CRange(190, 210);
    assert (toString(a) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}");

    a += CRange(106, 119) + CRange(152, 158);
    assert (toString(a) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}");

    a += CRange(-3, 170);
    a += CRange(-30, 1000);
    assert (toString(a) == "{<-30..1000>}");
    b = CRange(-500, -300) + CRange(2000, 3000) + CRange(700, 1001);

    a += b;
    assert (toString(a) == "{<-500..-300>,<-30..1001>,<2000..3000>}");

    a -= CRange(-400, -400);
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}");

    a -= CRange(10, 20) + CRange(900, 2500) + CRange(30, 40) + CRange(10000, 20000);
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    try {
        a += CRange(15, 18) + CRange(10, 0) + CRange(35, 38);
        assert ("Exception not thrown" == nullptr);
    }
    catch (const std::logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown" == nullptr);
    }
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");

    b = a;
    assert (a == b);

    assert (!(a != b));
    b += CRange(2600, 2700);
    assert (toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    assert (a == b);
    assert (!(a != b));
    b += CRange(15, 15);
    assert (toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}");
    assert (!(a == b));
    assert (a != b);

    assert (b.includes(15));
    assert (b.includes(2900));
    assert (b.includes(CRange(15, 15)));
    assert (b.includes(CRange(-350, -350)));
    assert (b.includes(CRange(100, 200)));
    assert (!b.includes(CRange(800, 900)));
    assert (!b.includes(CRange(-1000, -450)));
    assert (!b.includes(CRange(0, 500)));
    a += CRange(-10000, 10000) + CRange(10000000, 1000000000);
    assert (toString(a) == "{<-10000..10000>,<10000000..1000000000>}");
    b += a;
    assert (toString(b) == "{<-10000..10000>,<10000000..1000000000>}");
    b -= a;
    assert (toString(b) == "{}");
    b += CRange(0, 100) + CRange(200, 300) - CRange(150, 250) + CRange(160, 180) - CRange(170, 170);
    assert (toString(b) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
    b -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
    assert (toString(b) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");


#ifdef EXTENDED_SYNTAX
    CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
    assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
    ostringstream oss;
    oss << setfill ( '=' ) << hex << left;
    for ( const auto & v : x + CRange ( -100, -100 ) )
      oss << v << endl;
    oss << setw ( 10 ) << 1024;
    assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */