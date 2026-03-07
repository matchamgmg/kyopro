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
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

struct Edge{
    int to;
    ll weight;
};

int main()
{
    int N,M;
    cin >> N >> M;
    vi X(M),Y(M);
    vll Z(M);
    vector<vector<Edge>> G(N);
    rep(i,0,M){
        cin >> X[i] >> Y[i] >> Z[i];
        X[i]--;
        Y[i]--;
        G[X[i]].push_back({Y[i],Z[i]});
        G[Y[i]].push_back({X[i],Z[i]});
    }

    map<int,set<int>> m;

    vector<bool> seen(N,false);
    vector<ll> A(N,-1);
    queue<int> q;
    rep(i,0,N){
        if(seen[i]) continue;
        seen[i] = true;
        A[i] = 0;
        m[i].insert(i);
        q.push(i);
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(auto e : G[u]){
                if(seen[e.to]){
                    if(A[e.to]!=(e.weight^A[u])){
                        cout << -1 << endl;
                        return 0;
                    }else{
                        continue;
                    }
                }
                seen[e.to] = true;
                A[e.to] = e.weight^A[u];
                m[i].insert(e.to);
                q.push(e.to);
            }
        }
    }
    vll Ans(N,0);
    for(auto p : m){
        ll max_Ai = -1;
        for(auto i : p.second){
            chmax(max_Ai,A[i]);
        }
        ll bits = 0;
        int d = 0;
        for(ll x = 1;x<=max_Ai;x<<=1){
            ll a = 0;
            for(auto i : p.second){
                a += (A[i]>>d)&1;
            }
            if(a*2<p.second.size()){
                bits += ((A[p.first]>>d)&1)<<d;
            }else{
                bits += (((A[p.first]>>d)&1)^1)<<d;
            }
            d++;
        }
        Ans[p.first] = bits;
    }
    rep(i,0,N){
        seen[i] = false;
    }
    rep(i,0,N){
        if(seen[i]) continue;
        seen[i] = true;
        q.push(i);
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(auto e : G[u]){
                if(seen[e.to]){
                    continue;
                }
                seen[e.to] = true;
                Ans[e.to] = e.weight^Ans[u];
                q.push(e.to);
            }
        }
    }
    v_cout(Ans);
}