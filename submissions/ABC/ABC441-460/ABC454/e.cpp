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

void solve()
{
    int N, A, B;
    cin >> N >> A >> B;
    A--;
    B--;
    if (N % 2 == 1)
    {
        cout << "No" << endl;
        return;
    }
    if (B % 2 == 0 && A % 2 == 1)
    {
        cout << "Yes" << endl;

        rep(i, 0, B / 2)
        {
            rep(j, 0, N - 1)
            {
                cout << 'D';
            }
            cout << 'R';
            rep(j, 0, N - 1)
            {
                cout << 'U';
            }
            cout << 'R';
        }
        rep(i, 0, A / 2)
        {
            cout << "RDLD";
        }
        cout << "RD";
        rep(i, A / 2 + 1, N / 2)
        {
            cout << "DLDR";
        }

        rep(i, B / 2 + 1, N / 2)
        {
            cout << 'R';
            rep(j, 0, N - 1)
            {
                cout << 'U';
            }
            cout << 'R';
            rep(j, 0, N - 1)
            {
                cout << 'D';
            }
        }
        cout << endl;
    }
    else if (B % 2 == 1 && A % 2 == 0)
    {
        cout << "Yes" << endl;

        rep(i, 0, A / 2)
        {
            rep(j, 0, N - 1)
            {
                cout << 'R';
            }
            cout << 'D';
            rep(j, 0, N - 1)
            {
                cout << 'L';
            }
            cout << 'D';
        }
        rep(i, 0, B / 2)
        {
            cout << "DRUR";
        }
        cout << "DR";
        rep(i, B / 2 + 1, N / 2)
        {
            cout << "RURD";
        }

        rep(i, A / 2 + 1, N / 2)
        {
            cout << 'D';
            rep(j, 0, N - 1)
            {
                cout << 'L';
            }
            cout << 'D';
            rep(j, 0, N - 1)
            {
                cout << 'R';
            }
        }
        cout << endl;
    }
    else
    {
        cout << "No" << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (; T--;)
        solve();
}