#include <bits/stdc++.h>

using namespace std;
using LL = long long;
#define FOR(i, l, r) for(int i = (l); i <= (r); ++i)
#define REP(i, n) FOR(i, 0, (n) - 1)
#define ssize(x) int(x.size())


struct Tree {
    int pot = 1;
    vector<int> t;
    vector<int> lazy;

    explicit Tree(int n) {
        while (pot < n)
            pot *= 2;
        t.resize(2 * pot);
        lazy.resize(2 * pot);
    }

    void push(int v, int len) {
        if (lazy[v] == 0)
            return;
        if (lazy[v] == 1)
            t[v * 2] = t[v * 2 + 1] = len;
        else
            t[v * 2] = t[v * 2 + 1] = 0;
        lazy[v * 2] = lazy[v * 2 + 1] = lazy[v];
        lazy[v] = 0;
    }

    void set(int v, int x, int y, int l, int r, int wh) {
        if (y < l || x > r)
            return;
        if (x >= l && y <= r) {
            if (wh == 1)
                t[v] = y - x + 1;
            else
                t[v] = 0;
            lazy[v] = wh;
            return;
        }
        push(v, (y - x + 1) / 2);
        set(v * 2, x, (x + y) / 2, l, r, wh);
        set(v * 2 + 1, (x + y) / 2 + 1, y, l, r, wh);
        t[v] = t[v * 2] + t[v * 2 + 1];
    }

    void set(int l, int r, int wh) {
        set(1, 1, pot, l, r, wh);
    }

    int sum() {
        return t[1];
    }
};


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    int x, y;
    char ch;
    cin >> n >> q;
    Tree tree(n);
    while (q--) {
        cin >> x >> y >> ch;
        tree.set(x, y, (ch == 'C' ? 2 : 1));
        cout << tree.sum() << '\n';
    }
    return 0;
}
