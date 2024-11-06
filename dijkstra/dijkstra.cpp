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
    vector<pair<int, int> > hor(n), ver(n);
    REP(i, n) {
        int x, y;
        cin >> x >> y;
        hor[i] = {y, i};
        ver[i] = {x, i};
    }
    sort(hor.begin(), hor.end());
    sort(ver.begin(), ver.end());
    
    vector<vector<pair<int, int> > > dis(n);
    FOR(i, 0, n - 1) {
        auto &[x, a] = ver[i];
        auto &[y, b] = hor[i];
        if (i != 0) {
            dis[a].emplace_back(ver[i - 1].second, x - ver[i - 1].first);
            dis[b].emplace_back(hor[i - 1].second, y - hor[i - 1].first);
        }
        if (i != n - 1) {
            dis[a].emplace_back(ver[i + 1].second, ver[i + 1].first - x);
            dis[b].emplace_back(hor[i + 1].second, hor[i + 1].first - y);
        }
    }

    auto dijkstra = [&]() {
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<> > q;
        q.push({0, 0});
        vector<bool> vis(n);
        while (!q.empty()) {
            auto [d, x] = q.top();
            q.pop();
            if (x == n - 1)
                return d;
            if (vis[x])
                continue;
            vis[x] = true;
            for (auto &[y, e] : dis[x])
                q.push({d + e, y});
        }
        return -1;
    };

    cout << dijkstra() << '\n';

    return 0;
}
