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

// Ai を素因数分解
// 各素因数の次数を管理
// Ai を除くとき
// 各素因数について、Ai による寄与が最大ならLCMは小さくなる

struct Factor
{
    pair<int, int> best, second_best; // degree, index
    Factor() : best({0, -1}), second_best({0, -1}) {}
    const bool operator<(const Factor &rhs)
    {
        return this->best == rhs.best ? this->second_best < rhs.second_best : this->best < rhs.best;
    }
    void add(int deg, int idx)
    {
        if (deg >= best.first)
        {
            second_best = best;
            best = {deg, idx};
        }
        else if (deg >= second_best.first)
        {
            second_best = {deg, idx};
        }
    }
};

vector<ll> calc_LPF(ll N)
{
    vector<ll> lpf(N + 1);
    vector<ll> primes;
    iota(all(lpf), 0);
    for (ll x = 2; x <= N; ++x)
    {
        if (lpf[x] == x)
            primes.emplace_back(x);
        for (ll p : primes)
        {
            if (p * x > N || p > lpf[x])
                break;
            lpf[p * x] = p;
        }
    }
    return lpf;
}

void solve(const vector<ll> &LPF)
{
    int N;
    cin >> N;
    vector<int> A(N);
    rep(i, 0, N)
    {
        cin >> A[i];
    }
    map<int, Factor> factors;
    vector<vector<int>> Ai_factor(N);
    rep(i, 0, N)
    {
        ll x = A[i];
        map<int, int> facts;
        while (x > 1)
        {
            facts[LPF[x]]++;
            x /= LPF[x];
        }
        for (auto [f, d] : facts)
        {
            factors[f].add(d, i);
            Ai_factor[i].emplace_back(f);
        }
    }
    mint ans = 1;
    for (auto [_, f] : factors)
    {
        ans *= mint(_).pow(f.best.first);
    }
    rep(i, 0, N)
    {
        mint ans_k = ans;
        for (int f : Ai_factor[i])
        {
            if (factors[f].best.second == i)
            {
                mint fac(f);
                ans_k *= fac.pow(factors[f].best.first - factors[f].second_best.first).inv();
            }
        }
        cout << ans_k.val() << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    vector<ll> LPF = calc_LPF(1e7 + 1);
    for (; T--;)
        solve(LPF);
}