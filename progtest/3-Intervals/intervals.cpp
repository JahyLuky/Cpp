#include <iostream>
#include <vector>

using namespace std;

class Interval {
public:
    int start, end;

    Interval(int start, int end) : start(start), end(end) {}

    bool overlaps(const Interval& other) const {
        return (start <= other.end && end >= other.start);
    }

    void merge(const Interval& other) {
        start = min(start, other.start);
        end = max(end, other.end);
    }
};

vector<Interval> mergeIntervals(const vector<Interval>& intervals, const Interval& newInterval) {
    vector<Interval> result;
    result.reserve(intervals.size() + 1); // Reserve space for the worst case

    auto it = intervals.begin();

    while (it != intervals.end() && it->end < newInterval.start) {
        result.push_back(*it);
        ++it;
    }

    Interval mergedInterval = newInterval;

    while (it != intervals.end() && it->start <= newInterval.end) {
        mergedInterval.merge(*it);
        ++it;
    }

    result.push_back(mergedInterval);

    while (it != intervals.end()) {
        result.push_back(*it);
        ++it;
    }

    return result;
}


vector<Interval> mergeIntervals1(vector<Interval>& intervals, Interval newInterval) {
    vector<Interval> result;
    bool newIntervalInserted = false;

    for (auto& interval : intervals) {
        if (newInterval.overlaps(interval)) {
            newInterval.merge(interval);
        } else if (newInterval.end < interval.start && !newIntervalInserted) {
            result.push_back(newInterval);
            newIntervalInserted = true;
            result.push_back(interval);
        } else {
            result.push_back(interval);
        }
    }

    if (!newIntervalInserted) {
        result.push_back(newInterval);
    }

    return result;
}

int main() {

    Interval a(6, 7);
    Interval b(1, 5);
    cout << a.overlaps(b) << endl;
    cout << b.overlaps(a) << endl;

    vector<Interval> intervals;

    Interval newInterval(5, 10);
    intervals = mergeIntervals(intervals, newInterval);

    Interval newInterval2(25, 100);
    intervals = mergeIntervals(intervals, newInterval2);

    Interval newInterval3(-5, 0);
    intervals = mergeIntervals(intervals, newInterval3);

    Interval newInterval4(8, 50);
    intervals = mergeIntervals(intervals, newInterval4);

    for (const auto& interval : intervals) {
        cout << "[" << interval.start << "," << interval.end << "] ";
    }
    cout << endl;

    return 0;
}