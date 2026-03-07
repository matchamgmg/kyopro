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

// ちょうどMの重さにならない場合を考慮
// 重さj以下の最大値

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<int> A(N);
    vector<ll> B(N);
    rep(i, 0, N) cin >> A[i] >> B[i];
    const ll INF = 1LL << 60;
    vector<vector<ll>> dp1(N + 1, vector<ll>(M + 1, 0)),
        dp2(N + 1, vector<ll>(M + 1, 0));
    dp1[0][0] = 0;
    rep(i, 0, N) {
        rep(j, 0, M + 1) {
            chmax(dp1[i + 1][j], dp1[i][j]);
            if (j - A[i] >= 0) {
                chmax(dp1[i + 1][j], dp1[i][j - A[i]] + B[i]);
            }
        }
    }
    // vv_cout(dp1);
    // cout << endl;
    dp2[N][0] = 0;
    rrep(i, 0, N) {
        rep(j, 0, M + 1) {
            chmax(dp2[i][j], dp2[i + 1][j]);
            if (j - A[i] >= 0) {
                chmax(dp2[i][j], dp2[i + 1][j - A[i]] + B[i]);
            }
        }
    }
    // vv_cout(dp2);
    // cout << endl;

    ll ans = *max_element(all(dp1[N]));
    rep(i, 0, N) {
        bool ok = false;
        rep(j, 0, M + 1) {
            if (M - (j + A[i]) >= 0) {
                ok |= (dp1[i][j] + B[i] == ans - dp2[i + 1][M - (j + A[i])]);
            }
        }
        YN(ok);
    }
}