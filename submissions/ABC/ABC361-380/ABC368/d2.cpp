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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

int main()
{
    int N,K;
    cin >> N >> K;
    vvi G(N);
    rep(i,0,N-1){
        int a,b;
        cin >> a >> b;
        a--; b--;
        G[a].push_back(b);
        G[b].push_back(a);
    }
    vi V;
    rep(i,0,K){
        int v;
        cin >> v;
        v--;
        V.push_back(v);
    }
    set<int> ans;
    int r = *V.begin();
    vi par(N,-1);
    auto dfs = [&](auto dfs, int p) -> void {
        for(int c : G[p]){
            if(c==r) continue;
            if(par[c]!=-1) continue;
            par[c] = p;
            dfs(dfs,c);
        }
        return;
    };
    dfs(dfs,r);
    ans.insert(r);
    for(int v : V){
        int u = v;
        while(u!=r){
            if(ans.count(u)) break;
            ans.insert(u);
            u = par[u];
        }
    }
    cout << ans.size() << endl;
}