#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

int main()
{
    int N,M;
    cin >> N >> M;
    vvi G(N);
    rep(i,0,M){
        int a,b;
        cin >> a >> b;
        a--;
        b--;
        G[a].push_back(b);
    }

    vi dist(N,1<<29);
    dist[0] = 0;
    pq_g<pair<int,int>> que;
    que.push({dist[0],0});
    int ans = 1<<29;
    while(!que.empty()){
        int v = que.top().second;
        int d = que.top().first;
        que.pop();
        if(d>dist[v]) continue;
        for(int e : G[v]){
            if(e==0){
                chmin(ans,dist[v]+1);
            }
            if(dist[e] > dist[v] + 1){
                dist[e] = dist[v] + 1;
                que.push({dist[e],e});
            }
        }
    }
    
    if(ans==(1<<29)){
        cout << -1 << endl;
        return 0;
    }
    cout << ans << endl;
}