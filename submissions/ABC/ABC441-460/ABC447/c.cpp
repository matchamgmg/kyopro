#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T>
using pq = priority_queue<T, vector<T>>;  // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>;  // 小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) \
    if (a) {  \
        pyes; \
    } else {  \
        pno;  \
    }
#define v_cout(a)                         \
    {                                     \
        int n = a.size();                 \
        rep(i, 0, n) cout << a[i] << " "; \
        cout << endl;                     \
    }
#define vv_cout(a)                                         \
    {                                                      \
        int n = a.size();                                  \
        rep(i, 0, n) {                                     \
            rep(j, 0, a[i].size()) cout << a[i][j] << " "; \
            cout << endl;                                  \
        }                                                  \
        cout << endl;                                      \
    }
#define grid_check(x, y, X, Y) (0 <= x && x < X && 0 <= y && y < Y)
template <class T>
bool chmax(T& a, T b) {
    if (a < b) {
        a = b;
        return true;
    } else
        return false;
}
template <class T>
bool chmin(T& a, T b) {
    if (a > b) {
        a = b;
        return true;
    } else
        return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S, T;
    cin >> S >> T;
    vector<int> cntS(26, 0), cntT(26, 0);
    for (char s : S) {
        cntS[s - 'A']++;
    }
    for (char s : T) {
        cntT[s - 'A']++;
    }
    bool ok = true;
    rep(i, 1, 26) { ok &= (cntS[i] == cntT[i]); }
    if (!ok) {
        cout << -1 << endl;
        return 0;
    }
    string SS = "", TT = "";
    for (char s : S) {
        if (s != 'A') SS += s;
    }
    for (char s : T) {
        if (s != 'A') TT += s;
    }
    if (SS != TT) {
        cout << -1 << endl;
        return 0;
    }
    deque<char> queS;
    queue<char> queT;
    for (char s : S) queS.push_back(s);
    for (char s : T) queT.push(s);
    int ans = 0;
    while (!queS.empty()) {
        if (!queT.empty() && queS.front() == queT.front()) {
            queS.pop_front();
            queT.pop();
        } else {
            if (queS.front() == 'A') {
                queS.pop_front();
                ans++;
            } else {
                queS.push_front('A');
                ans++;
            }
        }
    }
    ans += queT.size();
    cout << ans << endl;
}