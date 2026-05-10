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

const int INF = 2e9;
int op_max(int a, int b) { return max(a, b); }
int op_min(int a, int b) { return min(a, b); }
int e_max() { return -INF; }
int e_min() { return INF; }

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<int> L(M), R(M);
    rep(i, 0, M)
    {
        cin >> L[i] >> R[i];
        L[i]--;
    }
    vector<vector<int>> L_list(N + 1), R_list(N + 1);
    rep(i, 0, M)
    {
        L_list[L[i]].emplace_back(i);
        R_list[R[i]].emplace_back(i);
    }
    rep(i, 0, N + 1)
    {
        sort(all(L_list[i]), [&](int i, int j)
             { return R[i] < R[j]; });
        sort(all(R_list[i]), [&](int i, int j)
             { return L[i] < L[j]; });
    }

    segtree<int, op_max, e_max> seg_max(N + 1);
    segtree<int, op_min, e_min> seg_min(N + 1);
    rep(i, 0, N + 1)
    {
        if (!L_list[i].empty())
        {
            seg_min.set(i, R[L_list[i][0]]);
        }
        if (!R_list[i].empty())
        {
            seg_max.set(i, L[R_list[i].back()]);
        }
    }

    int Q;
    cin >> Q;

    auto query = [&](int s, int t) -> void
    {
        bool ans = false;
        ll L_ok = -1, L_ng = L_list[s].size();
        while (L_ng - L_ok > 1)
        {
            ll mid = (L_ok + L_ng) / 2;
            ([&](ll m)
                 { return R[L_list[s][m]] <= t; }(mid) ? L_ok : L_ng) = mid;
        }
        L_ok++;
        ll R_ng = -1, R_ok = R_list[t].size();
        while (R_ok - R_ng > 1)
        {
            ll mid = (R_ok + R_ng) / 2;
            ([&](ll m)
                 { return L[R_list[t][m]] >= s; }(mid) ? R_ok : R_ng) = mid;
        }
        R_ng++;
        R_ok = (ll)(R_list[t].size()) - R_ng;
        if (L_ok * R_ok == 0)
        {
            ans = false;
        }
        else if (L_ok * R_ok > 1)
        {
            int r = L_list[s][L_ok - 1];
            int l = R_list[t][R_list[t].size() - R_ok];
            ans = (R[r] >= L[l]);
        }
        else
        {
            int r = L_list[s][L_ok - 1];
            int l = R_list[t][R_list[t].size() - R_ok];
            if (l == r)
            {
                int rr = seg_min.prod(s + 1, N + 1);
                ans = (rr == -INF ? false : rr <= t - 1);
            }
            else
            {
                ans = (R[r] >= L[l]);
            }
        }
        YN(ans);
    };

    rep(q, 0, Q)
    {
        int S, T;
        cin >> S >> T;
        S--;
        query(S, T);
    }
}