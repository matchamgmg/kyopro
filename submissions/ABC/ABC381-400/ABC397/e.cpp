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

int main()
{
    int N,K;
    cin >> N >> K;
    vvi G(N*K);
    rep(i,0,N*K-1){
        int a,b;
        cin >> a >> b;
        a--;
        b--;
        G[a].push_back(b);
        G[b].push_back(a);
    }
    dsu uf(N*K);
    queue<int> q;
    vector<int> remain_d(N*K,0);
    vector<bool> seen(N*K,false);
    rep(i,0,N*K){
        remain_d[i] = G[i].size();
        if(G[i].size()==1){
            seen[i] = true;
            q.push(i);
        }
    }

    while(!q.empty()){
        int p = q.front();
        q.pop();
        if(uf.size(p)==K){
            for(int n : G[p]){
                remain_d[n]--;
                if(remain_d[n]==1){
                    q.push(n);
                }
            }
            continue;
        }
        for(int n : G[p]){
            if(uf.size(p)+uf.size(n)>K){
                pno;
                return 0;
            }
            uf.merge(p,n);
            remain_d[n]--;
            if(remain_d[n]==1){
                q.push(n);
            }
        }
    }
    rep(i,0,N*K){
        if(uf.leader(i)==i){
            if(uf.size(i)!=K){
                pno;
                return 0;
            }
        }
    }
    pyes;
}