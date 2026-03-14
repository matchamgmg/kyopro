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

    int N, M;
    cin >> N >> M;
    vector<int> A(N);
    rep(i, 0, N) cin >> A[i];
    rep(i, 0, N) A[i]--;
    vector<ll> cnt(M, 0);
    rep(i, 0, N)
    {
        cnt[A[i]]++;
    }
    ll max_A = *max_element(all(cnt));
    vector<int> idx(M);
    iota(all(idx), 0);
    sort(all(idx), [&](int i, int j)
         { return cnt[i] == cnt[j] ? i < j : cnt[i] < cnt[j]; });
    vector<ll> B(M + 1, 0);
    B[0] = N;
    rep(i, 0, M)
    {
        B[i + 1] = B[i] + (cnt[idx[i + 1]] - cnt[idx[i]]) * (i + 1);
    }
    const ll INF = 2e18;
    B.emplace_back(INF);
    int Q;
    cin >> Q;
    vector<ll> query(Q);
    rep(q, 0, Q) cin >> query[q];
    rep(q, 0, Q)
    {
        ll X;
        cin >> X;
        ll k = lower_bound(all(B), X) - B.begin();
        if (k == 0)
        {
            cout << A[X - 1] + 1 << endl;
        }
        else
        {
            cout << idx[(X - 1 - B[k - 1]) % k] << endl;
        }
    }
}