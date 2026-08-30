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

// FenwickTree
// r = 1, 2, ..., Nの順に配列B_rを作る
// B_r[i] = すべてのi<j<=rについてP_i!=P_jのとき1，そうでないとき0
// 1..rまで順に見たとき、その値が最後に出てきたところなら1、そうでなければ0
// はじめすべて1
// 配列の更新
// P_r == P_i (i<r)なる最後のiについてB_i = 0

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    vector<int> P(N);
    rep(i, 0, N) cin >> P[i];
    rep(i, 0, N) P[i]--;
    vector<int> L(Q), R(Q);
    rep(i, 0, Q) cin >> L[i] >> R[i];
    rep(i, 0, Q) L[i]--;
    vector<int> qidx(Q);
    iota(all(qidx), 0);
    sort(all(qidx), [&](int i, int j) {
        return R[i] == R[j] ? L[i] < L[j] : R[i] < R[j];
    });
    vector<int> ans(Q);
    vector<int> Pos(N, -1);
    fenwick_tree<int> ft(N);
    int qi = 0;
    rep(i, 0, N) {
        ft.add(i, 1);
        if (Pos[P[i]] >= 0) {
            ft.add(Pos[P[i]], -1);
        }
        Pos[P[i]] = i;
        while (qi < Q && R[qidx[qi]] < i + 1) qi++;
        while (qi < Q && R[qidx[qi]] == i + 1) {
            ans[qidx[qi]] = ft.sum(L[qidx[qi]], R[qidx[qi]]);
            qi++;
        }
    }
    rep(i, 0, Q) { cout << ans[i] << endl; }
}