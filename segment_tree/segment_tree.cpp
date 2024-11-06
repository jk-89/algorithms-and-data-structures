#include <bits/stdc++.h>
using namespace std;
using LL = long long;
#define FOR(i, l, r) for(int i = (l); i <= (r); ++i)
#define REP(i, n) FOR(i, 0, (n) - 1)
#define ssize(x) int(x.size())


struct Tree {
    static const int mod = (int) (1e9);
    int pwr = 1;
    vector<int> tree;

    Tree(int n) {
        while (pwr < n)
            pwr *= 2;
        tree.resize(2 * pwr);    
    }

    void refill() {
        fill(tree.begin(), tree.end(), 0);
    }

    void set_value(int v, int val) {
        v += pwr;
        tree[v] = val;
        v /= 2;
        while (v) {
            tree[v] = tree[v * 2] + tree[v * 2 + 1];
            if (tree[v] >= mod)
                tree[v] -= mod;
            v /= 2;
        }
    }

    int count(int v, int x, int y, int r) {
        if (x > r)
            return 0;
        if (y <= r)
            return tree[v];
        return (count(v * 2, x, (x + y) / 2, r) + count(v * 2 + 1, (x + y) / 2 + 1, y, r)) % mod;
    }

    int find_value(int v) {
        if (v == 0)
            return 0;
        return count(1, 1, pwr, v);
    }
};


int main() {
    cin.tie(0)->sync_with_stdio(0);
    int n, k;
    cin >> n >> k;
    Tree t(n);
    vector<int> a(n);
    REP(i, n) {
        cin >> a[i];
        a[i] = n - a[i];
    }
    vector<int> dp(n, 1);
    FOR(i, 2, k) {
        t.refill();
        REP(j, n) {
            t.set_value(a[j], dp[j]);
            dp[j] = t.find_value(a[j]);
        }
    }
    int ans = 0;
    REP(i, n) {
        ans += dp[i];
        if (ans >= t.mod)
            ans -= t.mod;
    }
    cout << ans << '\n';
    return 0;
}