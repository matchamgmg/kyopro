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

    int N, M, K;
    cin >> N >> M >> K;
    vector<int> T(M);
    rep(i, 0, M)
    {
        cin >> T[i];
    }
    vector<vector<int>> S(N, vector<int>(M, 0));
    rep(i, 0, N)
    {
        rep(j, 0, M)
        {
            cin >> S[i][j];
        }
    }
    vector<int> C(N, 0);
    rep(i, 0, N)
    {
        rep(j, 0, M)
        {
            C[i] += S[i][j];
        }
    }
    int cnt = 0;
    vector<int> ans;
    rep(i, 0, N)
    {
        bool ok = true;
        rep(j, 0, M)
        {
            ok &= (S[i][j] >= T[j]);
        }
        if (ok)
        {
            ans.emplace_back(i);
            cnt++;
        }
    }
    if (cnt <= K)
    {
        for (int i : ans)
        {
            cout << i + 1 << '\n';
        }
    }
    else
    {
        vector<int> ans_copy = ans;
        sort(all(ans_copy), [&](int i, int j)
             { return C[i] > C[j]; });
        int border = C[ans_copy[K - 1]];
        for (int i : ans)
        {
            if (C[i] >= border)
            {
                cout << i + 1 << '\n';
            }
        }
    }
}