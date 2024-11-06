#include <bits/stdc++.h>

using namespace std;
using LL = long long;
#define FOR(i, l, r) for(int i = (l); i <= (r); ++i)
#define REP(i, n) FOR(i, 0, (n) - 1)
#define ssize(x) int(x.size())


int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vector<vector<int> > v(n);
    REP(i, n) {
        REP(j, 2) {
            int x;
            cin >> x;
            if (x != -1)
                v[i].emplace_back(--x);
        }
    }

    struct dp_node {
        int val, fur, son;

        dp_node() {
            val = fur = son = 0;
        }

        dp_node(int a, int b, int c) {
            val = a;
            fur = b;
            son = c;
        }

        bool operator <(dp_node other) {
            return val < other.val + 1;
        }

        bool operator <=(dp_node other) {
            return val < other.val;
        }

        dp_node inc() {
            dp_node ret = *this;
            ret.val++;
            return ret;
        }
    };

    vector<pair<dp_node, dp_node> > dp(n);
    vector<dp_node> final(n);

    function<void(int)> count_down = [&](int x) {
        auto &[a, b] = dp[x];
        a = {0, x, x};
        for (auto it : v[x]) {
            count_down(it);
            auto p = dp[it].first;
            if (a < p) {
                b = a;
                a = p.inc();
                a.son = it;
            }
            else if (b < p) {
                b = p.inc();
                b.son = it;
            }
        }
    };

    count_down(0);

    function<void(int, dp_node)> count_best = [&](int x, dp_node up) {
        auto [a, b] = dp[x];
        if (up <= a)
            final[x] = a;
        else
            final[x] = up;

        for (auto it : v[x]) {
            if (it == a.son) {
                if (b.val <= up.val)
                    count_best(it, up.inc());
                else
                    count_best(it, b.inc());
            }
            else {
                if (a.val <= up.val)
                    count_best(it, up.inc());
                else
                    count_best(it, a.inc());
            }
        }
    };

    count_best(0, {0, 0, 0});
    
    constexpr int LOGN = 20;
    vector<vector<int>> prev(n, vector<int>(LOGN));
    vector<int> lvl(n), pwr(LOGN);

    pwr[0] = 1;
    FOR(i, 1, LOGN - 1)
        pwr[i] = pwr[i - 1] * 2;

    function<void(int, int, int)> fill_lca = [&](int x, int father, int level) {
        prev[x][0] = father;
        lvl[x] = level;
        FOR(i, 1, LOGN - 1)
            prev[x][i] = prev[prev[x][i - 1]][i - 1];

        for (auto it : v[x])
            fill_lca(it, x, level + 1);
    };

    fill_lca(0, 0, 1);

    function<pair<int, int>(int x, int y)> lca = [&](int x, int y) {
        bool swp = false;
        if (lvl[x] > lvl[y]) {
            swap(x, y);
            swp = true;
        }
        pair<int, int> ret = {0, 0};
        auto &[x_up, sum] = ret;

        for (int i = LOGN - 1; i >= 0; i--) {
            if (lvl[prev[y][i]] > lvl[x]) {
                if (swp)
                    x_up += pwr[i];
                sum += pwr[i];
                y = prev[y][i];
            }
        }

        if (lvl[prev[y][0]] == lvl[x]) {
            y = prev[y][0];
            if (swp)
                x_up++;
            sum++;
        }

        if (x == y)
            return ret;

        for (int i = LOGN - 1; i >= 0; i--) {
            if (prev[x][i] != prev[y][i]) {
                x = prev[x][i];
                y = prev[y][i];
                x_up += pwr[i];
                sum += 2 * pwr[i];
            }
        }

        x_up++;
        sum += 2;

        return ret;
    };

    auto jmp = [&](int x, int val) {
        for (int i = LOGN - 1; i >= 0; i--) {
            if (val >= pwr[i]) {
                val -= pwr[i];
                x = prev[x][i];
            }
        }
        return x;
    };

    int q;
    cin >> q;
    while (q--) {
        int x, d;
        cin >> x >> d;
        if (final[--x].val < d) {
            cout << "-1\n";
            continue;
        }
        int y = final[x].fur;
        auto [up_x, sum] = lca(x, y);
        if (up_x >= d)
            cout << jmp(x, d) + 1 << '\n';
        else
            cout << jmp(y, sum - d) + 1 << '\n';
    }

    return 0;
}