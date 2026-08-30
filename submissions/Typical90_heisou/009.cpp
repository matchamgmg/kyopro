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

    int N;
    cin >> N;
    vector<ll> X(N), Y(N);
    rep(i, 0, N) cin >> X[i] >> Y[i];

    auto f = [&](int c) -> double {
        vector<int> idx;
        rep(i, 0, N) {
            if (i != c) idx.emplace_back(i);
        }
        vector<ll> NX(N), NY(N);
        rep(i, 0, N - 1) {
            NX[idx[i]] = X[idx[i]] - X[c];
            NY[idx[i]] = Y[idx[i]] - Y[c];
        }
        sort(all(idx), [&](int i, int j) {
            bool upi, upj;
            upi = (NY[i] > 0 || (NY[i] == 0 && NX[i] > 0));
            upj = (NY[j] > 0 || (NY[j] == 0 && NX[j] > 0));
            if (upi != upj) return upi > upj;
            return NX[i] * NY[j] - NX[j] * NY[i] > 0;
        });
        vector<double> angle(2 * N);
        rep(i, 0, N - 1) {
            angle[idx[i]] = atan2(NY[idx[i]], NX[idx[i]]) * 180. / M_PI;
            if (angle[idx[i]] < 0) angle[idx[i]] += 360.;
        }
        rep(i, 0, N - 1) {
            idx.push_back(idx[i] + N);
            angle[idx[N - 1 + i]] = angle[idx[i]] + 360.;
        }
        double res = 0;
        rep(i, 0, N - 1) {
            auto itr =
                upper_bound(all(idx), angle[idx[i]] + 180.,
                            [&](double v, int e) { return v < angle[e]; });
            itr--;
            chmax(res, angle[*itr] - angle[idx[i]]);
        }
        return res;
    };

    double ans = 0;
    rep(i, 0, N) { chmax(ans, f(i)); }
    printf("%.10lf\n", ans);
}