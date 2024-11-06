#include <bits/stdc++.h>
using namespace std;
using LL = long long;
#define FOR(i, l, r) for(int i = (l); i <= (r); ++i)
#define REP(i, n) FOR(i, 0, (n) - 1)
#define ssize(x) int(x.size())


struct Hashing {
    vector<int> ha, pw;
    int mod;
    int base = 31;

    Hashing(string &str, int m) {
        mod = m;
        int len = ssize(str);
        ha.resize(len + 1);
        pw.resize(len + 1, 1);
        REP(i, len) {
            ha[i + 1] = int(((LL) ha[i] * base + str[i] - 'a' + 1) % mod);
            pw[i + 1] = int(((LL) pw[i] * base) % mod);
        }
    }

    int operator()(int l, int r) {
        return int(((ha[r + 1] - (LL) ha[l] * pw[r - l + 1]) % mod + mod) % mod);
    }
};

struct DoubleHashing {
    Hashing h1, h2;
    DoubleHashing(string &str) : h1(str, (int) (1e9 + 7)), h2(str, (int) (1e9 + 9)) {}
    LL operator()(int l, int r) {
        return h1(l, r) * LL(h2.mod) + h2(l, r);
    }
};


int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, q;
    string s;
    cin >> n >> q >> s;

    DoubleHashing hash(s);
    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        int d1 = y1 - x1 + 1;
        int d2 = y2 - x2 + 1;
        x1--;
        x2--;
        
        int l = 0, r = min(d1, d2) - 1;
        while (l < r) {
            int mid = (l + r) / 2;
            if (hash(x1, x1 + mid) != hash(x2, x2 + mid))
                r = mid;
            else
                l = mid + 1;
        }

        LL h1 = hash(x1, x1 + l);
        LL h2 = hash(x2, x2 + l);
        if (h1 < h2)
            cout << "<";
        else if (h1 > h2)
            cout << ">";
        else {
            if (d1 < d2)
                cout << "<";
            else if (d1 > d2)
                cout << ">";
            else
                cout << "=";
        }
        cout << '\n';
    }

    return 0;
}