#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T>
using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) \
    if (a)    \
    {         \
        pyes; \
    }         \
    else      \
    {         \
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
        rep(i, 0, n)                                       \
        {                                                  \
            rep(j, 0, a[i].size()) cout << a[i][j] << " "; \
            cout << endl;                                  \
        }                                                  \
        cout << endl;                                      \
    }
#define grid_check(x, y, X, Y) (0 <= x && x < X && 0 <= y && y < Y)
template <class T>
bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    else
        return false;
}
template <class T>
bool chmin(T &a, T b)
{
    if (a > b)
    {
        a = b;
        return true;
    }
    else
        return false;
}

// 重さWの段ボールを一番上に置いたとき
// その一つ下の段ボールは，耐荷重W以上
// 重さは小さい方がよい <- 嘘

// X = W + D を考える
// W1 <= D2 ... W1 + W2 <= D2 + W2 = X2
// W1 + W2 <= D3 ... W1 + W2 + W3 <= D3 + W3 = X3
// W1 + W2 + W3 <= X3 ==> X2 <= X3
// sum W(i-1) <= Di が成り立つ状態を維持する
// 乗せる個数を最大化する -> 重さの寄与が大きいものを削除

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<ll> W(N), D(N);
    rep(i, 0, N) cin >> W[i] >> D[i];
    vector<ll> X(N);
    rep(i, 0, N) X[i] = W[i] + D[i];
    vector<int> idx(N);
    iota(all(idx), 0);
    sort(all(idx), [&](int i, int j)
         { return X[i] < X[j]; });
    pq<ll> que;
    ll weight = 0;
    rep(i, 0, N)
    {
        weight += W[idx[i]];
        que.push(W[idx[i]]);
        if (weight > X[idx[i]])
        {
            weight -= que.top();
            que.pop();
        }
    }
    cout << que.size() << endl;
}