#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Decomposes the array into blocks of size sqrt(n) multiplied by a factor
// The factor should be between 1 and 10, and should be smaller for large N
// Time Complexity:
//   constructor: O(N)
//   insert, erase: O(sqrt(N) + log(N))
//   push_front, pop_front: O(sqrt(N))
//   push_back, pop_back: O(1) amortized
//   front, back, empty, size: O(1)
//   at, accessor: O(log(N))
//   values: O(N)
// Memory Complexity: O(N)
template <class Value> struct SqrtArray {
    int n, SCALE_FACTOR; vector<vector<Value>> a; vector<int> prefixSZ;
    SqrtArray(const int SCALE_FACTOR = 1) : n(0), SCALE_FACTOR(SCALE_FACTOR) {}
    SqrtArray(const int n, const int SCALE_FACTOR) : n(n), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        for (int i = 0; i < n; i += sqrtn) { a.emplace_back(min(sqrtn, n - i)); prefixSZ.push_back(0); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    template <typename It> SqrtArray(const It st, const It en, const int SCALE_FACTOR = 1) : n(en - st), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        for (It i = st; i < en; i += sqrtn) { a.emplace_back(i, min(i + sqrtn, en)); prefixSZ.push_back(0); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    SqrtArray(initializer_list<Value> il, const int SCALE_FACTOR = 1) : n(il.end() - il.begin()), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        for (auto i = il.begin(); i < il.end(); i += sqrtn) { a.emplace_back(i, min(i + sqrtn, il.end())); prefixSZ.push_back(0); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void insert(int k, const Value val) { // inserts value before kth index
        assert(0 <= k && k <= n);
        if (n++ == 0) { a.emplace_back(); prefixSZ.push_back(0); }
        int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        k -= prefixSZ[hi]; int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if (hi == -1) a[hi += (int) a.size()].push_back(val);
        else a[hi].insert(a[hi].begin() + k, val);
        if ((int) a[hi].size() > 2 * sqrtn) {
            a.emplace(a.begin() + hi + 1, a[hi].begin() + sqrtn, a[hi].end()); a[hi].resize(sqrtn); prefixSZ.push_back(0);
        }
        for (int i = hi + 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void push_front(const Value val) {
        if (n++ == 0) { a.emplace_back(); prefixSZ.push_back(0); }
        a.front().insert(a.front().begin(), val); int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if ((int) a.front().size() > 2 * sqrtn) {
            a.emplace(a.begin() + 1, a.front().begin() + sqrtn, a.front().end()); a.front().resize(sqrtn); prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void push_back(const Value val) {
        if (n++ == 0) { a.emplace_back(); prefixSZ.push_back(0); }
        a.back().push_back(val); int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if ((int) a.back().size() > 2 * sqrtn) {
            a.emplace_back(a.back().begin() + sqrtn, a.back().end()); a[(int) a.size() - 2].resize(sqrtn);
            prefixSZ.push_back(prefixSZ[(int) a.size() - 2] + (int) a[(int) a.size() - 2].size());
        }
    }
    void erase(const int k) {
        assert(0 <= k && k < n); --n; int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        a[hi].erase(a[hi].begin() + k - prefixSZ[hi]);
        if (a[hi].empty()) { a.erase(a.begin() + hi); prefixSZ.pop_back(); }
        for (int i = hi + 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void pop_front() {
        assert(n > 0); --n; a.front().erase(a.front().begin());
        if (a.front().empty()) { a.erase(a.begin()); prefixSZ.pop_back(); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void pop_back() {
        assert(n > 0); --n; a.back().pop_back();
        if (a.back().empty()) { a.pop_back(); prefixSZ.pop_back(); }
    }
    Value &operator [](int k) {
        assert(0 <= k && k < n); int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi][k - prefixSZ[hi]];
    }
    const Value &at(const int k) const {
        assert(0 <= k && k < n); int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi][k - prefixSZ[hi]];
    }
    const Value &operator [](const int k) const { return at(k); }
    const Value &front() const { assert(n > 0); return a.front().front(); }
    const Value &back() const { assert(n > 0); return a.back().back(); }
    bool empty() const { return n == 0; }
    int size() const { return n; }
    vector<Value> values() const {
        vector<Value> ret;
        for (auto &&ai : a) for (auto &&aij : ai) ret.push_back(aij);
        return ret;
    }
};

