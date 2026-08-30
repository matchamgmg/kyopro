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

// x,yがMの倍数でない
// DFSしてngに当たればngを伝搬

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, A, B;
    cin >> M >> A >> B;
    vector<int> F(M * M, -1);
    rep(i, 0, M) {
        rep(j, 0, M) {
            int f = (A * j + B * i) % M;
            F[i * M + j] = j * M + f;
        }
    }
    vector<bool> ans(M * M, true);
    vector<bool> seen(M * M, false);
    auto DFS = [&](auto g, int s) -> bool {
        seen[s] = true;
        if (s % M == 0 || s / M == 0) {
            ans[s] = false;
            return false;
        }
        int v = F[s];
        if (seen[v]) {
            return ans[s] = ans[v];
        }
        return ans[s] = g(g, v);
    };
    rep(i, 0, M) {
        rep(j, 0, M) {
            if (!seen[i * M + j]) {
                DFS(DFS, i * M + j);
            }
        }
    }
    int cnt = 0;
    rep(i, 0, M) {
        rep(j, 0, M) { cnt += ans[i * M + j]; }
    }
    cout << cnt << endl;
}