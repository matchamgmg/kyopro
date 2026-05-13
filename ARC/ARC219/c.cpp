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

    ll INF = 1e18;
    ll H, W;
    cin >> H >> W;
    ll N;
    cin >> N;
    vector<ll> A(N), B(N);
    rep(i, 0, N)
    {
        cin >> A[i] >> B[i];
    }
    map<ll, vector<ll>> mp;
    rep(i, 0, N)
    {
        mp[A[i]].emplace_back(B[i]);
    }
    map<ll, vector<pair<ll, int>>> cost; // 0: ltr, 1: ltl, 2: rtr, 3: ltlrtr
    vector<int> cnt(4, 0);
    for (auto &[k, v] : mp)
    {
        sort(all(v));
        ll ltr = W - 1;
        ll ltl = (v.back() - 1) * 2;
        ll rtr = (W - v.front()) * 2;
        ll ltlrtr = rtr;
        rep(i, 0, v.size() - 1)
        {
            ll nd = (v[i] - 1) * 2 + (W - v[i + 1]) * 2;
            chmin(ltlrtr, nd);
        }
        cost[k].emplace_back(ltr, 0);
        cost[k].emplace_back(ltl, 1);
        cost[k].emplace_back(rtr, 2);
        cost[k].emplace_back(ltlrtr, 3);
        sort(all(cost[k]));
        cnt[cost[k][0].second]++;
    }
    ll ans = 0;
    for (auto &[k, v] : cost)
    {
        ans += v[0].first;
    }
    if (cnt[0] % 2 == 0 && (cnt[0] > 0 || (cnt[2] == 0 || cnt[3] == 0)))
    {

        cout << ans << endl;
    }
    else if (cnt[0] == 1 && (cnt[2] > 0 || cnt[3] > 0))
    {
        int key = -1;
        ll diff = INF;
        for (auto &[k, v] : cost)
        {
            if (v.front().second == 0)
            {
                continue;
            }
            ll ncost = v.front().first;
            rep(i, 1, 4)
            {
                if (chmax(diff, ncost - v[i].first))
                {
                    key = k;
                }
            }
        }
        cout << ans - diff << endl;
    }
    else
    {
        int key = -1;
        ll diff = INF;
        for (auto &[k, v] : cost)
        {
            ll ncost = v.front().first;
            rep(i, 1, 4)
            {
                if (chmax(diff, ncost - v[i].first))
                {
                    key = k;
                }
            }
        }
        cout << ans - diff << endl;
    }
}