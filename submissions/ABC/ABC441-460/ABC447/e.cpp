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
    vector<int> parent;
    UnionFind() {}
    UnionFind(int n_) : n(n_), group_num(n_) { parent.assign(n_, -1); }

    int root(int x) {
        if (parent[x] < 0) return x;
        return parent[x] = root(parent[x]);
    }

    int size(int x) {
        x = root(x);
        return -parent[x];
    }

    bool is_same(int x, int y) { return root(x) == root(y); }

    bool merge(int x, int y) {
        x = root(x);
        y = root(y);
        if (x == y) return false;
        if (size(x) > size(y)) swap(x, y);
        parent[y] += parent[x];
        parent[x] = y;
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
    UnionFind uf(N);
    mint ans(0);
    vector<int> U(M), V(M);
    rep(i, 0, M) cin >> U[i] >> V[i];
    rep(i, 0, M) U[i]--;
    rep(i, 0, M) V[i]--;
    rrep(i, 0, M) {
        if (uf.group_num == 2 && !uf.is_same(U[i], V[i])) {
            ans += mint(2).pow(i + 1);
            cout << U[i] + 1 << ' ' << V[i] + 1 << endl;
        } else {
            uf.merge(U[i], V[i]);
        }
    }
    cout << ans.val() << endl;
}