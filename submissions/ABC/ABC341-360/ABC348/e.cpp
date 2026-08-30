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


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<vector<int>> G(N);
    rep(i,0,N-1){
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        G[a].emplace_back(b);
        G[b].emplace_back(a);
    }
    vector<ll> C(N);
    rep(i,0,N) cin >> C[i];
    vector<ll> SC(N,0), SD(N,0);

    auto DFS = [&](auto f, int u, int p) -> void {
        SC[u] = C[u];
        SD[u] = 0;
        for(int v : G[u]){
            if(v == p) continue;
            f(f, v, u);
            SC[u] += SC[v];
            SD[u] += SD[v] + SC[v];
        }
    };

    DFS(DFS,0,-1);

    vector<ll> F(N,0);

    auto rerooting = [&](auto f, int u, ll pSC, ll pSD, int p) -> void {
        F[u] = SD[u] + pSD;
        for(int v : G[u]){
            // v を根とする部分木の寄与を取り除く
            if(v == p) continue;
            ll nSC = pSC + SC[u] - SC[v];
            ll nSD = pSD + SD[u] - (SD[v] + SC[v]) + nSC;
            f(f, v, nSC, nSD, u);
        }
    };

    rerooting(rerooting,0,0,0,-1);

    // rep(i,0,N){
    //     cout << F[i] << ' ';
    // }
    // cout << endl;

    cout << *min_element(all(F)) << endl;
}