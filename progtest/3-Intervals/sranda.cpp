#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<pair<int, int>> intervals;
    while (true) {
        int start, end;
        cin >> start >> end;
        if (cin.fail()) break; // stop reading when input fails
        auto it = lower_bound(intervals.begin(), intervals.end(), make_pair(start, end));
        if (it != intervals.begin() && (it-1)->second >= start) {
            --it;
            it->second = max(it->second, end);
        } else if (it != intervals.end() && end >= it->first) {
            it->first = min(it->first, start);
            it->second = max(it->second, end);
        } else {
            intervals.insert(it, make_pair(start, end));
        }
    }
    for (auto& interval : intervals) {
        cout << "[" << interval.first << ", " << interval.second << "]" << endl;
    }
    return 0;
}

