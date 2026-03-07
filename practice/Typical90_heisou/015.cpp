#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
// using mint = modint998244353;
using mint = modint1000000007;
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

// n個からa個選ぶ問題 <- n個から差k以上となるようにa個選ぶ
// 帰着させる
// n個からa個選ぶ選び方はnCa
// 選んだボールの直後にk-1個の選ぶボールを挿入する
// n + (k-1)*(a-1)個から差k以上となるようにa個選ぶ問題と同じ

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N;
    cin >> N;
    vector<mint> kaijou(N + 1, 1);
    rep(i, 0, N) { kaijou[i + 1] = kaijou[i] * (i + 1); }
    rep(k, 1, N + 1) {
        mint ans(0);
        for (ll a = 1; (a - 1) * k + 1 <= N; ++a) {
            if (N - (k - 1) * (a - 1) <= 0 || N - (k - 1) * (a - 1) - a < 0)
                continue;
            ans += kaijou[N - (k - 1) * (a - 1)] * kaijou[a].inv() *
                   kaijou[N - (k - 1) * (a - 1) - a].inv();
        }
        cout << ans.val() << endl;
    }
}