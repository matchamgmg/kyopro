#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
using vi = vector<int>;
using vll = vector<ll>;
using vld = vector<ld>;
using vmint = vector<mint>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using vvld = vector<vld>;
using vvmint = vector<vmint>;
using vvvi = vector<vvi>;
using vvvll = vector<vvll>;
using vvvld = vector<vvld>;
using vvvmint = vector<vvmint>;
using vb = vector<bool>;
using vvb = vector<vb>;
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
#define input(A) rep(i, 0, A.size()) cin >> A[i]
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, A, B;
    cin >> N >> A >> B;
    vector<vector<int>> TB(N, vector<int>(N, 0)), AB(N, vector<int>(N, 0));
    rep(i, 0, A) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        r1--;
        c1--;
        TB[r1][c1] += 1;
        if (c2 < N) TB[r1][c2] -= 1;
        if (r2 < N) TB[r2][c1] -= 1;
        if (r2 < N && c2 < N) TB[r2][c2] += 1;
    }
    rep(i, 0, B) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        r1--;
        c1--;
        AB[r1][c1] += 1;
        if (c2 < N) AB[r1][c2] -= 1;
        if (r2 < N) AB[r2][c1] -= 1;
        if (r2 < N && c2 < N) AB[r2][c2] += 1;
    }
    rep(i, 0, N) {
        rep(j, 0, N - 1) {
            TB[i][j + 1] += TB[i][j];
            AB[i][j + 1] += AB[i][j];
        }
    }
    rep(i, 0, N - 1) {
        rep(j, 0, N) {
            TB[i + 1][j] += TB[i][j];
            AB[i + 1][j] += AB[i][j];
        }
    }
    int ans = 0;
    rep(i, 0, N) {
        rep(j, 0, N) { ans += (TB[i][j] > 0 && AB[i][j] > 0); }
    }
    cout << ans << endl;
}