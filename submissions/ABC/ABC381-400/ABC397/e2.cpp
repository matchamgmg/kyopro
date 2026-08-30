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

// 木の葉の方から見る
// 次数1の頂点を順に削除していく
// 次数1の頂点を削除できるか
// 削除済みの辺を見て，パスの長さがKでないもの
// パスがちょうど二つ : 長さの和がK-1
// ひとつ : K - 1 以下
// なし : OK

struct UF {
    vector<int> par;
    UF(int n){ par.assign(n,-1); }

    int root(int x){
        if(par[x] < 0) return x;
        return par[x] = root(par[x]);
    }

    int size(int x){ return -par[root(x)]; }

    int same(int x, int y){ return root(x) == root(y); }

    bool merge(int x, int y){
        x = root(x); y = root(y);
        if(x == y) return false;
        if(size(x) > size(y)) swap(x, y);
        par[y] += par[x];
        par[x] = y;
        return true;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;
    vector<int> deg(N*K, 0);
    vector<vector<int>> G(N*K);
    rep(i,0,N*K-1){
        int u, v;
        cin >> u >> v;
        u--; v--;
        G[u].emplace_back(v);
        G[v].emplace_back(u);
        deg[u]++; deg[v]++;
    }

    UF uf(N*K);

    queue<int> que;
    rep(i,0,N*K){
        if(deg[i] == 1) que.emplace(i);
    }

    bool ans = true;
    while(ans && !que.empty()){
        int u = que.front();
        que.pop();
        int cnt = 0, len = 0;
        for(int v : G[u]){
            if(deg[v] == 0 && uf.size(v) < K){
                cnt++;
                len += uf.size(v);
            }
        }
        if(cnt == 0){
            for(int v : G[u]){
                if(deg[v] > 0){
                    deg[u]--; deg[v]--;
                    if(deg[v] <= 1) que.emplace(v);
                }
            }
        }else if(cnt == 1){
            for(int v : G[u]){
                if(deg[v] == 0 && uf.size(v) < K){
                    uf.merge(u, v);
                }
                if(deg[v] > 0){
                    deg[u]--; deg[v]--;
                    if(deg[v] <= 1) que.emplace(v);
                }
            }
        }else if(cnt == 2){
            if(len == K-1){
                for(int v : G[u]){
                    if(deg[v] == 0 && uf.size(v) < K){
                        uf.merge(u, v);
                    }
                    if(deg[v] > 0){
                        deg[u]--; deg[v]--;
                        if(deg[v] <= 1) que.emplace(v);
                    }
                }
            }else{
                ans = false;
            }
        }else{
            ans = false;
        }
    }

    YN(ans);
}