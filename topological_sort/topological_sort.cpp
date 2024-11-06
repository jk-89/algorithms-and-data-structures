#include <bits/stdc++.h>
using namespace std;
using LL = long long;
#define FOR(i, l, r) for(int i = (l); i <= (r); ++i)
#define REP(i, n) FOR(i, 0, (n) - 1)
#define ssize(x) int(x.size())


int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<int> > v(n);
    vector<vector<int> > prev(n);
    vector<int> a(n);
    int mx = 0;
    REP(i, n) {
        cin >> a[i];
        mx = max(mx, a[i]);
    }
    while (m--) {
        int x, y;
        cin >> x >> y;
        v[--y].emplace_back(--x);
        prev[x].emplace_back(y);
    }

    vector<bool> vis(n);
    vector<int> order;

    function<void(int)> topological = [&](int x) {
        vis[x] = true;
        for (auto it : v[x]) {
            if (!vis[it])
                topological(it);
        }
        order.emplace_back(x);
    };

    REP(i, n) {
        if (!vis[i])
            topological(i);
    }
    reverse(order.begin(), order.end());

    int l = 1, r = mx;
    while (l < r) {
        int mid = (l + r) / 2;
        vector<bool> taken(n);
        int cnt = 0;

        for (auto it : order) {
            bool can_take = true;
            for (auto parent : prev[it]) {
                if (!taken[parent]) {
                    can_take = false;
                    break;
                }
            }
            if (can_take && a[it] <= mid) {
                taken[it] = true;
                cnt++;
            }
        }

        if (cnt < k)
            l = mid + 1;
        else
            r = mid;
    }
    cout << l << '\n';

    return 0;
}