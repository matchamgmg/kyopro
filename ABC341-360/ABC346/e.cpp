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

// クエリを逆順に処理
// 各行各列の確定したマスの個数を管理

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W, M;
    cin >> H >> W >> M;
    ll tate = 0, yoko = 0;
    vector<bool> seen_tate(W, false), seen_yoko(H, false);
    vector<int> T(M), A(M), X(M);
    rep(i, 0, M) { cin >> T[i] >> A[i] >> X[i]; }
    int N = 2e5 + 1;
    vector<ll> C(N, 0);
    rrep(i, 0, M) {
        if (T[i] == 1) {
            if (!seen_yoko[A[i] - 1]) {
                seen_yoko[A[i] - 1] = true;
                C[X[i]] += W - yoko;
                tate++;
            }
        } else {
            if (!seen_tate[A[i] - 1]) {
                seen_tate[A[i] - 1] = true;
                C[X[i]] += H - tate;
                yoko++;
            }
        }
    }
    ll zero = (H - tate) * (W - yoko);
    int K = 0;
    C[0] += zero;
    rep(i, 0, N) { K += (C[i] > 0); }
    cout << K << endl;
    rep(i, 0, N) {
        if (C[i] > 0) {
            cout << i << ' ' << C[i] << endl;
        }
    }
}