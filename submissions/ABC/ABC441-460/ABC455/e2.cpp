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

    ll N;
    string S;
    cin >> N >> S;

    // A == B or B == C or C == A
    // N(A == B) + N(B == C) + N(C == A) - 3*N(A == B == C) + N(A == B == C)
    vector<ll> cnt(3, 0);
    vector<vector<ll>> diff(3, vector<ll>(2 * N + 1, 0));
    map<pair<ll, ll>, ll> mp_diff;
    diff[0][N] = diff[1][N] = diff[2][N] = 1;
    mp_diff[{0, 0}] = 1;
    ll AB = 0, BC = 0, CA = 0, ABC = 0;
    rep(i, 0, N)
    {
        cnt[S[i] - 'A']++;
        AB += diff[0][cnt[1] - cnt[0] + N]++;
        BC += diff[1][cnt[2] - cnt[1] + N]++;
        CA += diff[2][cnt[0] - cnt[2] + N]++;
        ABC += mp_diff[{cnt[1] - cnt[0], cnt[2] - cnt[0]}]++;
    }
    cout << N * (N + 1) / 2 - (AB + BC + CA - 2 * ABC) << endl;
}