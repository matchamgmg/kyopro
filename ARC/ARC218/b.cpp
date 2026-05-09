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

template <class T>
struct RunLength
{
    vector<pair<T, int>> code;
    RunLength() {}
    RunLength(const vector<T> &v)
    {
        pair<T, int> word = make_pair(v[0], 1);
        for (int i = 1; i < v.size(); ++i)
        {
            if (v[i] == word.first)
            {
                word.second++;
            }
            else
            {
                code.emplace_back(word);
                word = make_pair(v[i], 1);
            }
        }
        if (word.second > 0)
        {
            code.emplace_back(word);
        }
    }
    RunLength(const string &v)
    {
        pair<T, int> word = make_pair(v[0], 1);
        for (int i = 1; i < v.size(); ++i)
        {

            if (v[i] == word.first)
            {
                word.second++;
            }
            else
            {
                code.emplace_back(word);
                word = make_pair(v[i], 1);
            }
        }
        if (word.second > 0)
        {
            code.emplace_back(word);
        }
    }

    vector<pair<T, int>> &get_code()
    {
        return code;
    }

    friend ostream &operator<<(ostream &s, RunLength &rl)
    {
        for (pair<T, int> p : rl.code)
        {
            s << "(" << p.first << ", " << p.second << ") ";
        }
        return s;
    }
};

void solve()
{
    int N;
    cin >> N;
    vector<int> A(N);
    rep(i, 0, N)
    {
        cin >> A[i];
    }
    sort(all(A));
    RunLength<int> RL(A);

    vector<pair<int, int>> B = RL.get_code();
    int M = B.size();
    bool alice_win = false;
    int last = 0;
    int i = 0;
    while (i < M)
    {
        auto p = B[i];

        if (p.first == 0)
        {
            if (p.second > 1)
            {
                alice_win = true;
                break;
            }
            else
            {
                alice_win ^= true;
            }
        }
        if (p.first - last > 1)
        {
            alice_win ^= true;
            break;
        }
        else if (p.second > 1)
        {
            break;
        }
        last = p.first;
        ++i;
    }
    cout << (alice_win ? "Alice" : "Bob") << '\n';
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