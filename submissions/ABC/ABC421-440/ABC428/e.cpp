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
    int N;
    cin >> N;
    vector G(N,vector<int>());
    rep(i,0,N-1){
        int a,b;
        cin >> a >> b;
        a--, b--;
        G[a].emplace_back(b);
        G[b].emplace_back(a);
    }
    auto bfs = [&](int s) -> vector<int> {
        vector<int> dist(N,-1);
        queue<int> que;
        dist[s] = 0;
        que.push(s);
        while(!que.empty()){
            int u = que.front();
            que.pop();
            for(int v : G[u]){
                if(dist[v]!=-1) continue;
                dist[v] = dist[u] + 1;
                que.push(v);
            }
        }
        return dist;
    };

    int s = 0;
    vector<int> dist1 = bfs(s);
    int mx = 0;
    rep(i,0,N){
        if(dist1[i]>=mx){
            mx = dist1[i];
            s = i;
        }
    }
    vector<int> dist2 = bfs(s);
    int g = s;
    mx = 0;
    rep(i,0,N){
        if(dist2[i]>=mx){
            mx = dist2[i];
            g = i;
        }
    }
    dist1 = bfs(g);
    vector<int> ans(N);
    rep(i,0,N){
        ans[i] = (dist1[i]==dist2[i] ? max(s,g) : dist1[i] > dist2[i] ? g : s);
    }
    rep(i,0,N){
        cout << ans[i]+1 << endl;
    }
}