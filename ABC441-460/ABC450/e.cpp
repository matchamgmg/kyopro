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

// S_{infty} の Z 番目の文字をC_{infty}(Z)とおく
// |S_{n}| < Z を満たす最大のnについて
// C_{infty}(Z) = C_{n-1}(Z-|S_{n}|)

// S_{n}を求める

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using Answer = array<ll, 26>;
    Answer ans0;
    rep(i, 0, 26)
    {
        ans0[i] = 0;
    }
    auto Add_answer = [](Answer &l, const Answer &r) -> void
    {
        rep(i, 0, 26)
        {
            l[i] += r[i];
        }
    };
    auto Diff_answer = [](Answer &l, const Answer &r) -> void
    {
        rep(i, 0, 26)
        {
            l[i] -= r[i];
        }
    };

    string X, Y;
    cin >> X >> Y;
    ll len_max = 1e18;
    ll x = X.size(), y = Y.size();

    vector<ll> F_len(3);
    F_len[0] = 0;
    F_len[1] = y;
    F_len[2] = y + x;

    int cnt = 3;
    while (F_len[cnt - 1] <= len_max)
    {
        F_len.emplace_back(F_len[cnt - 1] + F_len[cnt - 2]);
        cnt++;
    }
    ll N = cnt - 1;

    vector<Answer> ansX(x + 1), ansY(y + 1);
    rep(i, 0, x)
    {
        rep(c, 0, 26)
        {
            ansX[i + 1][c] += ansX[i][c];
        }
        ansX[i + 1][X[i] - 'a']++;
    }
    rep(i, 0, y)
    {
        rep(c, 0, 26)
        {
            ansY[i + 1][c] += ansY[i][c];
        }
        ansY[i + 1][Y[i] - 'a']++;
    }

    map<ll, Answer> memo;
    auto solve = [&](auto f, ll n, ll Z) -> Answer
    {
        // Z < |F_{n}| なるnを求める
        // sub_solve(n, z) := F_{n} の z 文字目までについて各文字の個数を求める
        // solve(Z) = sub_solve(n, Z) = sub_solve(n-1, |F_{n-1}|) + solve(Z-|F_{n-1}|);
        Answer res = ans0;
        if (Z == 0)
        {
            return res;
        }
        else if (Z <= y)
        {
            Add_answer(res, ansY[Z]);
            return res;
        }
        else if (Z <= y + x)
        {
            Add_answer(res, ansY[y]);
            Add_answer(res, ansX[Z - y]);
            return res;
        }

        ll Fn_len = F_len[n];
        while (F_len[n - 1] > Z)
        {
            --n;
        }

        // n >= 3
        ll len_Fn = F_len[n - 1];
        ll len_rest = Z - len_Fn;
        if (!memo.contains(n - 1))
        {
            memo[n - 1] = f(f, n - 1, len_Fn);
        }
        Add_answer(res, memo[n - 1]);
        Add_answer(res, f(f, n - 2, len_rest));
        return res;
    };

    int Q;
    cin >> Q;
    rep(q, 0, Q)
    {
        ll L, R;
        char C;
        cin >> L >> R >> C;
        L--;
        Answer ansR = solve(solve, N, R), ansL = solve(solve, N, L);
        cout << ansR[C - 'a'] - ansL[C - 'a'] << endl;
    }
}