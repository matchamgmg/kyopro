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

// 区間を伸ばすとき
// P[i] が最大値 -> 転倒数 +- 0
// それ以外 -> 転倒数 +

// 区間を縮めるとき
// P[i] が最小値 -> 転倒数 +- 0
// それ以外 -> 転倒数 -

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, K;
    cin >> N >> K;
    vector<int> P(N);
    rep(i, 0, N)
    {
        cin >> P[i];
        P[i]--;
    }
    fenwick_tree<ll> ft(N);
    ll tumble = 0;
    ll ans = 0;
    ll r = 0;
    rep(l, 0, N)
    {
        // 伸ばすとき
        ll ok = -1;
        if (tumble == K)
        {
            ok = r;
        }
        while (r < N)
        {
            ll add_tumble = ft.sum(P[r], N);

            if (tumble + add_tumble < K)
            {
                ft.add(P[r], 1);
                tumble += add_tumble;
                ++r;
            }
            else
            {
                if (tumble + add_tumble == K)
                {
                    ft.add(P[r], 1);
                    tumble += add_tumble;
                    ok = r;
                    ++r;
                }

                break;
            }
        }
        if (ok >= 0)
        {
            while (r < N)
            {
                ll add_tumble = ft.sum(P[r], N);

                if (add_tumble == 0)
                {
                    ft.add(P[r], 1);
                    ++r;
                }
                else
                {
                    ans += r - ok;
                    break;
                }
            }
        }
        // 縮めるとき
        ll del_tumble = ft.sum(0, P[l]);
        ft.add(P[l], -1);
        if (r == N && tumble == K)
        {
            ans++;
        }
        tumble -= del_tumble;
    }
    cout << ans << endl;
}