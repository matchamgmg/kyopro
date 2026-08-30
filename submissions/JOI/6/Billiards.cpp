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
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, X;
    cin >> N >> X;
    struct Edge {
        int ball;
        ll cost;
        Edge() : ball(0), cost(0) {}
        Edge(int i, ll c) : ball(i), cost(c) {}
    };
    vll A(N);
    vi P(N);
    rep(i,0,N) cin >> A[i];
    rep(i,0,N) cin >> P[i];

    vector<vector<Edge>> G(N+1, vector<Edge>());
    rep(i,0,N){
        if(P[i] == -1) P[i] = 0;
        G[P[i]].emplace_back(i+1,A[i]);
    }

    int ans = 0;

    auto DFS = [&](auto f, int u) -> void {
        if(X < 0) return;
        chmax(ans, u);
        for(auto p : G[u]){
            X -= p.cost;
            f(f, p.ball);
            X += p.cost;
        }
        return;
    };

    DFS(DFS, 0);
    cout << (ans == 0 ? -1 : ans) << endl;
}