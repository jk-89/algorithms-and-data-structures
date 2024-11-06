#include <bits/stdc++.h>

using namespace std;
using LL = long long;
#define FOR(i, l, r) for(int i = (l); i <= (r); ++i)
#define REP(i, n) FOR(i, 0, (n) - 1)
#define ssize(x) int(x.size())


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int p, k, fr;
    int n, m;
    int x, y;
    cin >> p >> k >> fr;
    vector<bool> frb(p + 1);
    REP(i, fr) {
        cin >> x;
        frb[x] = true;
    }
    cin >> n >> m;
    vector<vector<int> > v(n);
    REP(i, m) {
        cin >> x >> y;
        v[--x].emplace_back(--y);
        v[y].emplace_back(x);
    }
    vector<int> power(n);
    REP(i, n)
        cin >> power[i];

    vector<int> from(n, -1);
    vector<bool> vis(n);
    queue<int> q;
    q.push(0);
    vis[0] = true;
    while (!q.empty()) {
        x = q.front();
        q.pop();
        for (auto it : v[x]) {
            if (!vis[it]) {
                q.push(it);
                vis[it] = true;
                from[it] = x;
            }
        }
    }
    if (from[n - 1] == -1) {
        cout << "-1\n";
        return 0;
    }
    vector<int> ord;
    x = n - 1;
    while (x != -1) {
        ord.emplace_back(x);
        x = from[x];
    }
    reverse(ord.begin(), ord.end());

    vector<vector<bool> > dp(n, vector<bool>(p + 1));
    vector<vector<bool> > chrg(n, vector<bool>(p + 1));
    dp[0][p] = true;
    int lay = 0;
    for (auto it : ord) {
        if (it == 0)
            continue;
        lay++;
        for (int i = p; i >= k; i--)
            dp[lay][i - k] = dp[lay - 1][i];
        for (int i = p; i >= 0; i--) {
            int nxt = i + power[lay];
            if (dp[lay][i] && nxt <= p && !dp[lay][nxt] && !frb[nxt])
                dp[lay][nxt] = chrg[lay][nxt] = true;
        }
    }

    int start = -1;
    for (int i = p; i >= 0; i--) {
        if (dp[lay][i]) {
            start = i;
            break;
        }
    }
    if (start == -1) {
        cout << "-1\n";
        return 0;
    }
    vector<int> stp;
    x = lay, y = start;
    while (x != 0) {
        if (chrg[x][y]) {
            stp.emplace_back(ord[x]);
            y -= power[x];
        }
        y += k;
        x--;
    }
    reverse(stp.begin(), stp.end());
    cout << ssize(ord) << ' ' << start << ' ' << ssize(stp) << '\n';
    for (auto it : ord)
        cout << it + 1 << ' ';
    cout << '\n';
    for (auto it : stp)
        cout << it + 1 << ' ';
    cout << '\n';
    return 0;
}
