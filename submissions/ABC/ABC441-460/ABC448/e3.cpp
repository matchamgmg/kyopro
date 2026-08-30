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
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x)
{
    cout << (x ? "Yes" : "No") << endl;
}
template <class T>
void v_cout(const vector<T> &a)
{
    int n = a.size();
    rep(i, 0, n) cout << a[i] << " ";
    cout << endl;
}
template <class T>
void vv_cout(const vector<T> &a)
{
    int n = a.size();
    rep(i, 0, n)
    {
        rep(j, 0, a[i].size()) cout << a[i][j] << " ";
        cout << endl;
    }
}
bool grid_check(int x, int y, int X, int Y)
{
    return (0 <= x && x < X && 0 <= y && y < Y);
}
template <class T>
bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    else
    {
        return false;
    }
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
    {
        return false;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll K, M;
    cin >> K >> M;
    ll P = 10007;
    const ll MOD = P * M;
    vector<ll> C(K), L(K);
    rep(i, 0, K)
    {
        cin >> C[i] >> L[i];
    }

    ll N = 0;
    ll TEN = 1;

    rrep(i, 0, K)
    {
        ll c = C[i], l = L[i];
        ll Ni = c * TEN % MOD;
        ll repunit = 0, repunit_element = 1;
        ll TEN2 = 1, TEN3 = 10;
        while (l > 0)
        {
            if (l & 1)
            {
                repunit = repunit_element * TEN2 % MOD + repunit;
                repunit %= MOD;
                TEN2 *= TEN3;
                TEN2 %= MOD;
            }
            repunit_element = repunit_element * TEN3 % MOD + repunit_element;
            repunit_element %= MOD;
            TEN3 *= TEN3;
            TEN3 %= MOD;
            l >>= 1;
        }
        Ni *= repunit;
        Ni %= MOD;
        TEN *= TEN2;
        TEN %= MOD;
        N += Ni;
        N %= MOD;
    }
    cout << N / M << endl;
}