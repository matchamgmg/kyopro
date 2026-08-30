#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T>
using pq = priority_queue<T, vector<T>>;  // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>;  // 小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) \
    if (a) {  \
        pyes; \
    } else {  \
        pno;  \
    }
#define v_cout(a)                         \
    {                                     \
        int n = a.size();                 \
        rep(i, 0, n) cout << a[i] << " "; \
        cout << endl;                     \
    }
#define vv_cout(a)                                         \
    {                                                      \
        int n = a.size();                                  \
        rep(i, 0, n) {                                     \
            rep(j, 0, a[i].size()) cout << a[i][j] << " "; \
            cout << endl;                                  \
        }                                                  \
        cout << endl;                                      \
    }
#define grid_check(x, y, X, Y) (0 <= x && x < X && 0 <= y && y < Y)
template <class T>
bool chmax(T& a, T b) {
    if (a < b) {
        a = b;
        return true;
    } else
        return false;
}
template <class T>
bool chmin(T& a, T b) {
    if (a > b) {
        a = b;
        return true;
    } else
        return false;
}

struct UnionFind {
    int n;
    int group_num;
    vector<int> par;
    UnionFind(int n_) : n(n_), group_num(n_) { par.assign(n_, -1); }
    int root(int x) {
        if (par[x] < 0) return x;
        return par[x] = root(par[x]);
    }
    int size(int x) { return -par[root(x)]; }
    bool same(int x, int y) { return root(x) == root(y); }
    bool merge(int x, int y) {
        x = root(x);
        y = root(y);
        if (x == y) return false;
        if (size(x) > size(y)) swap(x, y);
        par[y] += par[x];
        par[x] = y;
        group_num--;
        return true;
    }
    int groups() { return group_num; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<vector<int>> G1(N), G2(N);
    rep(i, 0, M) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        G1[a].emplace_back(b);
        G2[b].emplace_back(a);
    }
    UnionFind uf(N);
    auto CSS = [&]() -> void {
        stack<int> st;
        vector<bool> seen1(N, false);
        auto DFS1 = [&](auto f, int s) -> void {
            seen1[s] = true;
            for (int v : G1[s]) {
                if (seen1[v]) continue;
                f(f, v);
            }
            st.emplace(s);
            return;
        };
        rep(i, 0, N) {
            if (seen1[i]) continue;
            DFS1(DFS1, i);
        }
        vector<bool> seen2(N, false);
        auto DFS2 = [&](auto f, int s) -> void {
            seen2[s] = true;
            for (int v : G2[s]) {
                if (seen2[v]) continue;
                uf.merge(s, v);
                f(f, v);
            }
            return;
        };
        while (!st.empty()) {
            int s = st.top();
            st.pop();
            if (seen2[s]) continue;
            DFS2(DFS2, s);
        }
    };

    CSS();
    ll ans = 0;
    rep(i, 0, N) {
        if (i == uf.root(i)) {
            ll t = uf.size(i);
            ans += t * (t - 1) / 2;
        }
    }
    cout << ans << endl;
}