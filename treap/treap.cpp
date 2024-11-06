#include <bits/stdc++.h>
using namespace std;
using LL = long long;
#define FOR(i, l, r) for(int i = (l); i <= (r); ++i)
#define REP(i, n) FOR(i, 0, (n) - 1)
#define ssize(x) int(x.size())


mt19937 rng_key(0); 

vector<int> vals;

struct Treap {
    struct Node {
        int prio, val, cnt;
        Node *l = nullptr, *r = nullptr;
        bool rev = false;
        int mt, tl, cl, tr, cr, best;
        Node(int _val) : prio((int) rng_key()), val(_val) {
            mt = tl = tr = val;
            cl = cr = best = 1;
        }
    };

    using pNode = Node*;
    pNode root = nullptr;

    void push(pNode t) {
        if (t && t->rev) {
            t->rev = false;
            swap(t->l, t->r);
            swap(t->tl, t->tr);
            swap(t->cl, t->cr);
            if (t->l)
                t->l->rev ^= true;
            if (t->r)
                t->r->rev ^= true;
        }
    }

    int cnt(pNode t) { 
        return t ? t->cnt : 0;
    }

    void update(pNode t) {
        if (!t) 
            return;
        push(t);
        push(t->l);
        push(t->r);
        t->cnt = cnt(t->l) + cnt(t->r) + 1;
        t->tl = t->tr = t->mt;
        t->cl = t->cr = t->best = 1;
        if (!(t->l) && !(t->r)) {
            return;
        }
        else if (!(t->r)) {
            t->tl = t->l->tl;
            t->cl = t->l->cl;
            t->best = max(t->best, t->l->best);
            if (t->l->tr == t->mt)
                t->best = max(t->best, t->l->cr + 1);

            if (t->l->cl == t->l->cnt) {
                if (t->mt == t->l->tl)
                    t->cl += 1;
            }

            if (t->mt == t->l->tr)
                t->cr += t->l->cr;
        }
        else if (!(t->l)) {
            t->tr = t->r->tr;
            t->cr = t->r->cr;
            t->best = max(t->best, t->r->best);
            if (t->r->tl == t->mt)
                t->best = max(t->best, t->r->cl + 1);

            if (t->r->cr == t->r->cnt) {
                if (t->mt == t->r->tr)
                    t->cr += 1;
            }

            if (t->mt == t->r->tl)
                t->cl += t->r->cl;
        }
        else {
            t->tl = t->l->tl;
            t->tr = t->r->tr;
            t->cl = t->l->cl;
            t->cr = t->r->cr;
            t->best = max({t->best, t->l->best, t->r->best});
            if (t->l->tr == t->mt && t->r->tl == t->mt) {
                t->best = max(t->best, t->l->cr + t->r->cl + 1);
            }
            if (t->l->tr == t->mt) {
                t->best = max(t->best, t->l->cr + 1);
            }
            if (t->r->tl == t->mt) {
                t->best = max(t->best, t->r->cl + 1);
            }

            if (t->l->cl == t->l->cnt) {
                if (t->mt == t->l->tl) {
                    if (t->l->tl == t->r->tl)
                        t->cl += 1 + t->r->cl;
                    else
                        t->cl += 1;
                }
            }

            if (t->r->cr == t->r->cnt) {
                if (t->mt == t->r->tr) {
                    if (t->l->tr == t->r->tr)
                        t->cr += 1 + t->l->cr;
                    else
                        t->cr += 1;
                }
            }
        }
    }

    void heapify(pNode t) {
        pNode maxi = t;
        if (t->l && t->l->prio > maxi->prio)
            maxi = t->l;
        if (t->r && t->r->prio > maxi->prio)
            maxi = t->r;
        if (maxi != t) {
            swap(t->prio, maxi->prio);
            heapify(maxi);
        }
    }

    void builder(int x, int y, pNode &t) {
        if (x > y)
            return;
        int mid = (x + y) / 2;
        t = new Node(vals[mid]);
        builder(x, mid - 1, t->l);
        builder(mid + 1, y, t->r);
        heapify(t);
        update(t);
    }

    void build(int n) {
        builder(0, n - 1, root);
    }

    void split(pNode t, int key, pNode &l, pNode &r) {
        if (!t)
            l = r = nullptr;
        else {
            push(t);
            if (key <= cnt(t->l)) 
                split(t->l, key, l, t->l), r = t;
            else
                split(t->r, key - cnt(t->l) - 1, t->r, r), l = t;
        }
        update(t);
    }

    void merge(pNode &t, pNode l, pNode r) {
        push(l);
        push(r);
        if (!l || !r)
            t = (l ? l : r);
        else if (l->prio > r->prio)
            merge(l->r, l->r, r), t = l;
        else
            merge(r->l, l, r->l), t = r;
        update(t);
    }

    void reverse(int l, int r) {
        pNode t1, t2, t3;
        split(root, l, t1, t2);
        split(t2, r - l + 1, t2, t3);
        t2->rev ^= true;
        merge(root, t1, t2);
        merge(root, root, t3);
    }

    void find_answer(int l, int r) {
        pNode t1, t2, t3;
        split(root, l, t1, t2);
        split(t2, r - l + 1, t2, t3);
        cout << t2->best << '\n';
        merge(root, t1, t2);
        merge(root, root, t3);
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, q;
    string s;
    Treap treap;

    cin >> n >> q >> s;
    vals.resize(n);
    REP(i, n) {
        if (s[i] == 'A')
            vals[i] = 0;
        else if (s[i] == 'C')
            vals[i] = 1;
        else if (s[i] == 'G')
            vals[i] = 2;
        else
            vals[i] = 3;
    }

    treap.build(n);

    while (q--) {
        char c;
        int x, y, d;
        cin >> c >> x >> y;
        x--;
        y--;
        if (c == 'O') {
            treap.reverse(x, y);
        }
        else if (c == 'P') {
            cin >> d;
            d--;
            if (d < x) {
                treap.reverse(d, y);
                treap.reverse(d, d + y - x);
                treap.reverse(d + y - x + 1, y);
            }
            else if (d > x) {
                d += y - x;
                treap.reverse(x, d);
                treap.reverse(x, d - (y - x + 1));
                treap.reverse(d - (y - x), d);
            }
        }
        else {
            treap.find_answer(x, y);
        }
    }

    return 0;
}
