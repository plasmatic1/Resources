#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in an array, allowing for multiple insertions at the same time (including negative)
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   rebuild: O(sqrt(N))
//   count(): O(1)
//   floor, ceiling, above, below, contains, count(val), count(lo, hi): O(sqrt(N) + log(N)) amortized
//   valuesAndCount: O(N)
// Memory Complexity: O(N)
template <class Value, class CountType, class Comparator = less<Value>> struct SqrtBuffer {
    Comparator cmp; CountType tot; double SCALE_FACTOR; vector<pair<Value, CountType>> small, large;
    function<bool(const pair<Value, CountType>&, const pair<Value, CountType>&)> pairCmp =
        [&] (const pair<Value, CountType> &a, const pair<Value, CountType> &b) { return cmp(a.first, b.first); };
    SqrtBuffer(const double SCALE_FACTOR = 1) : tot(0), SCALE_FACTOR(SCALE_FACTOR) {}
    template <class PairIt> SqrtBuffer(const PairIt st, const PairIt en, const double SCALE_FACTOR = 1) :
            SCALE_FACTOR(SCALE_FACTOR), large(st, en) {
        assert(is_sorted(st, en, pairCmp)); resizeUnique(large); tot = 0; for (auto &&p : large) tot += p.second;
    }
    void resizeUnique(vector<pair<Value, CountType>> &v) {
        if (!v.empty()) {
            int j = 0;
            for (int i = 1; i < int(v.size()); i++) {
                if (cmp(v[i].first, v[j].first) || cmp(v[j].first, v[i].first)) {
                    v[++j] = v[i]; v[j].second += v[j - 1].second;
                } else v[j].second += v[i].second;
            }
            v.resize(j + 1);
        }
    }
    bool rebuild() {
        if (int(small.size()) > SCALE_FACTOR * sqrt(small.size() + large.size())) {
            int largeSz = int(large.size()); sort(small.begin(), small.end(), pairCmp);
            for (int i = largeSz - 1; i >= 1; i--) large[i].second -= large[i - 1].second;
            for (auto &&p : small) large.push_back(p);
            small.clear(); inplace_merge(large.begin(), large.begin() + largeSz, large.end(), pairCmp); resizeUnique(large);
            return true;
        }
        return false;
    }
    void insert(const pair<Value, CountType> &p) { small.push_back(p); tot += p.second; }
    void emplace(const Value &v, const CountType &c) { small.emplace_back(v, c); tot += c; }
    CountType aboveInd(const Value &val) {
        rebuild(); int ind = upper_bound(large.begin(), large.end(), make_pair(val, CountType(0)), pairCmp) - large.begin();
        CountType ret = ind == 0 ? 0 : large[ind - 1].second;
        for (auto &&p : small) if (!cmp(val, p.first)) ret += p.second;
        return ret;
    }
    CountType ceilingInd(const Value &val) {
        rebuild(); int ind = lower_bound(large.begin(), large.end(), make_pair(val, CountType(0)), pairCmp) - large.begin();
        CountType ret = ind == 0 ? 0 : large[ind - 1].second;
        for (auto &&p : small) if (cmp(p.first, val)) ret += p.second;
        return ret;
    }
    CountType floorInd(const Value &val) { return aboveInd(val) - 1; }
    CountType belowInd(const Value &val) { return ceilingInd(val) - 1; }
    bool contains(const Value &val) {
        if (binary_search(large.begin(), large.end(), make_pair(val, CountType(0)), pairCmp)) return true;
        if (rebuild() && binary_search(large.begin(), large.end(), make_pair(val, CountType(0)), pairCmp)) return true;
        for (auto &&p : small) if (!cmp(val, p.first) && !cmp(p.first, val)) return true;
        return false;
    }
    CountType count(const Value &val) { return aboveInd(val) - ceilingInd(val); }
    // number of values in the range [lo, hi]
    CountType count(const Value &lo, const Value &hi) { return aboveInd(hi) - ceilingInd(lo); }
    CountType count() const { return tot; } 
    void clear() { tot = 0; small.clear(); large.clear(); }
    vector<pair<Value, CountType>> valuesAndCount() const { // sorted
        vector<pair<Value, CountType>> ret;
        for (auto &&p : small) ret.push_back(p);
        int mid = int(ret.size());
        for (auto &&p : large) ret.push_back(p);
        inplace_merge(ret.begin(), ret.begin() + mid, ret.end(), pairCmp); resizeUnique(ret); return ret;
    }
};