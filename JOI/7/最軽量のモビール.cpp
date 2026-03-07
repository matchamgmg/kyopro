#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
//using mint = modint1000000007;
using vi = vector<int>;
using vll = vector<ll>;
using vld = vector<ld>;
using vmint = vector<mint>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using vvld = vector<vld>;
using vvmint = vector<vmint>;
using vvvi = vector<vvi>;
using vvvll = vector<vvll>;
using vvvld = vector<vvld>;
using vvvmint = vector<vvmint>;
using vb = vector<bool>;
using vvb = vector<vb>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

// 各棒の取りうる重さ : 最小重さの倍数

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector G(N, vector<int>(2));
    vector ratio(N, vector<int>(2));
    vector<int> parent(N, -1);
    rep(i,0,N){
        cin >> ratio[i][0] >> ratio[i][1] >> G[i][0] >> G[i][1];
        G[i][0]--; G[i][1]--;
        if(G[i][0] >= 0) parent[G[i][0]] = i;
        if(G[i][1] >= 0) parent[G[i][1]] = i;
    }

    auto gcd = [](auto g, ll a, ll b) -> ll {
        if(a % b == 0) return b;
        return g(g, b, a % b);
    };

    auto lcm = [&](ll a, ll b) -> ll {
        return a * b / gcd(gcd, a, b);
    };

    auto DFS = [&](auto f, int u) -> ll {
        if(u == -1) return 1;
        ll l = ratio[u][0];
        ll r = ratio[u][1];
        ll g = gcd(gcd, l, r);
        l /= g; r /= g;

        ll w1 = f(f, G[u][0]);
        ll w2 = f(f, G[u][1]);
        ll w = lcm(w1*l, w2*r);
        return w/l + w/r;
    };

    ll ans;
    rep(i,0,N){
        if(parent[i] == -1) ans = DFS(DFS, i);
    }
    cout << ans << endl;
}