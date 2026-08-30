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

// 1クエリあたりO(N logN)程度
// 距離の値O((N+Q)^2)
// クエリ1 : 距離の値+O(N+Q)
// クエリ2 : マージされる辺の個数は高々O((N+Q)^2)

struct UF {
    vector<int> par;
    UF(int n) {
        par.assign(n, -1);
    }

    int root(int x){
        if(par[x] < 0) return x;
        return par[x] = root(par[x]);
    }

    int size(int x){
        return -par[root(x)];
    }

    int same(int x, int y){
        return root(x) == root(y);
    }

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

    int N, Q;
    cin >> N >> Q;

    struct Edge {
        int dist;
        int from, to;
        bool ok;
        Edge(int d, int u, int v) :dist(d), from(u), to(v), ok(false) {}
        constexpr bool operator<(const Edge &e) const { return dist < e.dist; }
        constexpr bool operator>(const Edge &e) const { return dist > e.dist; }
    };

    pq_g<Edge> edges;
    UF uf(N+Q);

    vector<pair<int,int>> V(N+Q);

    auto calc_dist = [&](int a, int b) -> int {
        return abs(V[a].first-V[b].first) + abs(V[a].second-V[b].second);
    };

    rep(i,0,N){
        cin >> V[i].first >> V[i].second;
        rep(j,0,i){
            int k = calc_dist(i, j);
            edges.emplace(k,j,i);
        }
    }

    int t, a, b, u, v;
    int n = 0;
    for(;Q--;){
        cin >> t;
        if(t == 1){
            cin >> a >> b;
            V[N+n] = {a, b};
            rep(i,0,N+n){
                int k = calc_dist(i, N+n);
                edges.emplace(k,i,N+n);
            }
            n++;
        }else if(t == 2){
            bool ok = false;
            vector<Edge> que;
            while(!ok && !edges.empty()){
                Edge p = edges.top();
                while(!edges.empty() && p.dist == edges.top().dist){
                    Edge q = edges.top();
                    edges.pop();
                    if(!uf.same(q.from, q.to)){
                        ok = true;
                        q.ok = true;
                        que.emplace_back(q);
                    }
                }
            }

            if(ok){
                int k = que[0].dist;
                for(Edge &e : que){
                    uf.merge(e.from, e.to);
                }
                cout << k << endl;
            }else{
                cout << -1 << endl;
            }
        }else{
            cin >> u >> v;
            u--; v--;
            if(uf.same(u, v)){
                cout << "Yes" << endl;
            }else{
                cout << "No" << endl;
            }
        }
    }
}